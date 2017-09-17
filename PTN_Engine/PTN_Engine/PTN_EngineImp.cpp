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

namespace ptne
{
	using namespace std;

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
			const vector<ConditionFunctorPtr>& additionalConditions,
			const vector<string>& inhibitorPlaces)
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
						additionalConditions,
						inhibitorPlacesVector));
	}

	void PTN_EngineImp::createTransition(
			const vector<string>& activationPlaces,
			const vector<string>& destinationPlaces,
			const vector<ConditionFunctorPtr>& additionalConditions,
			const vector<std::string>& inhibitorPlaces)
	{
		vector<WeakPtrPlace> activationPlacesVector;
		for(const auto& activationPlace : activationPlaces)
		{
			if(m_places.find(activationPlace) == m_places.end())
			{
				throw PTN_Exception("Invalid name: " + activationPlace);
			}
			activationPlacesVector.push_back(m_places.at(activationPlace));
		}

		vector<WeakPtrPlace> destinationPlacesVector;
		for(const auto& destinationPlace : destinationPlaces)
		{
			if(m_places.find(destinationPlace) == m_places.end())
			{
				throw PTN_Exception("Invalid name: " + destinationPlace);
			}
			destinationPlacesVector.push_back(m_places.at(destinationPlace));
		}

		vector<WeakPtrPlace> inhibitorPlacesVector;
		for(const auto& inhibitorPlace : inhibitorPlaces)
		{
			if(m_places.find(inhibitorPlace) == m_places.end())
			{
				throw PTN_Exception("Invalid name: " + inhibitorPlace);
			}
			inhibitorPlacesVector.push_back(m_places.at(inhibitorPlace));
		}

		m_transitions.push_back(
				Transition(activationPlacesVector,
						destinationPlacesVector,
						additionalConditions,
						inhibitorPlacesVector));
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


	void PTN_EngineImp::addPlace(
			const string& name,
			const size_t initialNumberOfTokens,
			ActionFunctorPtr onEnterAction,
			ActionFunctorPtr onExitAction,
			const bool input)
	{
		if(m_places.find(name)!= m_places.end())
		{
			throw PTN_Exception("Trying to add an already existing place");
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
			throw PTN_Exception("Invalid place " + place);
		}
		return m_places.at(place)->getNumberOfTokens();
	}

	void PTN_EngineImp::incrementInputPlace(const string& place)
	{
		if(m_places.find(place) == m_places.end())
		{
			throw PTN_Exception("Invalid place " + place);
		}
		if(!m_places.at(place)->isInputPlace())
		{
			throw PTN_Exception(place + " is not an input place");
		}
		m_places.at(place)->increaseNumberOfTokens(1);
	}

	vector<WeakPtrPlace> PTN_EngineImp::getPlacesFromNames(const vector<string>& placesNames) const
	{
		vector<WeakPtrPlace> placesVector;
		for(const auto& placeName : placesNames)
		{
			if(m_places.find(placeName) == m_places.end())
			{
				throw PTN_Exception("Invalid name: " + placeName);
			}
			placesVector.push_back(m_places.at(placeName));
		}
		return placesVector;
	}

}
