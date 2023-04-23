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

#include "PTN_Engine/EventLoop.h"
#include "PTN_Engine/IPTN_EngineEL.h"
#include "PTN_Engine/IPTN_EnginePlace.h"
#include "PTN_Engine/ManagedContainer.h"
#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/PlacesManagedContainer.h"
#include "PTN_Engine/TransitionsManagedContainer.h"
#include <atomic>
#include <shared_mutex>
#include <thread>

namespace ptne
{
class IConditionFunctor;
class IActionFunctor;
class IExporter;
class IImporter;
class JobQueue;
class Place;
class Transition;

using SharedPtrPlace = std::shared_ptr<Place>;
using WeakPtrPlace = std::weak_ptr<Place>;


//! Implements the Petri net logic.
/*!
 * Implements the Petri net logic.
 * Used by the PTN_Engine class.
 * \sa PTN_Engine
 */
class PTN_EngineImp final : public IPTN_EngineEL, public IPTN_EnginePlace
{
public:
	//! Constructor.
	PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION actionsRuntimeThread);

	~PTN_EngineImp();
	PTN_EngineImp(const PTN_EngineImp &) = delete;
	PTN_EngineImp(PTN_EngineImp &&) = delete;
	PTN_EngineImp &operator=(const PTN_EngineImp &) = delete;
	PTN_EngineImp &operator=(PTN_EngineImp &&) = delete;

	/*!
	 * Clear the token counter from all input places.
	 */
	void clearInputPlaces();

	/*!
	 * \brief clearNet
	 */
	void clearNet();

	/*!
	 * Create a new place in the net.
	 * \param name The name of the place.
	 * \param initialNumberOfTokens The number of tokens to be initialized with.
	 * \param onEnterAction The function to be called once a token enters the place.
	 * \param onExitAction The function to be called once a token leaves the place.
	 * \param input A flag determining if this place can have tokens added manually.
	 */
	void createPlace(const std::string &name,
					 const size_t initialNumberOfTokens,
					 ActionFunction onEnterAction,
					 ActionFunction onExitAction,
					 const bool input = false);

	/*!
	 * Create a new place in the net.
	 * \param name The name of the place.
	 * \param initialNumberOfTokens The number of tokens to be initialized with.
	 * \param onEnterAction Name of the function to be called once a token enters the place.
	 * \param onExitAction Name of the function to be called once a token leaves the place.
	 * \param input A flag determining if this place can have tokens added manually.
	 */
	void createPlaceStr(const std::string &name,
						const size_t initialNumberOfTokens,
						const std::string &onEnterAction,
						const std::string &onExitAction,
						const bool input = false);

	/*!
	 * Create a new transition
	 * \param activationPlaces A vector with the names of the activation places.
	 * \param activationWeights A vector with the weights of each activation place.
	 * \param destinationPlaces A vector with the names of the destination places.
	 * \param destinationWeights A vector with the weights of each destination place.
	 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
	 * \param additionalConditions A vector with additionalConditions.
	 */
	void createTransition(const std::vector<std::string> &activationPlaces,
						  const std::vector<size_t> &acPTN_EngineInternaltivationWeights,
						  const std::vector<std::string> &destinationPlaces,
						  const std::vector<size_t> &destinationWeights,
						  const std::vector<std::string> &inhibitorPlaces,
						  const std::vector<ConditionFunction> &additionalConditions,
						  const bool requireNoActionsInExecution);

	/*!
	 * Create a new transition
	 * \param activationPlaces A vector with the names of the activation places.
	 * \param activationWeights A vector with the weights of each activation place.
	 * \param destinationPlaces A vector with the names of the destination places.
	 * \param destinationWeights A vector with the weights of each destination place.
	 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
	 * \param additionalConditions A vector with a pairs of a name and a function that returns bool.
	 */
	void createTransition(const std::vector<std::string> &activationPlaces,
						  const std::vector<size_t> &activationWeights,
						  const std::vector<std::string> &destinationPlaces,
						  const std::vector<size_t> &destinationWeights,
						  const std::vector<std::string> &inhibitorPlaces,
						  const std::vector<std::string> &additionalConditions,
						  const bool requireNoActionsInExecution);

	/*!
	 * \brief Imports the Petri net using the provided importer.
	 * \param importer Object containing all necessary information to create a new Petri net.
	 */
	bool isEventLoopRunning() const;

	/*!
	 * \brief Stop the execution loop.
	 */
	void stop() noexcept;

	/*!
	 * Register an action to be called by the Petri net.
	 * \param name The name of the place.
	 * \param action The function to be called once a token enters the place.
	 */
	void registerAction(const std::string &name, ActionFunction action);

	/*!
	 * Register a condition
	 * \param name The name of the condition
	 * \param conditions A function pointer to a condition.
	 */
	void registerCondition(const std::string &name, ConditionFunction condition);

	/*!
	 * Return the number of tokens in a given place.
	 * \param place The name of the place to get the number of tokens from.
	 * \return The number of tokens present in the place.
	 */
	size_t getNumberOfTokens(const std::string &place) const;

	/*!
	 * Add a token in an input place.
	 * \param place Name of the place to be incremented.
	 */
	void incrementInputPlace(const std::string &place);

	/*!
	 * \brief export_
	 * \param exporter
	 */
	void export_(IExporter &exporter) const;

	/*!
	 * \brief import
	 * \param importer
	 */
	void import(const IImporter &importer);

	//! Specify the thread where the actions should be run.
	void setActionsThreadOption(const PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption);

	/*!
	 * \brief getActionsThreadOption
	 * \return
	 */
	PTN_Engine::ACTIONS_THREAD_OPTION getActionsThreadOption() const;

	/*!
	 * Start the petri net event loop.
	 * \param log Flag logging the state of the net on or off.
	 * \param o Log output stream.
	 */
	void execute(const bool log = false, std::ostream &o = std::cout);

	/*!
	 * \brief executeInt
	 * \param log
	 * \param o
	 * \return
	 */
	bool executeInt(const bool log = false, std::ostream &o = std::cout);

	/*!
	 * \brief Add job to job queue.
	 * \param Function to be executed in the job queue.
	 */
	void addJob(const ActionFunction &actionFunction);

	/*!
	 * \brief Indicates if there are new tokens in any input places.
	 * \return True of there is a new token in an input place.
	 */
	bool getNewInputReceived() const;

	/*!
	 * \brief Flags or clears flag of new tokens in input places.
	 * \param flag - The new value for the new input received flag.
	 */
	void setNewInputReceived(const bool flag);

	/*!
	 * \brief Gets the transitions that are currently enabled.
	 * \return Weak pointers to the transitions that are enabled.
	 */
	std::vector<std::weak_ptr<Transition>> enabledTransitions() const;

	/*!
	 * Print the petri net places and number of tokens.
	 * \param o Output stream.
	 */
	void printState(std::ostream &o) const;

	/*!
	 * \brief Set the sleep duration of the event loop.
	 * \param sleepDuration - Time the event loop takes until it checks for new inputs.
	 */
	void setEventLoopSleepDuration(const PTN_Engine::EventLoopSleepDuration sleepDuration);

	/*!
	 * \brief Gets the current sleep time set in the event loop.
	 * \return The sleep time of the event loop.
	 */
	PTN_Engine::EventLoopSleepDuration getEventLoopSleepDuration() const;

private:
	/*!
	 * Create a new transition
	 * \param activationPlaces A vector with the names of the activation places.
	 * \param activationWeights A vector with the weights of each activation place.
	 * \param destinationPlaces A vector with the names of the destination places.
	 * \param destinationWeights A vector with the weights of each destination place.
	 * \param inhibitorPlaces Places that cannot have tokens to fire the transition.
	 * \param additionalConditions A vector with names to additional conditions.
	 */
	void createTransition(const std::vector<std::string> &activationPlaces,
						  const std::vector<size_t> &activationWeights,
						  const std::vector<std::string> &destinationPlaces,
						  const std::vector<size_t> &destinationWeights,
						  const std::vector<std::string> &inhibitorPlaces,
						  const std::vector<std::pair<std::string, ConditionFunction>> &additionalConditions,
						  const bool requireNoActionsInExecution);

	/*!
	 * \brief exportPlaces
	 * \param exporter
	 */
	void exportPlaces(IExporter &exporter) const;

	/*!
	 * \brief createAnonymousConditions
	 * \param conditions
	 * \return
	 */
	const std::vector<std::pair<std::string, ConditionFunction>>
	createAnonymousConditions(const std::vector<ConditionFunction> &conditions) const;

	/*!
	 * \brief ACTIONS_THREAD_OPTION_fromString
	 * \param actionsThreadOptionStr
	 * \param out - Output value
	 * \return true if successful.
	 */
	static bool ACTIONS_THREAD_OPTION_fromString(const std::string &actionsThreadOptionStr,
												 PTN_Engine::ACTIONS_THREAD_OPTION &out);

	/*!
	 * \brief ACTIONS_THREAD_OPTION_toString
	 * \param actionsThreadOption
	 * \param out - Output value
	 * \return true if successful
	 */
	static bool
	ACTIONS_THREAD_OPTION_toString(const PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption, std::string &out);

	//! Container with all the transitions in the Petri net.
	TransitionsManagedContainer m_transitions;

	//! Container with all the places in the Petri net.
	PlacesManagedContainer m_places;

	//! Container with all the actions available to this Petri net.
	ManagedContainer<ActionFunction> m_actions;

	//! Conditions that can be used by the Petri net.
	ManagedContainer<ConditionFunction> m_conditions;

	//! Flag reporting a new input event.
	std::atomic<bool> m_newInputReceived = false;

	//! Mutex to synchronize m_actionsThreadOption.
	mutable std::shared_mutex m_actionsThreadOptionMutex;

	//! Determines how the actions will be executed.
	PTN_Engine::ACTIONS_THREAD_OPTION m_actionsThreadOption;

	//! Job queue to dispatch actions.
	std::unique_ptr<JobQueue> m_jobQueue;

	//! Loop that processes events and executes the Petri net.
	EventLoop m_eventLoop;
};

} // namespace ptne
