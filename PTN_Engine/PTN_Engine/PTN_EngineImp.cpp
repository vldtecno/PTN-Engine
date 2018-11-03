/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "PTN_Engine/PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/PTN_Engine/Transition.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include "PTN_Engine/Utilities/DetectRepeated.h"

namespace ptne
{
	using namespace std;

	PTN_Engine::PTN_EngineImp::PTN_EngineImp():
		m_stop{false}
	{}

	PTN_Engine::PTN_EngineImp::~PTN_EngineImp()
	{}

	void PTN_Engine::PTN_EngineImp::createTransition(
			const vector<string>& activationPlaces,
			const vector<size_t>& activationWeights,
			const vector<string>& destinationPlaces,
			const vector<size_t>& destinationWeights,
			const vector<string>& inhibitorPlaces,
			const vector<ConditionFunctorPtr>& additionalConditions)
	{
		unique_lock<mutex> guard(m_mutex);
		vector<WeakPtrPlace> activationPlacesVector =
				getPlacesFromNames(activationPlaces);

		vector<WeakPtrPlace> destinationPlacesVector =
				getPlacesFromNames(destinationPlaces);

		vector<WeakPtrPlace> inhibitorPlacesVector =
				getPlacesFromNames(inhibitorPlaces);

		m_transitions.push_back(
				Transition(activationPlacesVector,
						activationWeights,
						destinationPlacesVector,
						destinationWeights,						
						inhibitorPlacesVector,
						additionalConditions));
	}

	void PTN_Engine::PTN_EngineImp::createTransition(
		const vector<string>& activationPlaces,
		const vector<size_t>& activationWeights,
		const vector<string>& destinationPlaces,
		const vector<size_t>& destinationWeights,
		const vector<string>& inhibitorPlaces,
		const vector<string>& additionalConditions)
	{
		createTransition(
			activationPlaces,
			activationWeights,
			destinationPlaces,
			destinationWeights,
			inhibitorPlaces,
			getConditionFunctors(additionalConditions));
	}

	vector<Transition*> PTN_Engine::PTN_EngineImp::collectActiveTransitionsRandomly()
	{
		vector<Transition*> activeTransitions;
		for(Transition& transition : m_transitions)
		{
			if(transition.isActive())
			{
				activeTransitions.push_back(&transition);
			}
		}
		random_shuffle(activeTransitions.begin(), activeTransitions.end());
		return activeTransitions;
	}

	void PTN_Engine::PTN_EngineImp::createPlace(
		const string& name,
		const size_t initialNumberOfTokens,
		ActionFunctorPtr onEnterAction,
		ActionFunctorPtr onExitAction,
		const bool input)
	{
		unique_lock<mutex> guard(m_mutex);
		if(m_places.find(name)!= m_places.end())
		{
			throw RepeatedPlaceException(name);
		}

		SharedPtrPlace place = make_shared<Place>(initialNumberOfTokens,
			onEnterAction,
			onExitAction,
			input);

		m_places[name] = place;
		if(place->isInputPlace())
		{
			m_inputPlaces.push_back(place);
		}
	}

	void PTN_Engine::PTN_EngineImp::createPlaceStr(
		const string& name,
		const size_t initialNumberOfTokens,
		const string& onEnterAction,
		const string& onExitAction,
		const bool input)
	{
		ActionFunctorPtr onEnter = getActionFunctor(onEnterAction);
		ActionFunctorPtr onExit = getActionFunctor(onExitAction);
		createPlace(name, initialNumberOfTokens, onEnter, onExit, input);
	}

	void PTN_Engine::PTN_EngineImp::registerAction(
		const string& name,
		ActionFunctorPtr action)
	{
		unique_lock<mutex> guard(m_mutex);
		if(name.empty())
		{
			throw InvalidFunctionNameException(name);
		}

		if(m_actions.find(name)!= m_actions.end())
		{
			throw RepeatedActionException(name);
		}

		//verificar acções repetidas
		m_actions[name] = action;
	}

	void PTN_Engine::PTN_EngineImp::registerCondition(
		const string& name,
		ConditionFunctorPtr condition)
	{
		unique_lock<mutex> guard(m_mutex);
		if(name.empty())
		{
			throw InvalidFunctionNameException(name);
		}

		if(m_conditions.find(name)!= m_conditions.end())
		{
			throw RepeatedConditionException(name);
		}

		//verificar se há repetidos?
		m_conditions[name] = condition;
	}

	void PTN_Engine::PTN_EngineImp::execute(const bool log, ostream& o)
	{
		unique_lock<mutex> guard(m_mutex);
		m_stop = false;
		bool transitionFired;

		do
		{
			if (log)
			{
				printState(o);
			}

			//TODO: Create an iterator for active transitions
			//Safe to use raw pointers here. Nothing justifies deleting a
			// transition from m_transitions, so there should never be an
			// invalid pointer. At the moment this is only single threaded, 
			// so synchronization problems are not an issue.
			vector<Transition*> activeTransitions(
				collectActiveTransitionsRandomly());

			transitionFired = false;
			for (Transition* transition : activeTransitions)
			{
				transitionFired |= transition->execute();
			}
		} while (transitionFired && !m_stop);

		clearInputPlaces();
		m_stop = false;
	}

	void PTN_Engine::PTN_EngineImp::execute()
	{
		execute(false);
	}

	void PTN_Engine::PTN_EngineImp::clearInputPlaces()
	{
		for( const WeakPtrPlace& place : m_inputPlaces)
		{
			SharedPtrPlace spPlace = lockWeakPtr(place);
			spPlace->setNumberOfTokens(0);
		}
	}

	ActionFunctorPtr PTN_Engine::PTN_EngineImp::getActionFunctor(const string& name) const
	{
		if(name.empty())
		{
			return nullptr;
		}

		if(m_actions.find(name) == m_actions.end())
		{
			throw InvalidFunctionNameException(name);
		}
		return m_actions.at(name);
	}

	vector<ConditionFunctorPtr> PTN_Engine::PTN_EngineImp::getConditionFunctors(const vector<string>& names) const
	{
		vector<ConditionFunctorPtr> conditions;
		for_each(names.cbegin(), names.cend(), [&](const string& name)
		{
			if(name.empty())
			{
				return;
			}

			if(m_conditions.find(name) == m_conditions.end())
			{
				throw InvalidFunctionNameException(name);
			}
			conditions.push_back(m_conditions.at(name));
		});
		return conditions;
	}

	size_t PTN_Engine::PTN_EngineImp::getNumberOfTokens(const string& place) const
	{
		unique_lock<mutex> guard(m_mutex);
		if(m_places.find(place) == m_places.end())
		{
			throw InvalidNameException(place);
		}
		return m_places.at(place)->getNumberOfTokens();
	}

	void PTN_Engine::PTN_EngineImp::incrementInputPlace(const string& place)
	{
		unique_lock<mutex> guard(m_mutex);

		if(m_places.find(place) == m_places.end())
		{
			throw InvalidNameException(place);
		}
		if(!m_places.at(place)->isInputPlace())
		{
			throw NotInputPlaceException(place);
		}
		m_places.at(place)->enterPlace(1);
	}

	vector<WeakPtrPlace> PTN_Engine::PTN_EngineImp::getPlacesFromNames(const vector<string>& placesNames) const
	{
		utility::detectRepeatedNames<string, RepeatedPlaceNamesException>(placesNames);

		vector<WeakPtrPlace> placesVector;
		for(const auto& placeName : placesNames)
		{
			if(m_places.find(placeName) == m_places.end())
			{
				throw InvalidNameException(placeName);
			}
			placesVector.push_back(m_places.at(placeName));
		}
		return placesVector;
	}

	PTN_Engine::PTN_EngineImp::InvalidNameException::InvalidNameException(const string& name):
		PTN_Exception("Invalid name: " + name +".")
	{}

	PTN_Engine::PTN_EngineImp::RepeatedPlaceNamesException::RepeatedPlaceNamesException():
		PTN_Exception("Tried to create transition with repeated places.")
	{}

	PTN_Engine::PTN_EngineImp::NotInputPlaceException::NotInputPlaceException(const string& name):
		PTN_Exception(name + " is not an input place.")
	{}

	PTN_Engine::PTN_EngineImp::RepeatedPlaceException::RepeatedPlaceException(const string& name):
		PTN_Exception("Trying to add an already existing place: "+name+".")
	{}

	PTN_Engine::PTN_EngineImp::RepeatedActionException::RepeatedActionException(const string& name):
		PTN_Exception("Trying to add an already existing action: "+name+".")
	{}

	PTN_Engine::PTN_EngineImp::RepeatedConditionException::RepeatedConditionException(const string& name):
		PTN_Exception("Trying to add an already existing condition: "+name+".")
	{}

	PTN_Engine::PTN_EngineImp::InvalidFunctionNameException::InvalidFunctionNameException(const string& name):
		PTN_Exception("The function is not yet registered: "+name+".")
	{}

	void PTN_Engine::PTN_EngineImp::printState(ostream& o ) const
	{
		o << "Place; Tokens" << endl;
		for (const auto& p : m_places)
		{
			o << p.first.c_str() << ": " << p.second->getNumberOfTokens() << endl;
		}
		o << endl << endl;
	}
}
