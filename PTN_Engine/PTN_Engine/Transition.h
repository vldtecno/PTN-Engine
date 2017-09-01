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

#include <memory>
#include <vector>

namespace ptne
{
	class IConditionFunctor;
	class Place;

	//! Shared pointer to member functions of the controller.
	using ConditionFunctorPtr = std::shared_ptr<IConditionFunctor>;

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
		 * \param destinationPlaces Collection of outgoing places.
		 * \param additionalActivationConditions Boolean function from the controller that can block firing.
		 */
		Transition(const std::vector<WeakPtrPlace>& activationPlaces,
			const std::vector<WeakPtrPlace>& destinationPlaces,
			const std::vector<ConditionFunctorPtr>& additionalActivationConditions);

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

	private:

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
		std::vector<WeakPtrPlace> m_activationPlaces;

		//! Pointers to the destination places from the net.
		std::vector<WeakPtrPlace> m_destinationPlaces;

		//! Pointers to the controller's functions that evaluate if the transition can be fired.
		std::vector<ConditionFunctorPtr> m_additionalActivationConditions;

	};
}


