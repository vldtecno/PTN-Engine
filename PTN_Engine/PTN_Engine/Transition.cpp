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


namespace ptne
{
	using namespace std;

	Transition::Transition(const vector<WeakPtrPlace>& activationPlaces,
		const vector<WeakPtrPlace>& destinationPlaces,
		const vector<ConditionFunctorPtr>& additionalActivationConditions,
		const vector<WeakPtrPlace>& inhibitorPlaces):
			m_additionalActivationConditions{additionalActivationConditions},
			m_inhibitorPlaces(inhibitorPlaces)
	{
		for(size_t i = 0; i < activationPlaces.size(); ++i)
		{
			m_activationPlaces.push_back(tuple<WeakPtrPlace, size_t>(activationPlaces[i],1));
		}

		for(size_t i = 0; i < destinationPlaces.size(); ++i)
		{
			m_destinationPlaces.push_back(tuple<WeakPtrPlace, size_t>(destinationPlaces[i],1));
		}
	}

	Transition::Transition(
		const vector<WeakPtrPlace>& activationPlaces,
		const vector<size_t>& activationWeights,
		const vector<WeakPtrPlace>& destinationPlaces,
		const vector<size_t>& destinationWeights,
		const vector<ConditionFunctorPtr>& additionalActivationConditions,
		const vector<WeakPtrPlace>& inhibitorPlaces):
			m_additionalActivationConditions{additionalActivationConditions},
			m_inhibitorPlaces(inhibitorPlaces)
	{
		if(activationPlaces.size() != activationWeights.size())
		{
			throw ActivationWeightDimensionException();
		}

		if(destinationPlaces.size() != destinationWeights.size())
		{
			throw DestinationWeightDimensionException();
		}

		for(size_t i = 0; i < activationPlaces.size(); ++i)
		{
			if(activationWeights[i] == 0)
			{
				throw ZeroValueWeightException();
			}
			m_activationPlaces.push_back(tuple<WeakPtrPlace, size_t>(activationPlaces[i], activationWeights[i]));
		}

		for(size_t i = 0; i < destinationPlaces.size(); ++i)
		{
			if(destinationWeights[i] == 0)
			{
				throw ZeroValueWeightException();
			}
			m_destinationPlaces.push_back(tuple<WeakPtrPlace, size_t>(destinationPlaces[i], destinationWeights[i]));
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


}
