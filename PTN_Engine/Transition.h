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

#pragma once

#include <functional>
#include <memory>
#include <shared_mutex>
#include <tuple>
#include <vector>

namespace ptne
{
class IExporter;
class Place;

//! Shared pointer to member functions of the controller.
using ConditionFunction = std::function<bool(void)>;

/*!
 * \brief Implements a Petri net transition.
 */
class Transition final
{
public:
	//! Shared pointer type to a place in the net.
	using SharedPtrPlace = std::shared_ptr<Place>;

	//! Weak pointer type to a place in the net.
	using WeakPtrPlace = std::weak_ptr<Place>;

	//! Constructor.
	/*!
	 * Constructor.
	 * \param activationPlaces Collection of incoming places.
	 * \param activationWeights Weights of each activation place.
	 * \param destinationPlaces Collection of outgoing places.
	 * \param destinationWeights Weights of each destination place.
	 * \param additionalActivationConditions Boolean function from the controller that can block firing.
	 * \param inhibitorPlaces Places that must be empty for the transition to fire.
	 */
	Transition(const std::vector<WeakPtrPlace> &activationPlaces,
			   const std::vector<size_t> &activationWeights,
			   const std::vector<WeakPtrPlace> &destinationPlaces,
			   const std::vector<size_t> &destinationWeights,
			   const std::vector<WeakPtrPlace> &inhibitorPlaces,
			   const std::vector<std::pair<std::string, ConditionFunction>> &additionalActivationConditions,
	           const bool requireNoActionsInExecution);

	~Transition() = default;
	Transition(const Transition &) = delete;
	Transition(Transition &&transition) noexcept;
	Transition &operator=(Transition &) = delete;
	Transition &operator=(Transition &&) = delete;

	/*!
	 * Evaluate the activation places and transit the tokens if possible.
	 * \return true if token transit was performed, false if not.
	 */
	bool execute();

	/*!
	 * Evaluates if the transition can attempt to be fired.
	 * \return true if can attepmted to be fired, false if it cannot.
	 */
	bool isEnabled() const;

	/*!
	 * \brief getActivationPlaces Gets the activation places of the transition.
	 * \return A vector of tuples with the activation places and their weights.
	 */
	std::vector<std::tuple<WeakPtrPlace, size_t>> getActivationPlaces() const;

	/*!
	 * \brief getDestinationPlaces
	 * \return A vector of tuples with the destination places and their weights.
	 */
	std::vector<std::tuple<WeakPtrPlace, size_t>> getDestinationPlaces() const;

	/*!
	 * \brief getAdditionalActivationConditions
	 * \return
	 */
	std::vector<std::pair<std::string, ConditionFunction>> getAdditionalActivationConditions() const;

	/*!
	 * \brief getInhibitorPlaces
	 * \return
	 */
	std::vector<WeakPtrPlace> getInhibitorPlaces() const;

	/*!
	 * \brief Export a transition.
	 * \param exporter - Object that can export a transition.
	 */
	void export_(IExporter &exporter) const;

	/*!
	 * \brief requireNoActionsInExecution
	 * \return true if it is required that the activation places have no ongoing on enter actions.
	 */
	bool requireNoActionsInExecution() const;

private:
	/*!
	 * \brief Checks if all inhibitor places are empty.
	 * \return True if yes, false if not.
	 */
	bool checkInhibitorPlaces() const;

	/*!
	 * \brief Checks if all activation places have enough tokens.
	 * \return True if yes, false if not.
	 */
	bool checkActivationPlaces() const;

	/*!
	 * \brief Checks if all additional conditions allow firing.
	 * \return True if yes, false if not.
	 */
	bool checkAdditionalConditions() const;

	/*!
	 * \brief If there are no actions being currently executed in the activation places.
	 * \return true if there are no actions being currently executed in the activation places.
	 */
	bool noActionsInExecution() const;

	/*!
	 * \brief Evaluates if the transition can be fired.
	 * \return true if can be fired, false if it cannot.
	 */
	bool isActive() const;

	/*!
	 * \brief Evaluates if the transition can attempt to be fired.
	 * \return True if can attepmted to be fired, false if it cannot.
	 */
	bool isEnabledInternal() const;

	//! Moves the tokens from the inputs to the outputs.
	void performTransit();

	//! Removes the tokens from the activation places.
	void exitActivationPlaces();

	//! Inserts tokens in the destination places.
	void enterDestinationPlaces();

	//! Move members of the class into the transition passed as argument.
	void moveMembers(Transition &transition);

	//! Block/unblock activation places from starting any on enter actions.
	void blockStartingOnEnterActions(const bool value);

	//! Pointers to the activations places from the net.
	std::vector<std::tuple<WeakPtrPlace, size_t>> m_activationPlaces;

	//! Pointers to the destination places from the net.
	std::vector<std::tuple<WeakPtrPlace, size_t>> m_destinationPlaces;

	//! Pointers to the controller's functions that evaluate if the transition can be fired.
	std::vector<std::pair<std::string, ConditionFunction>> m_additionalActivationConditions;

	//! Inhibitor arcs
	std::vector<WeakPtrPlace> m_inhibitorPlaces;

	//! Shared mutex to synchronize calls, allowing simultaneous reads (readers-writer lock).
	mutable std::shared_mutex m_mutex;

	//! If on, the transition will only be activated if, besides all other oncditions,
	//! the activation places have no on enter actions in execution.
	bool m_requireNoActionsInExecution = false;
};
} // namespace ptne
