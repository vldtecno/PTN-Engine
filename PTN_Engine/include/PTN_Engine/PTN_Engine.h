/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2024 Eduardo Valgôde
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

#include "PTN_Engine/Utilities/Explicit.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ptne
{
using ConditionFunction = std::function<bool(void)>;
using ActionFunction = std::function<void(void)>;

/*!
 * \brief The PlaceProperties class
 */
struct DLL_PUBLIC ArcProperties final
{
	enum class Type {
		ACTIVATION,
		DESTINATION,
		BIDIRECTIONAL,
		INHIBITOR,
		//RESET,
	};

	/*!
	 * \brief weight
	 */
	size_t weight = 1;

	/*!
	 * \brief placeName
	 */
	std::string placeName;

	/*!
	 * \brief transitionName
	 */
	std::string transitionName;

	/*!
	 * \brief type
	 */
	Type type = { Type::ACTIVATION };
};

/*!
 * \brief The TransitionProperties class
 */
struct DLL_PUBLIC TransitionProperties final
{
	/*!
	 * \brief Name and identifier of the transition
	 */
	std::string name;

	//!
	//! \brief activationArcs
	//!
	std::vector<ArcProperties> activationArcs;

	//!
	//! \brief destinationArcs
	//!
	std::vector<ArcProperties> destinationArcs;

	//!
	//! \brief inhibitorArcs
	//!
	std::vector<ArcProperties> inhibitorArcs;

	//!
	//! \brief additional conditions function names
	//!
	std::vector<std::string> additionalConditionsNames;

	//!
	//! \brief A vector with functions that return bool.
	//!
	std::vector<ConditionFunction> additionalConditions;

	//!
	//! \brief requireNoActionsInExecution
	//!
	bool requireNoActionsInExecution = false;
};

/*!
 * \brief The PlaceProperties class
 */
struct DLL_PUBLIC PlaceProperties final
{
	//!
	//! \brief The name of the place.
	//!
	std::string name;

	//!
	//! \brief The number of tokens to be initialized with.
	//!
	size_t initialNumberOfTokens = 0;

	//!
	//! \brief The function to be called once a token enters the place.
	//!
	std::string onEnterActionFunctionName;

	//!
	//! \brief The function to be called once a token leaves the place.
	//!
	std::string onExitActionFunctionName;

	//!
	//! \brief onEnterAction
	//!
	ActionFunction onEnterAction = nullptr;

	//!
	//! \brief onExitAction
	//!
	ActionFunction onExitAction = nullptr;

	//!
	//! \brief A flag determining if this place can have tokens added manually.
	//!
	bool input = false;
};

//! Base class that implements the Petri net logic.
/*!
 * Base class that implements the Petri net logic.
 * Using the pimpl idiom.
 * \sa PTN_EngineImp
 */
class DLL_PUBLIC PTN_Engine
{
public:
	enum class ACTIONS_THREAD_OPTION
	{
		SINGLE_THREAD,
		EVENT_LOOP,
		DETACHED,
		JOB_QUEUE
	};

	/*!
	 * \brief toACTIONS_THREAD_OPTION
	 * \param actionsThreadOptionStr
	 * \return
	 */
	static ACTIONS_THREAD_OPTION toACTIONS_THREAD_OPTION(const std::string &actionsThreadOptionStr);

	/*!
	 * \brief toString
	 * \param actionsThreadOption
	 * \return
	 */
	static std::string toString(ACTIONS_THREAD_OPTION actionsThreadOption);

	using EventLoopSleepDuration = std::chrono::duration<long, std::ratio<1, 1000>>;

	virtual ~PTN_Engine();

	PTN_Engine(const PTN_Engine &) = delete;
	PTN_Engine(PTN_Engine &&) = delete;
	PTN_Engine &operator=(const PTN_Engine &) = delete;
	PTN_Engine &operator=(PTN_Engine &&) = delete;

	/*!
	 * \brief createTransition
	 * \param transitionProperties
	 */
	void createTransition(const TransitionProperties &transitionProperties);

	/*!
	 * \brief createPlace
	 * \param placeProperties
	 */
	void createPlace(const PlaceProperties &placeProperties);

	/*!
	 * Register an action to be called by the Petri net.
	 * \param name The name of the place.
	 * \param action The function to be called once a token enters the place.
	 */
	void registerAction(const std::string &name, const ActionFunction &action) const;

	/*!
	 * Register a condition
	 * \param name The name of the condition
	 * \param conditions A function pointer to a condition.
	 */
	void registerCondition(const std::string &name, const ConditionFunction &condition) const;

	/*!
	 * Start the petri net event loop.
	 * \param returnWhenStopped run the net only until no transition is fireable.
	 * \param log Flag logging the state of the net on or off.
	 * \param o Log output stream.
	 */
	void execute(const bool log = false, std::ostream &o = std::cout);

	/*!
	 * Return the number of tokens in a given place.
	 * \param place The name of the place to get the number of tokens from.
	 * \return The number of tokens present in the place.
	 */
	size_t getNumberOfTokens(const std::string &place) const;

	/*!
	 * Add a token to an input place.
	 * \param place Name of the place to be incremented.
	 */
	void incrementInputPlace(const std::string &place);

	/*!
	 * Print the petri net places and number of tokens.
	 * \param o Output stream.
	 */
	void printState(std::ostream &o) const;

	//! Constructor
	explicit PTN_Engine(ACTIONS_THREAD_OPTION actionsRuntimeThread = ACTIONS_THREAD_OPTION::JOB_QUEUE);

	//! Specify the thread where the actions should be run.
	void setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption);

	//! Get the information on which thread the actions are run.
	ACTIONS_THREAD_OPTION getActionsThreadOption() const;

	/*!
	 * \brief Whether the Petri Net's event loop is running or not.
	 * \return True if the event loop is running, false otherwise.
	 */
	bool isEventLoopRunning() const;

	/*!
	 * \brief Stop the execution loop.
	 */
	void stop();

	/*!
	 * \brief setEventLoopSleepDuration
	 * \param sleepDuration
	 */
	void setEventLoopSleepDuration(const EventLoopSleepDuration sleepDuration);

	/*!
	 * \brief getEventLoopSleepDuration
	 * \return
	 */
	EventLoopSleepDuration getEventLoopSleepDuration() const;

	/*!
	 * \brief addArc
	 * \param arcProperties
	 */
	void addArc(const ArcProperties& arcProperties);

	/*!
	 * \brief addArc
	 * \param arcProperties
	 */
	void removeArc(const ArcProperties& arcProperties);

	/*!
	 * \brief clearNet
	 */
	void clearNet();

	/*!
	 * \brief getPlacesProperties
	 * \return
	 */
	std::vector<PlaceProperties> getPlacesProperties() const;

	/*!
	 * \brief getTransitionsProperties
	 * \return
	 */
	std::vector<TransitionProperties> getTransitionsProperties() const;

private:
	class PTN_EngineImpProxy;

	//! Pointer to implementation of the PTN_Engine.
	std::unique_ptr<PTN_EngineImpProxy> m_impProxy;
};

} // namespace ptne
