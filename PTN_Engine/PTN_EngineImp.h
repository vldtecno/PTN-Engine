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

#include "PTN_Engine/EventLoop.h"
#include "PTN_Engine/IPTN_EngineEL.h"
#include "PTN_Engine/ManagedContainer.h"
#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/PlacesManager.h"
#include "PTN_Engine/TransitionsManager.h"
#include <atomic>
#include <shared_mutex>

namespace ptne
{

class IActionFunctor;
class IConditionFunctor;
class IActionsExecutor;
class JobQueue;
class Place;
class Transition;

using SharedPtrPlace = std::shared_ptr<Place>;
using WeakPtrPlace = std::weak_ptr<Place>;


//! Implements the Petri net logic.
class PTN_EngineImp final : public IPTN_EngineEL
{
public:
	~PTN_EngineImp() override;
	explicit PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption);
	PTN_EngineImp(const PTN_EngineImp &) = delete;
	PTN_EngineImp(PTN_EngineImp &&) = delete;
	PTN_EngineImp &operator=(const PTN_EngineImp &) = delete;
	PTN_EngineImp &operator=(PTN_EngineImp &&) = delete;

	PTN_Engine::ACTIONS_THREAD_OPTION getActionsThreadOption() const override;

	//!
	//! \brief Indicates if there are new tokens in any input places.
	//! \return True of there is a new token in an input place.
	//!
	bool getNewInputReceived() const override;

	void addArc(const ArcProperties &arcProperties) const;

	//!
	//! Clear the token counter from all input places.
	//!
	void clearInputPlaces();

	void clearNet();

	void createPlace(PlaceProperties placeProperties);

	void createTransition(const TransitionProperties &transitionProperties);

	//!
	//! \brief Gets the transitions that are currently enabled.
	//! \return Weak pointers to the transitions that are enabled.
	//!
	std::vector<std::weak_ptr<Transition>> enabledTransitions() const;

	//!
	//! Start the petri net event loop.
	//! \param log Flag logging the state of the net on or off.
	//! \param o Log output stream.
	//!
	void execute(const bool log = false, std::ostream &o = std::cout);

	//!
	//! \brief Gets the current sleep time set in the event loop.
	//! \return The sleep time of the event loop.
	//!
	PTN_Engine::EventLoopSleepDuration getEventLoopSleepDuration() const;

	//!
	//! Return the number of tokens in a given place.
	//! \param place The name of the place to get the number of tokens from.
	//! \return The number of tokens present in the place.
	//!
	size_t getNumberOfTokens(const std::string &place) const;

	std::vector<PlaceProperties> getPlacesProperties() const;

	std::vector<TransitionProperties> getTransitionsProperties() const;

	//!
	//! Add a token in an input place.
	//! \param place Name of the place to be incremented.
	//!
	void incrementInputPlace(const std::string &place);

	bool isEventLoopRunning() const;

	//!
	//! Print the petri net places and number of tokens.
	//! \param o Output stream.
	//!
	void printState(std::ostream &o) const;

	//!
	//! Register an action to be called by the Petri net.
	//! \param name The name of the place.
	//! \param action The function to be called once a token enters the place.
	//!
	void registerAction(const std::string &name, const ActionFunction &action);

	//!
	//! Register a condition
	//! \param name The name of the condition
	//! \param conditions A function pointer to a condition.
	//!
	void registerCondition(const std::string &name, const ConditionFunction &condition);

	void removeArc(const ArcProperties &arcProperties) const;

	//! Specify the thread where the actions should be run.
	void setActionsThreadOption(const PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption);

	//!
	//! \brief Set the sleep duration of the event loop.
	//! \param sleepDuration - Time the event loop takes until it checks for new inputs.
	//!
	void setEventLoopSleepDuration(const PTN_Engine::EventLoopSleepDuration sleepDuration);

	//!
	//! \brief Stop the execution of the petri net.
	//!
	void stop() noexcept;

private:
	//!
	//! \brief Execute the Petri net.
	//! \param log
	//! \param o
	//! \return
	//!
	bool executeInt(const bool log = false, std::ostream &o = std::cout) override;

	//!
	//! \brief createAnonymousConditions - Create activation conditions without proiding a name.
	//! \param conditions
	//! \return
	//!
	std::vector<std::pair<std::string, ConditionFunction>>
	createAnonymousConditions(const std::vector<ConditionFunction> &conditions) const;

	//!
	//! \brief Create a new transition in the petri net.
	//! \param name - name of the transition
	//! \param activationArcs
	//! \param destinationArcs
	//! \param inhibitorArcs
	//! \param additionalConditions - boolean functions that provide additional conditions, necessary to fire a
	//! transition. \param requireNoActionsInExecution - flag that determines if the on enter actions of each
	//! activation place, must have finished before fireing the transition.
	//!
	void createTransition(const std::string &name,
						  const std::vector<ArcProperties> &activationArcs,
						  const std::vector<ArcProperties> &destinationArcs,
						  const std::vector<ArcProperties> &inhibitorArcs,
						  const std::vector<std::pair<std::string, ConditionFunction>> &additionalConditions,
						  const bool requireNoActionsInExecution);

	//!
	//! \brief Flags or clears flag of new tokens in input places.
	//! \param newInputReceived - The new value for the new input received flag.
	//!
	void setNewInputReceived(const bool newInputReceived);

	//! Container with all the actions available to this Petri net.
	ManagedContainer<ActionFunction> m_actions;

	//! Executes the actions associated to each place, when tokens enter or exit them.z
	std::shared_ptr<IActionsExecutor> m_actionsExecutor;

	//! Determines how the actions will be executed.
	PTN_Engine::ACTIONS_THREAD_OPTION m_actionsThreadOption;

	//! Mutex to synchronize m_actionsThreadOption.
	mutable std::shared_mutex m_actionsThreadOptionMutex;

	//! Conditions that can be used by the Petri net.
	ManagedContainer<ConditionFunction> m_conditions;

	//! Loop that processes events and executes the Petri net.
	EventLoop m_eventLoop;

	//! Flag reporting a new input event.
	std::atomic<bool> m_newInputReceived = false;

	PlacesManager m_places;

	TransitionsManager m_transitions;
};

} // namespace ptne
