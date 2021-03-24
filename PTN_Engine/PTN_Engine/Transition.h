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

#pragma once

#include "PTN_Engine/PTN_Exception.h"
#include <functional>
#include <memory>
#include <tuple>
#include <vector>

namespace ptne
{
class Place;

//! Shared pointer to member functions of the controller.
using ConditionFunction = std::function<bool(void)>;

//! Implements a Petri net transition.
/*!
 * Implements a Petri net transition.
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
			   const std::vector<std::pair<std::string, ConditionFunction>> &additionalActivationConditions);

	/*!
	 * Evaluate the activation places and transit the tokens if possible.
	 * \return True if token transit was performed, false if not.
	 */
	bool execute();

	/*!
	 * Evaluates if the transition can be fired.
	 * \return True if can be fired, false if it cannot.
	 */
	bool isActive() const;

    /*!
     * Evaluates if the transition can attempt to be fired.
     * \return True if can attepmted to be fired, false if it cannot.
     */
    bool isEnabled() const;

	std::vector<std::tuple<WeakPtrPlace, size_t>> getActivationPlaces() const;

	std::vector<std::tuple<WeakPtrPlace, size_t>> getDestinationPlaces() const;

	std::vector<std::pair<std::string, ConditionFunction>> getAdditionalActivationConditions() const;

	std::vector<WeakPtrPlace> getInhibitorPlaces() const;

	/*!
	 * Exception to be thrown when the dimension of the vector containing the activation weights does not match
	 * the dimension of the vector containing the names of the activation places.
	 */
	class ActivationWeightDimensionException : public PTN_Exception
	{
	public:
		ActivationWeightDimensionException();
	};

	/*!
	 * Exception to be thrown when the dimension of the vector containing the destination weights does not match
	 * the dimension of the vector containing the names of the destination places.
	 */
	class DestinationWeightDimensionException : public PTN_Exception
	{
	public:
		DestinationWeightDimensionException();
	};

	//! Exception to be thrown when the weight value is 0.
	class ZeroValueWeightException : public PTN_Exception
	{
	public:
		ZeroValueWeightException();
	};

	/*!
	 * Exception to be thrown when activation places in the constructor are repeated.
	 */
	class ActivationPlaceRepetitionException : public PTN_Exception
	{
	public:
		ActivationPlaceRepetitionException();
	};

	/*!
	 * Exception to be thrown when activation places in the constructor are repeated.
	 */
	class DestinationPlaceRepetitionException : public PTN_Exception
	{
	public:
		DestinationPlaceRepetitionException();
	};

private:
	/*!
	 * Checks if all inhibitor places are empty.
	 * \return True if yes, false if not.
	 */
	bool checkInhibitorPlaces() const;

	/*!
	 * Checks if all activation places have enough tokens.
	 * \return True if yes, false if not.
	 */
	bool checkActivationPlaces() const;

	/*!
	 * Checks if all additional conditions allow firing.
	 * \return True if yes, false if not.
	 */
	bool checkAdditionalConditions() const;

	//! Moves the tokens from the inputs to the outputs.
	void performTransit();

	//! Removes the tokens from the activation places.
	void exitActivationPlaces();

	//! Inserts tokens in the destination places.
	void enterDestinationPlaces();

	//! Pointers to the activations places from the net.
	std::vector<std::tuple<WeakPtrPlace, size_t>> m_activationPlaces;

	//! Pointers to the destination places from the net.
	std::vector<std::tuple<WeakPtrPlace, size_t>> m_destinationPlaces;

	//! Pointers to the controller's functions that evaluate if the transition can be fired.
	std::vector<std::pair<std::string, ConditionFunction>> m_additionalActivationConditions;

	//! Inhibitor arcs
	std::vector<WeakPtrPlace> m_inhibitorPlaces;
};
} // namespace ptne
