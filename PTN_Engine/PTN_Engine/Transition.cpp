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

#include "PTN_Engine/PTN_Engine/Transition.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/IConditionFunctor.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <set>


namespace ptne
{
	using namespace std;

	static size_t getNumberOfUniquePlaces(const vector<Transition::WeakPtrPlace>& places)
	{
		set<Transition::WeakPtrPlace, owner_less<Transition::WeakPtrPlace>> s;
		for(Transition::WeakPtrPlace place : places)
		{
			s.insert(place);
		}
		return s.size();
	}

	Transition::Transition(
		const vector<WeakPtrPlace>& activationPlaces,
		const vector<size_t>& activationWeights,
		const vector<WeakPtrPlace>& destinationPlaces,
		const vector<size_t>& destinationWeights,		
		const vector<WeakPtrPlace>& inhibitorPlaces,
		const vector<ConditionFunctorPtr>& additionalActivationConditions):
			m_additionalActivationConditions{additionalActivationConditions},
			m_inhibitorPlaces(inhibitorPlaces)
	{
		if(activationPlaces.size() != activationWeights.size() && activationWeights.size() != 0)
		{
			throw ActivationWeightDimensionException();
		}

		if(destinationPlaces.size() != destinationWeights.size() && destinationWeights.size() != 0)
		{
			throw DestinationWeightDimensionException();
		}

		if(activationPlaces.size() != getNumberOfUniquePlaces(activationPlaces))
		{
			throw ActivationPlaceRepetitionException();
		}

		if(destinationPlaces.size() != getNumberOfUniquePlaces(destinationPlaces))
		{
			throw DestinationPlaceRepetitionException();
		}

		if (activationWeights.size() != 0)
		{
			for (size_t i = 0; i < activationPlaces.size(); ++i)
			{
				m_activationPlaces.push_back(tuple<WeakPtrPlace, size_t>(activationPlaces[i], activationWeights[i]));
			}
		}
		else
		{
			for (const auto& ap : activationPlaces)
			{
				m_activationPlaces.push_back(tuple<WeakPtrPlace, size_t>(ap, 1));
			}
		}

		if (destinationWeights.size() != 0)
		{
			for (size_t i = 0; i < destinationPlaces.size(); ++i)
			{
				if (destinationWeights[i] == 0)
				{
					throw ZeroValueWeightException();
				}
				m_destinationPlaces.push_back(tuple<WeakPtrPlace, size_t>(destinationPlaces[i], destinationWeights[i]));
			}
		}
		else
		{
			for (const auto& dp : destinationPlaces)
			{
				m_destinationPlaces.push_back(tuple<WeakPtrPlace, size_t>(dp, 1));
			}
		}
	}

	bool Transition::execute()
	{
		if (!isActive())
		{
			return false;
		}

		performTransit();

		return true;
	}

	bool Transition::isActive() const
	{
		if(!checkInhibitorPlaces())
		{
			return false;
		}

		if(!checkActivationPlaces())
		{
			return false;
		}

		if(!checkAdditionalConditions())
		{
			return false;
		}

		return true;
	}

	bool Transition::checkInhibitorPlaces() const
	{
		for(const WeakPtrPlace& inhibitorPlace: m_inhibitorPlaces)
		{
			SharedPtrPlace spInhibitorPlace = lockWeakPtrNotNull(inhibitorPlace);

			if(spInhibitorPlace->getNumberOfTokens() > 0)
			{
				return false;
			}

		}
		return true;
	}

	bool Transition::checkActivationPlaces() const
	{
		for( const tuple<WeakPtrPlace, size_t>& tupleActivationPlace : m_activationPlaces)
		{
			const WeakPtrPlace& activationPlace = get<0>(tupleActivationPlace);
			const size_t activationWeight = get<1>(tupleActivationPlace);

			SharedPtrPlace spPlace = lockWeakPtrNotNull(activationPlace);

			if(spPlace->getNumberOfTokens() < activationWeight)
			{
				return false;
			}

		}
		return true;
	}

	bool Transition::checkAdditionalConditions() const
	{
		for( const ConditionFunctorPtr& activationCondition : m_additionalActivationConditions)
		{
			if(!activationCondition)
			{
				return false;
			}
			else
			{
				if( !((*activationCondition)()) )
				{
					return false;
				}
			}
		}
		return true;
	}

	void Transition::performTransit()
	{
		exitActivationPlaces();
		enterDestinationPlaces();
	}

	void Transition::exitActivationPlaces()
	{
		for( tuple<WeakPtrPlace, size_t>& tupleActivationPlace : m_activationPlaces)
		{
			WeakPtrPlace& activationPlace = get<0>(tupleActivationPlace);
			const size_t activationWeight = get<1>(tupleActivationPlace);

			if(SharedPtrPlace spPlace = lockWeakPtr(activationPlace))
			{
				spPlace->exitPlace(activationWeight);
			}
		}
	}

	void Transition::enterDestinationPlaces()
	{
		for( tuple<WeakPtrPlace, size_t>& tupleDestinationPlace : m_destinationPlaces)
		{
			WeakPtrPlace& destinationPlace = get<0>(tupleDestinationPlace);
			const size_t destinationWeight = get<1>(tupleDestinationPlace);

			if (SharedPtrPlace spPlace = lockWeakPtr(destinationPlace))
			{
				spPlace->enterPlace(destinationWeight);
			}
		}
	}

	Transition::ActivationWeightDimensionException::ActivationWeightDimensionException():
		PTN_Exception("The number of activation weights must be the same as the number of activation places.")
	{}

	Transition::DestinationWeightDimensionException::DestinationWeightDimensionException() :
		PTN_Exception("The number of destination weights must be the same as the number of destination places.")
	{}

	Transition::ZeroValueWeightException::ZeroValueWeightException() :
		PTN_Exception("Weights cannot be 0.")
	{}

	Transition::ActivationPlaceRepetitionException::ActivationPlaceRepetitionException() :
		PTN_Exception("Repetition of activation places is not permitted.")
	{}

	Transition::DestinationPlaceRepetitionException::DestinationPlaceRepetitionException() :
		PTN_Exception("Repetition of destination places is not permitted.")
	{}


}
