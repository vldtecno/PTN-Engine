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

using ConditionFunction = std::function<bool(void)>;
using SharedPtrPlace = std::shared_ptr<Place>;
using WeakPtrPlace = std::weak_ptr<Place>;

struct Arc
{
	WeakPtrPlace place;
	size_t weight = 1;
};

//! \brief Implements a Petri net transition.
class Transition final
{
public:
	~Transition() = default;

	//!
	//! \brief Transition constructor.
	//! \param name - name of the transition.
	//! \param activationArcs - vector of activation arcs.
	//! \param destinationArcs - vector of destination arcs.
	//! \param inhibitorArcs - vector of inhibitor arcs.
	//! \param additionalActivationConditions - vector of additional conditions
	//! \param requireNoActionsInExecution - flag if the transition requires no onEnter actions in execution in
	//! order to fire.
	//!
	Transition(const std::string &name,
			   const std::vector<Arc> &activationArcs,
			   const std::vector<Arc> &destinationArcs,
			   const std::vector<Arc> &inhibitorArcs,
			   const std::vector<std::pair<std::string, ConditionFunction>> &additionalActivationConditions,
			   const bool requireNoActionsInExecution);

	Transition(const Transition &) = delete;
	Transition(Transition &&transition) noexcept;
	Transition &operator=(Transition &) = delete;
	Transition &operator=(Transition &&) = delete;

	void addArc(const std::shared_ptr<Place> &place, const ArcProperties::Type type, const size_t weight = 0);

	//!
	//! Evaluate the activation places and transit the tokens if possible.
	//! \return true if token transit was performed, false if not.
	//!
	bool execute();

	std::vector<Arc> getActivationArcs() const;

	//!
	//! \brief Gets all arc properties for each arc in the transition.
	//! \return Array size 3 with vectors of arcs for Activation, Destination, Inhibitor
	//!
	std::array<std::vector<ArcProperties>, 3> getArcsProperties() const;

	//!
	//! Evaluates if the transition can attempt to be fired.
	//! \return true if can attepmt to be fire the transition.
	//!
	bool isEnabled() const;

	//!
	//! \brief Get all additional activation conditions.
	//! \return A vector of function name, ConditionFunction pairs.
	//!
	std::vector<std::pair<std::string, ConditionFunction>> getAdditionalActivationConditions() const;

	std::vector<Arc> getDestinationArcs() const;

	std::vector<Arc> getInhibitorArcs() const;

	std::string getName() const;

	//!
	//! \brief Describe all the transition's internals in a TransitionProperties object.
	//! \return TransitionProperties object with all properties of the transition.
	//!
	TransitionProperties getTransitionProperties() const;

	//!
	//! \brief Remove the arc from the transition.
	//! \param place - place pointing to or from the transition.
	//! \param type - the type of arc.
	//!
	void removeArc(const std::shared_ptr<Place> &place, const ArcProperties::Type type);

	//!
	//! \brief requireNoActionsInExecution
	//! \return true if it is required that the activation places have no ongoing on enter actions.
	//!
	bool requireNoActionsInExecution() const;

private:
	//! Block/unblock activation places from starting any on enter actions.
	void blockStartingOnEnterActions(const bool value) const;

	//!
	//! \brief Checks if all inhibitor places are empty.
	//! \return True if yes, false if not.
	//!
	bool checkInhibitorPlaces() const;

	//!
	//! \brief Checks if all activation places have enough tokens to fire a transition.
	//! \return True ifif all activation places have enough tokens to fire a transition.
	//!
	bool checkActivationPlaces() const;

	//!
	//! \brief Checks if all additional conditions allow firing the transition.
	//! \return True if all additional conditions are true.
	//!
	bool checkAdditionalConditions() const;

	//! Inserts tokens in the destination places.
	void enterDestinationPlaces() const;

	//! Removes the tokens from the activation places.
	void exitActivationPlaces() const;

	//!
	//! \brief Evaluates if the transition can be fired.
	//! \return true if can be fired, false if it cannot.
	//!
	bool isActive() const;

	//!
	//! \brief Evaluates if the transition can be fired.
	//! \return true if the transition can be fired, false if it cannot.
	//!
	bool isEnabledInternal() const;

	//! Move members of the class into the transition passed as argument.
	void moveMembers(Transition &transition);

	//!
	//! \brief If there are no actions being currently executed in the activation places.
	//! \return true if there are no actions being currently executed in the activation places.
	//!
	bool noActionsInExecution() const;

	//! Moves the tokens from the inputs to the outputs.
	void performTransit() const;

	std::vector<Arc> m_activationArcs;

	//! Pointers to the controller's functions that evaluate if the transition can be fired.
	std::vector<std::pair<std::string, ConditionFunction>> m_additionalActivationConditions;

	std::vector<Arc> m_destinationArcs;

	std::vector<Arc> m_inhibitorArcs;

	//! Shared mutex to synchronize calls, allowing simultaneous reads (readers-writer lock).
	mutable std::shared_mutex m_mutex;

	//! \brief Name that identifies the transition.
	std::string m_name;

	//! If on, the transition will only be activated if, besides all other conditions,
	//! the activation places have no on enter actions in execution.
	bool m_requireNoActionsInExecution = false;
};

} // namespace ptne
