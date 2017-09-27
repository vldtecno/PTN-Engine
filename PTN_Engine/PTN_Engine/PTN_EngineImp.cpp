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
#include <algorithm>
#include <set>

namespace ptne
{
	using namespace std;

	static size_t getNumberOfUniqueNames(const vector<string>& names)
	{
		set<string> s;
		for(string name : names)
		{
			s.insert(name);
		}
		return s.size();
	}

	PTN_EngineImp::PTN_EngineImp():
		m_stop{false}
	{}

	PTN_EngineImp::~PTN_EngineImp()
	{}

	void PTN_EngineImp::createTransition(
			const vector<string>& activationPlaces,
			const vector<size_t>& activationWeights,
			const vector<string>& destinationPlaces,
			const vector<size_t>& destinationWeights,
			const vector<string>& inhibitorPlaces,
			const vector<ConditionFunctorPtr>& additionalConditions)
	{
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

	void PTN_EngineImp::createTransition(
			const vector<string>& activationPlaces,
			const vector<string>& destinationPlaces,			
			const vector<string>& inhibitorPlaces,
			const vector<ConditionFunctorPtr>& additionalConditions)
	{
		createTransition(
			activationPlaces, 
			vector<size_t>{},
			destinationPlaces,
			vector<size_t>{},
			inhibitorPlaces,
			additionalConditions);
	}

	void PTN_EngineImp::createTransition(
		const vector<string>& activationPlaces,
		const vector<size_t>& activationWeights,
		const vector<string>& destinationPlaces,
		const vector<size_t>& destinationWeights,
		const vector<string>& inhibitorPlaces)
	{
		createTransition(
			activationPlaces, 
			activationWeights, 
			destinationPlaces, 
			destinationWeights, 
			inhibitorPlaces, 
			vector<ConditionFunctorPtr>{});
	}

	void PTN_EngineImp::createTransition(
		const vector<string>& activationPlaces,
		const vector<size_t>& activationWeights,
		const vector<string>& destinationPlaces,
		const vector<size_t>& destinationWeights,
		const vector<ConditionFunctorPtr>& additionalConditions)
	{
		createTransition(
			activationPlaces,
			activationWeights,
			destinationPlaces,
			destinationWeights,	
			vector<string>{},
			vector<ConditionFunctorPtr>{});
	}

	void PTN_EngineImp::createTransition(
		const vector<string>& activationPlaces,
		const vector<string>& destinationPlaces)
	{
		createTransition(
			activationPlaces,
			vector<size_t>{},
			destinationPlaces, 
			vector<size_t>{},
			vector<string>{},
			vector<ConditionFunctorPtr>{});
	}

	void PTN_EngineImp::createTransition(
		const vector<string>& activationPlaces,
		const vector<string>& destinationPlaces,
		const vector<string>& inhibitorPlaces)
	{
		createTransition(
			activationPlaces,
			vector<size_t>{},
			destinationPlaces,
			vector<size_t>{},
			inhibitorPlaces, 
			vector<ConditionFunctorPtr>{});
	}

	void PTN_EngineImp::createTransition(
		const vector<string>& activationPlaces,
		const vector<string>& destinationPlaces,
		const vector<ConditionFunctorPtr>& additionalConditions)
	{
		createTransition(
			activationPlaces,
			vector<size_t>{},
			destinationPlaces,
			vector<size_t>{},
			vector<string>{},
			vector<ConditionFunctorPtr>{});
	}

	void PTN_EngineImp::execute()
	{
		m_stop = false;
		bool transitionFired;

		do
		{
			//Safe to use raw pointers here. Nothing justifies deleting a
			// transition from m_transitions, so there should never be an
			// invalid pointer. At this moment this is single threaded, so
			// synchronization problems are not an issue.
			vector<Transition*> activeTransitions(move(
					collectActiveTransitionsRandomly()));

			transitionFired = false;
			for(Transition* transition : activeTransitions)
			{
				transitionFired |= transition->execute();
			}
		} while(transitionFired && !m_stop);

		clearInputPlaces();
		m_stop = false;
	}

	vector<Transition*> PTN_EngineImp::collectActiveTransitionsRandomly()
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


	void PTN_EngineImp::createPlace(
			const string& name,
			const size_t initialNumberOfTokens,
			ActionFunctorPtr onEnterAction,
			ActionFunctorPtr onExitAction,
			const bool input)
	{
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

	void PTN_EngineImp::createPlace(
		const string& name,
		const size_t initialNumberOfTokens,
		const bool input)
	{
		createPlace(name, initialNumberOfTokens, nullptr, nullptr, input);
	}

	void PTN_EngineImp::createPlace(
		const string& name,
		const size_t initialNumberOfTokens,
		ActionFunctorPtr onEnterAction,
		const bool input)
	{
		createPlace(name, initialNumberOfTokens, onEnterAction, nullptr, input);
	}

	void PTN_EngineImp::clearInputPlaces()
	{
		for( const WeakPtrPlace& place : m_inputPlaces)
		{
			SharedPtrPlace spPlace = lockWeakPtrNotNull(place);
			spPlace->setNumberOfTokens(0);
		}
	}

	size_t PTN_EngineImp::getNumberOfTokens(const string& place) const
	{
		if(m_places.find(place) == m_places.end())
		{
			throw InvalidNameException(place);
		}
		return m_places.at(place)->getNumberOfTokens();
	}

	void PTN_EngineImp::incrementInputPlace(const string& place)
	{
		if(m_places.find(place) == m_places.end())
		{
			throw InvalidNameException(place);
		}
		if(!m_places.at(place)->isInputPlace())
		{
			throw NotInputPlaceException(place);
		}
		m_places.at(place)->increaseNumberOfTokens(1);
	}

	vector<WeakPtrPlace> PTN_EngineImp::getPlacesFromNames(const vector<string>& placesNames) const
	{
		if(placesNames.size() != getNumberOfUniqueNames(placesNames))
		{
			throw RepeatedPlaceNamesException();
		}

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

	PTN_EngineImp::InvalidNameException::InvalidNameException(const string& name):
		PTN_Exception("Invalid name: " + name +".")
	{}

	PTN_EngineImp::RepeatedPlaceNamesException::RepeatedPlaceNamesException():
		PTN_Exception("Tried to create transition with repeated places.")
	{}

	PTN_EngineImp::NotInputPlaceException::NotInputPlaceException(const string& name):
		PTN_Exception(name + " is not an input place.")
	{}

	PTN_EngineImp::RepeatedPlaceException::RepeatedPlaceException(const string& name):
		PTN_Exception("Trying to add an already existing place: "+name+".")
	{}

}
