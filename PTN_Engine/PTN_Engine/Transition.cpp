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

namespace ptne
{
	using namespace std;

	Transition::Transition(const vector<WeakPtrPlace>& activationPlaces,
		const vector<WeakPtrPlace>& destinationPlaces,
		const vector<ConditionFunctorPtr>& additionalActivationConditions):
			m_activationPlaces{activationPlaces},
			m_destinationPlaces{destinationPlaces},
			m_additionalActivationConditions{additionalActivationConditions}
	{}

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

	bool Transition::checkActivationPlaces() const
	{
		for( const WeakPtrPlace activationPlace : m_activationPlaces)
		{
			if(SharedPtrPlace spPlace = activationPlace.lock())
			{
				if(spPlace->getNumberOfTokens() < 1) //only arcs with weight 1 supported in this implementation
				{
					return false;
				}
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
		for( WeakPtrPlace activationPlace : m_activationPlaces)
		{
			if(SharedPtrPlace spPlace = activationPlace.lock())
			{
				spPlace->exitPlace();
			}
		}
	}

	void Transition::enterDestinationPlaces()
	{
		for(WeakPtrPlace destinationPlace : m_destinationPlaces)
		{
			if(SharedPtrPlace spPlace = destinationPlace.lock())
			{
				spPlace->enterPlace();
			}
		}
	}

}
