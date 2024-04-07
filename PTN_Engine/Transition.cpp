/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
 * Copyright (c) 2024 Eduardo Valgôde
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
#include "PTN_Engine/PTN_Exception.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/Utilities/DetectRepeated.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <algorithm>
#include <array>
#include <mutex>
#include <vector>

namespace ptne
{
using namespace std;

Transition::~Transition() = default;

Transition::Transition(const string &name,
                       const vector<Arc> &activationArcs,
                       const vector<Arc> &destinationArcs,
                       const vector<Arc> &inhibitorArcs,
                       const vector<pair<string, ConditionFunction>> &additionalActivationConditions,
                       const bool requireNoActionsInExecution)
: m_name(name)
, m_activationArcs(activationArcs)
, m_destinationArcs(destinationArcs)
, m_additionalActivationConditions(additionalActivationConditions)
, m_inhibitorArcs(inhibitorArcs)
, m_requireNoActionsInExecution(requireNoActionsInExecution)
{
	auto getPlacesFromArcs = [](const vector<Arc> &arcs)
	{
		vector<WeakPtrPlace> places;
		ranges::transform(arcs, back_inserter(places), [](const auto &arc) { return arc.place; });
		return places;
	};

	utility::detectRepeated<Place, ActivationPlaceRepetitionException>(getPlacesFromArcs(activationArcs));
	utility::detectRepeated<Place, DestinationPlaceRepetitionException>(getPlacesFromArcs(destinationArcs));
	utility::detectRepeated<Place, InhibitorPlaceRepetitionException>(getPlacesFromArcs(inhibitorArcs));

	auto validateWeights = [](const vector<Arc> &arcs)
	{
		if (ranges::find_if(arcs, [](const auto &arc) { return arc.weight == 0; }) != arcs.cend())
		{
			throw ZeroValueWeightException();
		}
	};

	validateWeights(activationArcs);
	validateWeights(destinationArcs);
	validateWeights(inhibitorArcs);
}

string Transition::getName() const
{
	shared_lock guard(m_mutex);
	return m_name;
}

bool Transition::execute()
{
	unique_lock guard(m_mutex);
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
	shared_lock guard(m_mutex);
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

vector<Arc> Transition::getActivationArcs() const
{
	shared_lock guard(m_mutex);
	return m_activationArcs;
}

vector<Arc> Transition::getDestinationArcs() const
{
	shared_lock guard(m_mutex);
	return m_destinationArcs;
}

vector<pair<string, ConditionFunction>> Transition::getAdditionalActivationConditions() const
{
	shared_lock guard(m_mutex);
	return m_additionalActivationConditions;
}

vector<Arc> Transition::getInhibitorArcs() const
{
	shared_lock guard(m_mutex);
	return m_inhibitorArcs;
}

bool Transition::checkInhibitorPlaces() const
{
	auto numberOfTokensGreaterThan0 = [](const auto &inhibitorArc)
	{
		SharedPtrPlace spInhibitorArc = lockWeakPtr(inhibitorArc.place);
		return spInhibitorArc->getNumberOfTokens() > 0;
	};

	if (ranges::find_if(m_inhibitorArcs, numberOfTokensGreaterThan0) != m_inhibitorArcs.end())
	{
		return false;
	}
	return true;
}

TransitionProperties Transition::getTransitionProperties() const
{
	shared_lock guard(m_mutex);

	TransitionProperties transitionProperties;

	auto getAdditionalConditionsNames = [this]()
	{
		vector<string> additionalActivationConditionNames;
		for (const auto &[additionalActivationConditionName, _] : m_additionalActivationConditions)
		{
			additionalActivationConditionNames.push_back(additionalActivationConditionName);
		}
		return additionalActivationConditionNames;
	};

	auto getAdditionalConditions = [this]()
	{
		vector<ConditionFunction> additionalActivationConditions;
		for (const auto &[_, additionalActivationCondition] : m_additionalActivationConditions)
		{
			additionalActivationConditions.push_back(additionalActivationCondition);
		}
		return additionalActivationConditions;
	};

	transitionProperties.additionalConditionsNames = getAdditionalConditionsNames();

	auto getProperties = [this](const vector<Arc> &arcs, ArcProperties::Type type)
	{
		vector<ArcProperties> arcsProperties;
		for (const auto &arc : arcs)
		{
			auto spPlace = lockWeakPtr(arc.place);
			arcsProperties.emplace_back(arc.weight, spPlace->getName(), getName(), type);
		}
		return arcsProperties;
	};
	transitionProperties.additionalConditions = getAdditionalConditions();

	transitionProperties.activationArcs = getProperties(getActivationArcs(), ArcProperties::Type::ACTIVATION);
	transitionProperties.destinationArcs = getProperties(getDestinationArcs(), ArcProperties::Type::DESTINATION);
	transitionProperties.inhibitorArcs = getProperties(getInhibitorArcs(), ArcProperties::Type::INHIBITOR);
	transitionProperties.name = getName();
	transitionProperties.requireNoActionsInExecution = m_requireNoActionsInExecution;

	return transitionProperties;
}

void Transition::addArc(const shared_ptr<Place> &place, const ArcProperties::Type type, const size_t weight)
{
	unique_lock guard(m_mutex);

	auto addArcTo = [&place, weight](auto &placesContainer)
	{
		auto sameNameAs = [&place](const auto &arc)
		{ return lockWeakPtr(arc.place)->getName() == place->getName(); };

		if (ranges::find_if(placesContainer, sameNameAs) != placesContainer.cend())
		{
			throw PTN_Exception("Arc already exists");
		}
		placesContainer.push_back({ place, weight });
	};

	using enum ArcProperties::Type;
	switch (type)
	{
	default:
	{
		throw PTN_Exception("Unexpected type");
	}
	case ACTIVATION:
	{
		addArcTo(m_activationArcs);
		break;
	}
	case BIDIRECTIONAL:
	{
		addArcTo(m_activationArcs);
		addArcTo(m_destinationArcs);
		break;
	}
	case DESTINATION:
	{
		addArcTo(m_destinationArcs);
		break;
	}
	case INHIBITOR:
	{
		addArcTo(m_inhibitorArcs);
		break;
	}
	}
}

void Transition::removeArc(const shared_ptr<Place> &place, const ArcProperties::Type type)
{
	unique_lock guard(m_mutex);

	auto removePlaceFrom = [&place](auto &placesContainer)
	{
		auto sameNameAs = [&place](const auto &arc)
		{ return lockWeakPtr(arc.place)->getName() == place->getName(); };

		auto it = ranges::find_if(placesContainer, sameNameAs);
		if (it != placesContainer.cend())
		{
			placesContainer.erase(it);
		}
		else
		{
			throw PTN_Exception("Cannot remove palce " + place->getName());
		}
	};

	using enum ArcProperties::Type;
	switch (type)
	{
	default:
	{
		throw PTN_Exception("Unexpected type");
	}
	case ACTIVATION:
	{
		removePlaceFrom(m_activationArcs);
		break;
	}
	case BIDIRECTIONAL:
	{
		removePlaceFrom(m_activationArcs);
		removePlaceFrom(m_destinationArcs);
		break;
	}
	case DESTINATION:
	{
		removePlaceFrom(m_destinationArcs);
		break;
	}
	case INHIBITOR:
	{
		removePlaceFrom(m_inhibitorArcs);
		break;
	}
	}
}

bool Transition::checkActivationPlaces() const
{
	for (const Arc &activationArc : m_activationArcs)
	{
		const WeakPtrPlace &activationPlace = activationArc.place;
		const size_t activationWeight = activationArc.weight;

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
	for (const auto &[name, activationCondition] : m_additionalActivationConditions)
	{
		if (!activationCondition)
		{
			throw PTN_Exception("Invalid activation condition " + name);
		}
		else
		{
			if (!activationCondition())
			{
				return false;
			}
		}
	}
	return true;
}

bool Transition::noActionsInExecution() const
{
	for (const Arc &activationArc : m_activationArcs)
	{
		const WeakPtrPlace &activationPlace = activationArc.place;

		SharedPtrPlace spPlace = lockWeakPtr(activationPlace);
		if (spPlace->isOnEnterActionInExecution())
		{
			return false;
		}
	}
	return true;
}

void Transition::performTransit() const
{
	exitActivationPlaces();
	enterDestinationPlaces();
}

void Transition::exitActivationPlaces() const
{
	for (const Arc &activationArc : m_activationArcs)
	{
		const WeakPtrPlace &activationPlace = activationArc.place;
		const size_t activationWeight = activationArc.weight;

		if (SharedPtrPlace spPlace = lockWeakPtr(activationPlace))
		{
			spPlace->exitPlace(activationWeight);
		}
	}
}

void Transition::enterDestinationPlaces() const
{
	for (const Arc &destinationArc : m_destinationArcs)
	{
		const WeakPtrPlace &destinationPlace = destinationArc.place;
		const size_t destinationWeight = destinationArc.weight;

		if (SharedPtrPlace spPlace = lockWeakPtr(destinationPlace))
		{
			spPlace->enterPlace(destinationWeight);
		}
	}
}

void Transition::blockStartingOnEnterActions(const bool value) const
{
	if (!m_requireNoActionsInExecution)
	{
		return;
	}

	for (const Arc &activationArc : m_activationArcs)
	{
		const WeakPtrPlace &activationPlace = activationArc.place;
		if (SharedPtrPlace spPlace = lockWeakPtr(activationPlace))
		{
			spPlace->blockStartingOnEnterActions(value);
		}
	}
}
} // namespace ptne
