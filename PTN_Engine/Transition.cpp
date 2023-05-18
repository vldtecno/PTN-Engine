/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
 * Copyright (c) 2023 Eduardo Valgôde
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

#include "PTN_Engine/Transition.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/PTN_Exception.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/Utilities/DetectRepeated.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <mutex>
#include <ranges>
#include <tuple>

namespace ptne
{
using namespace std;

Transition::Transition(const vector<WeakPtrPlace> &activationPlaces,
                       const vector<size_t> &activationWeights,
                       const vector<WeakPtrPlace> &destinationPlaces,
                       const vector<size_t> &destinationWeights,
                       const vector<WeakPtrPlace> &inhibitorPlaces,
                       const vector<pair<string, ConditionFunction>> &additionalActivationConditions,
                       const bool requireNoActionsInExecution)
: m_additionalActivationConditions{ additionalActivationConditions }
, m_inhibitorPlaces(inhibitorPlaces)
, m_requireNoActionsInExecution(requireNoActionsInExecution)
{
	if (activationPlaces.size() != activationWeights.size() && !activationWeights.empty())
	{
		throw ActivationWeightDimensionException();
	}

	if (destinationPlaces.size() != destinationWeights.size() && !destinationWeights.empty())
	{
		throw DestinationWeightDimensionException();
	}

	utility::detectRepeated<Place, ActivationPlaceRepetitionException>(activationPlaces);

	utility::detectRepeated<Place, DestinationPlaceRepetitionException>(destinationPlaces);

	if (!activationWeights.empty())
	{
		for (size_t i = 0; i < activationPlaces.size(); ++i)
		{
			m_activationPlaces.push_back(tuple<WeakPtrPlace, size_t>(activationPlaces[i], activationWeights[i]));
		}
	}
	else
	{
		transform(activationPlaces.cbegin(), activationPlaces.cend(), back_inserter(m_activationPlaces),
				  [](const auto &ap) { return tuple<WeakPtrPlace, size_t>(ap, 1); });
	}

	if (!destinationWeights.empty())
	{
		for (size_t i = 0; i < destinationPlaces.size(); ++i)
		{
			if (destinationWeights[i] == 0)
			{
				throw ZeroValueWeightException();
			}
			m_destinationPlaces.push_back(
			tuple<WeakPtrPlace, size_t>(destinationPlaces[i], destinationWeights[i]));
		}
	}
	else
	{
		transform(destinationPlaces.cbegin(), destinationPlaces.cend(), back_inserter(m_destinationPlaces),
				  [](const auto &dp) { return tuple<WeakPtrPlace, size_t>(dp, 1); });
	}
}

Transition::Transition(Transition &&transition) noexcept
{
	unique_lock<shared_mutex> guard(m_mutex);
	moveMembers(transition);
}

bool Transition::execute()
{
	unique_lock<shared_mutex> guard(m_mutex);
	bool result = false;

	blockStartingOnEnterActions(true);

	if (!isActive())
	{
		result = false;
	}
	else
	{
		performTransit();
		result = true;
	}

	blockStartingOnEnterActions(false);

	return result;
}

bool Transition::isEnabled() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return isEnabledInternal();
}

bool Transition::isEnabledInternal() const
{
	if (!checkInhibitorPlaces())
	{
		return false;
	}

	if (!checkActivationPlaces())
	{
		return false;
	}

	return true;
}

bool Transition::isActive() const
{
	return isEnabledInternal() && (!m_requireNoActionsInExecution || noActionsInExecution()) &&
		   checkAdditionalConditions();
}

vector<tuple<Transition::WeakPtrPlace, size_t>> Transition::getActivationPlaces() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_activationPlaces;
}

vector<tuple<Transition::WeakPtrPlace, size_t>> Transition::getDestinationPlaces() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_destinationPlaces;
}

vector<std::pair<std::string, ConditionFunction>> Transition::getAdditionalActivationConditions() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_additionalActivationConditions;
}

std::vector<Transition::WeakPtrPlace> Transition::getInhibitorPlaces() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_inhibitorPlaces;
}

void Transition::export_(IExporter &exporter) const
{
	vector<tuple<string, size_t>> activationPlacesExport;
	transform(m_activationPlaces.cbegin(), m_activationPlaces.cend(), back_inserter(activationPlacesExport),
			  [](const auto &activationPlace)
			  {
				  tuple<string, size_t> activationPlaceExport;
				  if (auto activationPlaceShPtr = get<0>(activationPlace).lock())
				  {
					  get<0>(activationPlaceExport) = activationPlaceShPtr->getName();
					  get<1>(activationPlaceExport) = get<1>(activationPlace);
					  return activationPlaceExport;
				  }
				  else
				  {
					  throw PTN_Exception("Could not lock activation place weak pointer");
				  }
			  });

	vector<tuple<string, size_t>> destinationPlacesExport;
	transform(m_destinationPlaces.cbegin(), m_destinationPlaces.cend(), back_inserter(destinationPlacesExport),
			  [](const auto &destinationPlace)
			  {
				  tuple<string, size_t> destinationPlaceExport;
				  if (auto destinationPlaceShPtr = get<0>(destinationPlace).lock())
				  {
					  get<0>(destinationPlaceExport) = destinationPlaceShPtr->getName();
					  get<1>(destinationPlaceExport) = get<1>(destinationPlace);
					  return destinationPlaceExport;
				  }
				  else
				  {
					  throw PTN_Exception("Could not lock destination place weak pointer");
				  }
			  });

	vector<string> activationConditionsNames;
	transform(m_additionalActivationConditions.cbegin(), m_additionalActivationConditions.cend(),
			  back_inserter(activationConditionsNames),
			  [](const auto &activationCondition) { return activationCondition.first; });

	vector<string> inhibitorPlacesNames;
	transform(m_inhibitorPlaces.cbegin(), m_inhibitorPlaces.cend(), back_inserter(inhibitorPlacesNames),
			  [](const auto &inhibitorPlace)
			  {
				  if (auto inhibitorPlaceShPtr = inhibitorPlace.lock())
				  {
					  return inhibitorPlaceShPtr->getName();
				  }
				  else
				  {
					  throw PTN_Exception("Could not lock inhibitor place weak pointer");
				  }
			  });

	exporter.exportTransition(activationPlacesExport, destinationPlacesExport, activationConditionsNames,
							  inhibitorPlacesNames, requireNoActionsInExecution());
}

bool Transition::requireNoActionsInExecution() const
{
	return m_requireNoActionsInExecution;
}

bool Transition::checkInhibitorPlaces() const
{
	for (const WeakPtrPlace &inhibitorPlace : m_inhibitorPlaces)
	{
		SharedPtrPlace spInhibitorPlace = lockWeakPtr(inhibitorPlace);

		if (spInhibitorPlace->getNumberOfTokens() > 0)
		{
			return false;
		}
	}
	return true;
}

bool Transition::checkActivationPlaces() const
{
	for (const tuple<WeakPtrPlace, size_t> &tupleActivationPlace : m_activationPlaces)
	{
		const WeakPtrPlace &activationPlace = get<0>(tupleActivationPlace);
		const size_t activationWeight = get<1>(tupleActivationPlace);

		SharedPtrPlace spPlace = lockWeakPtr(activationPlace);

		if (spPlace->getNumberOfTokens() < activationWeight)
		{
			return false;
		}
	}
	return true;
}

bool Transition::checkAdditionalConditions() const
{
	for (const auto &p : m_additionalActivationConditions)
	{
		const ConditionFunction &activationCondition = p.second;
		if (!activationCondition)
		{
			throw PTN_Exception("Invalid activation condition " + p.first);
		}
		else
		{
			if (!(activationCondition)())
			{
				return false;
			}
		}
	}
	return true;
}

bool Transition::noActionsInExecution() const
{
	for (const tuple<WeakPtrPlace, size_t> &tupleActivationPlace : m_activationPlaces)
	{
		const WeakPtrPlace &activationPlace = get<0>(tupleActivationPlace);

		SharedPtrPlace spPlace = lockWeakPtr(activationPlace);

		if (spPlace->isOnEnterActionInExecution())
		{
			return false;
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
	for (tuple<WeakPtrPlace, size_t> &tupleActivationPlace : m_activationPlaces)
	{
		const WeakPtrPlace &activationPlace = get<0>(tupleActivationPlace);
		const size_t activationWeight = get<1>(tupleActivationPlace);

		if (SharedPtrPlace spPlace = lockWeakPtr(activationPlace))
		{
			spPlace->exitPlace(activationWeight);
		}
	}
}

void Transition::enterDestinationPlaces()
{
	for (tuple<WeakPtrPlace, size_t> &tupleDestinationPlace : m_destinationPlaces)
	{
		const WeakPtrPlace &destinationPlace = get<0>(tupleDestinationPlace);
		const size_t destinationWeight = get<1>(tupleDestinationPlace);

		if (SharedPtrPlace spPlace = lockWeakPtr(destinationPlace))
		{
			spPlace->enterPlace(destinationWeight);
		}
	}
}

void Transition::moveMembers(Transition &transition)
{
	m_activationPlaces = std::move(transition.m_activationPlaces);
	m_destinationPlaces = std::move(transition.m_destinationPlaces);
	m_additionalActivationConditions = std::move(transition.m_additionalActivationConditions);
	m_inhibitorPlaces = std::move(transition.m_inhibitorPlaces);
}

void Transition::blockStartingOnEnterActions(const bool value)
{
	if (!m_requireNoActionsInExecution)
	{
		return;
	}

	for (tuple<WeakPtrPlace, size_t> &tupleActivationPlace : m_activationPlaces)
	{
		const WeakPtrPlace &activationPlace = get<0>(tupleActivationPlace);
		if (SharedPtrPlace spPlace = lockWeakPtr(activationPlace))
		{
			spPlace->blockStartingOnEnterActions(value);
		}
	}
}
} // namespace ptne
