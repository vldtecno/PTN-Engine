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

#include <vector>
#include <map>
#include <memory>
#include "PTN_Engine/PTN_Exception.h"

namespace ptne
{
	class Place;
	class Transition;
	class IConditionFunctor;
	class IActionFunctor;

	using ConditionFunctorPtr = std::shared_ptr<IConditionFunctor>;
	using ActionFunctorPtr = std::shared_ptr<IActionFunctor>;
	using SharedPtrPlace = std::shared_ptr<Place>;
	using WeakPtrPlace = std::weak_ptr<Place>;

	//! Implements the Petri net logic.
	/*!
	 * Implements the Petri net logic.
	 * Used by the PTN_Engine class.
	 * \sa PTN_Engine
	 */
	class PTN_EngineImp final
	{
	public:

		//! Constructor.
		PTN_EngineImp();

		virtual ~PTN_EngineImp();

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param activationWeights A vector with the weights of each activation place.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param destinationWeights A vector with the weights of each destination place.
		 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
		 * \param additionalConditions A vector with functors that return bool.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<size_t>& activationWeights,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<size_t>& destinationWeights,
			const std::vector<std::string>& inhibitorPlaces,
			const std::vector<ConditionFunctorPtr>& additionalConditions
		);

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param activationWeights A vector with the weights of each activation place.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param destinationWeights A vector with the weights of each destination place.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<size_t>& activationWeights,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<size_t>& destinationWeights);

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param activationWeights A vector with the weights of each activation place.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param destinationWeights A vector with the weights of each destination place.
		 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<size_t>& activationWeights,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<size_t>& destinationWeights,
			const std::vector<std::string>& inhibitorPlaces);

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param activationWeights A vector with the weights of each activation place.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param destinationWeights A vector with the weights of each destination place.
		 * \param additionalConditions A vector with functors that return bool.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<size_t>& activationWeights,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<size_t>& destinationWeights,
			const std::vector<ConditionFunctorPtr>& additionalConditions);

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param destinationPlaces A vector with the names of the destination places.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<std::string>& destinationPlaces);


		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<std::string>& inhibitorPlaces);

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param additionalConditions A vector with functors that return bool.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<ConditionFunctorPtr>& additionalConditions);

		/*!
		 * Create a new transition
		 * \param activationPlaces A vector with the names of the activation places.
		 * \param destinationPlaces A vector with the names of the destination places.
		 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
		 * \param additionalConditions A vector with functors that return bool.
		 */
		void createTransition(
			const std::vector<std::string>& activationPlaces,
			const std::vector<std::string>& destinationPlaces,
			const std::vector<std::string>& inhibitorPlaces,
			const std::vector<ConditionFunctorPtr>& additionalConditions);

		//! Run until it no more transitions can be fired or stop is flagged.
		void execute();

		/*!
		 * Add place a place to the net.
		 * \param name The name of the place.
		 * \param initialNumberOfTokens The number of tokens to be initialized with.
		 * \param onEnterAction The functor to be called once a token enters the place.
		 * \param onExitAction The functor to be called once a token leaves the place.
		 * \param input A flag determining if this place can have tokens added manually.
		 */
		void addPlace(
				const std::string& name,
				const size_t initialNumberOfTokens,
				ActionFunctorPtr onEnterAction,
				ActionFunctorPtr onExitAction,
				const bool input = false);

		/*!
		 * Return the number of tokens in a given place.
		 * \param place The name of the place to get the number of tokens from.
		 * \return The number of tokens present in the place.
		 */
		size_t getNumberOfTokens(const std::string& place) const;

		/*!
		 * Add a token in an input place.
		 * \param place Name of the place to be incremented.
		 */
		void incrementInputPlace(const std::string& place);

		/*!
		 * Exception to be thrown when trying to use a place with
		 * a wrong name.
		 */
		class InvalidNameException: public PTN_Exception
		{
		public:
			InvalidNameException(const std::string& name);
		};

		/*!
		 * Exception to be thrown if names of places to construct
		 * a transition are repeated.
		 */
		class RepeatedPlaceNamesException: public PTN_Exception
		{
		public:
			RepeatedPlaceNamesException();
		};

		/*!
		 * Exception to be thrown if the user tries to increment the
		 * number of tokens of a non input place.
		 */
		class NotInputPlaceException: public PTN_Exception
		{
		public:
			NotInputPlaceException(const std::string& name);
		};

		/*!
		 * Exception to be thrown if the user tries to add a Place
		 * to the net that with the name of an already existing one.
		 */
		class RepeatedPlaceException: public PTN_Exception
		{
		public:
			RepeatedPlaceException(const std::string& name);
		};

	private:

		/*!
		 * Clear the token counter from all input places.
		 */
		void clearInputPlaces();

		/*!
		 * Collects and randomizes the order of all active transitions.
		 * \return Vector of unique pointers to active transitions.
		 */
		std::vector<Transition*> collectActiveTransitionsRandomly();

		//! Vector with the transitions.
		/*!
		 * Insertions on construction. Otherwise (should remain) unchanged.
		 */
		std::vector<Transition> m_transitions;

		//! Vector with the input places.
		/*!
		 * Insertions on construction. Otherwise (should remain) unchanged.
		 */
		std::vector<WeakPtrPlace> m_inputPlaces;

		//! Map of places.
		/*!
		 * Insertion in only performed on construction and no removals
		 * during the lifetime of the net. Otherwise there are many
		 * access operations.
		 */
		std::map<std::string, SharedPtrPlace> m_places;

		//! Translates a vector of names of places to a vector of weak pointers to those places.
		/*!
		 * \param names Names of the places.
		 * \return Vector of weakt_ptr for each place given in "names".
		 */
		std::vector<WeakPtrPlace> getPlacesFromNames(const std::vector<std::string>& names) const;

		//! Flag to stop the execution of the net.
		/*!
		 * For future use in multi-threaded operation.
		 * Will probably become atomic.
		 */
		bool m_stop;
	};		


}
