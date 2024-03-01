/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
 * Copyright (c) 2023-2024 Eduardo Valgôde
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
#include "PTN_Engine/PTN_EngineImp.h"

namespace ptne
{

//!
//! \brief The PTN_Engine::PTN_EngineImpProxy class is a proxy class to the PTN_EngineImp, which implements the
//! PTN_Engine logic. This proxy, implements the necessary synchronization for multi-threaded usage of the
//! PTN_Engine.
//!
class PTN_Engine::PTN_EngineImpProxy final
{
public:
	//! Constructor.
	explicit PTN_EngineImpProxy(ACTIONS_THREAD_OPTION actionsThreadOption);

	~PTN_EngineImpProxy();

	PTN_EngineImpProxy(const PTN_EngineImpProxy &) = delete;
	PTN_EngineImpProxy(PTN_EngineImpProxy &&) = delete;
	PTN_EngineImpProxy &operator=(const PTN_EngineImpProxy &) = delete;
	PTN_EngineImpProxy &operator=(PTN_EngineImpProxy &&) = delete;

	//!
	//! \brief createTransition
	//! \param transitionProperties
	//!
	void createTransition(const TransitionProperties &transitionProperties);

	//!
	//! \brief createPlace
	//! \param placeProperties
	//!
	void createPlace(const PlaceProperties &placeProperties);

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

	//!
	//! Start the petri net event loop.
	//! \param log Flag logging the state of the net on or off.
	//! \param o Log output stream.
	//!
	void execute(const bool log = false, std::ostream &o = std::cout);

	//!
	//! \brief Stop the execution loop.
	//!
	void stop();

	//!
	//! Return the number of tokens in a given place.
	//! \param place The name of the place to get the number of tokens from.
	//! \return The number of tokens present in the place.
	//!
	size_t getNumberOfTokens(const std::string &place) const;

	//!
	//! Add a token in an input place.
	//! \param place Name of the place to be incremented.
	//!
	void incrementInputPlace(const std::string &place);

	//!
	//! Print the petri net places and number of tokens.
	//! \param o Output stream.
	//!
	void printState(std::ostream &o) const;

	//!
	//! \brief isEventLoopRunning
	//! \return
	//!
	bool isEventLoopRunning() const;

	//! Specify the thread where the actions should be run.
	void setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption);

	//! Get the information on which thread the actions are run.
	ACTIONS_THREAD_OPTION getActionsThreadOption() const;

	//!
	//! \brief setEventLoopSleepDuration
	//! \param sleepDuration
	//!
	void setEventLoopSleepDuration(const EventLoopSleepDuration sleepDuration);

	//!
	//! \brief getEventLoopSleepDuration
	//! \return
	//!
	EventLoopSleepDuration getEventLoopSleepDuration() const;

	//!
	//! \brief addArc
	//! \param arcProperties
	//!
	void addArc(const ArcProperties& arcProperties);

	//!
	//! \brief addArc
	//! \param arcProperties
	//!
	void removeArc(const ArcProperties& arcProperties);

	//!
	//! \brief clearNet
	//!
	void clearNet();

	//!
	//! \brief getPlacesProperties
	//! \return
	//!
	std::vector<PlaceProperties> getPlacesProperties() const;

	//!
	//! \brief getTransitionsProperties
	//! \return
	//!
	std::vector<TransitionProperties> getTransitionsProperties() const;

	//!
	//! \brief getArcsProperties
	//! \return
	//!
	std::vector<std::vector<ArcProperties>> getArcsProperties() const;

private:
	//! Synchronizes calls to m_ptnEngineImp
	mutable std::shared_mutex m_mutex;

	//! The PTN Engine implementation.
	PTN_EngineImp m_ptnEngineImp;
};

} // namespace ptne
