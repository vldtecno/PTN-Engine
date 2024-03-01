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

#pragma once

#include "PTN_Engine/PTN_Engine.h"
#include <functional>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace ptne
{
class Place;

//! Shared pointer to member functions of the controller.
using ConditionFunction = std::function<bool(void)>;

//! Shared pointer type to a place in the net.
using SharedPtrPlace = std::shared_ptr<Place>;

//! Weak pointer type to a place in the net.
using WeakPtrPlace = std::weak_ptr<Place>;

//!
//! \brief The Arc class
//!
struct Arc
{
	//!
	//! \brief place
	//!
	WeakPtrPlace place;

	//!
	//! \brief weight
	//!
	size_t weight = 1;
};

//!
//! \brief Implements a Petri net transition.
//!
class Transition final
{
public:
	//! \brief Constructor.
	//! \param name Name and identifier of the transition.
	//! \param activationPlaces Collection of incoming places.
	//! \param activationWeights Weights of each activation place.
	//! \param destinationPlaces Collection of outgoing places.
	//! \param destinationWeights Weights of each destination place.
	//! \param additionalActivationConditions Boolean function from the controller that can block firing.
	//! \param inhibitorPlaces Places that must be empty for the transition to fire.
	//!
	Transition( const std::string& name,
				const std::vector<Arc> &activationArcs,
				const std::vector<Arc> &destinationArcs,
				const std::vector<Arc> &inhibitorArcs,
				const std::vector<std::pair<std::string, ConditionFunction>> &additionalActivationConditions,
				const bool requireNoActionsInExecution);

	~Transition() = default;
	Transition(const Transition &) = delete;
	Transition(Transition &&transition) noexcept;
	Transition &operator=(Transition &) = delete;
	Transition &operator=(Transition &&) = delete;

	//!
	//! \brief getName
	//! \return
	//!
	std::string getName() const;

	//!
	//! Evaluate the activation places and transit the tokens if possible.
	//! \return true if token transit was performed, false if not.
	//!
	bool execute();

	//!
	//! Evaluates if the transition can attempt to be fired.
	//! \return true if can attepmted to be fired, false if it cannot.
	//!
	bool isEnabled() const;

	//!
	//! \brief getActivationPlaces Gets the activation places of the transition.
	//! \return A vector of Arcs with the activation places and their weights.
	//!
	std::vector<Arc> getActivationPlaces() const;

	//!
	//! \brief getDestinationPlaces
	//! \return A vector of Arcs with the destination places and their weights.
	//!
	std::vector<Arc> getDestinationPlaces() const;

	//!
	//! \brief getAdditionalActivationConditions
	//! \return
	//!
	std::vector<std::pair<std::string, ConditionFunction>> getAdditionalActivationConditions() const;

	//!
	//! \brief getInhibitorArcs
	//! \return
	//!
	std::vector<Arc> getInhibitorArcs() const;

	//!
	//! \brief requireNoActionsInExecution
	//! \return true if it is required that the activation places have no ongoing on enter actions.
	//!
	bool requireNoActionsInExecution() const;

	//!
	//! \brief getTransitionProperties
	//! \return
	//!
	TransitionProperties getTransitionProperties() const;

	//!
	//! \brief getArcsProperties
	//! \return
	//!
	std::array<std::vector<ArcProperties>,3> getArcsProperties() const;

	//!
	//! \brief addPlace
	//! \param place
	//! \param activation
	//! \param isInhibitor
	//! \param weight
	//!
	void addPlace(const std::shared_ptr<Place> &place, const ArcProperties::Type type, const size_t weight = 0);

	//!
	//! \brief removePlace
	//! \param place
	//! \param activation
	//! \param isInhibitor
	//!
	void removePlace(const std::shared_ptr<Place> &place, const ArcProperties::Type type);

private:
	//!
	//! \brief Name and identifier of the transition.
	//!
	std::string m_name;

	//!
	//! \brief Checks if all inhibitor places are empty.
	//! \return True if yes, false if not.
	//!
	bool checkInhibitorPlaces() const;

	//!
	//! \brief Checks if all activation places have enough tokens.
	//! \return True if yes, false if not.
	//!
	bool checkActivationPlaces() const;

	//!
	//! \brief Checks if all additional conditions allow firing.
	//! \return True if yes, false if not.
	//!
	bool checkAdditionalConditions() const;

	//!
	//! \brief If there are no actions being currently executed in the activation places.
	//! \return true if there are no actions being currently executed in the activation places.
	//!
	bool noActionsInExecution() const;

	//!
	//! \brief Evaluates if the transition can be fired.
	//! \return true if can be fired, false if it cannot.
	//!
	bool isActive() const;

	//!
	//! \brief Evaluates if the transition can attempt to be fired.
	//! \return True if can attepmted to be fired, false if it cannot.
	//!
	bool isEnabledInternal() const;

	//! Moves the tokens from the inputs to the outputs.
	void performTransit() const;

	//! Removes the tokens from the activation places.
	void exitActivationPlaces() const;

	//! Inserts tokens in the destination places.
	void enterDestinationPlaces() const;

	//! Move members of the class into the transition passed as argument.
	void moveMembers(Transition &transition);

	//! Block/unblock activation places from starting any on enter actions.
	void blockStartingOnEnterActions(const bool value) const;

	//! Pointers to the activations places from the net.
	std::vector<Arc> m_activationArcs;

	//! Pointers to the destination places from the net.
	std::vector<Arc> m_destinationArcs;

	//! Pointers to the controller's functions that evaluate if the transition can be fired.
	std::vector<std::pair<std::string, ConditionFunction>> m_additionalActivationConditions;

	//! Inhibitor arcs
	std::vector<Arc> m_inhibitorArcs;

	//! Shared mutex to synchronize calls, allowing simultaneous reads (readers-writer lock).
	mutable std::shared_mutex m_mutex;

	//! If on, the transition will only be activated if, besides all other oncditions,
	//! the activation places have no on enter actions in execution.
	bool m_requireNoActionsInExecution = false;
};
} // namespace ptne
