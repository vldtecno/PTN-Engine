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
	~PTN_EngineImpProxy();
	explicit PTN_EngineImpProxy(ACTIONS_THREAD_OPTION actionsThreadOption);
	PTN_EngineImpProxy(const PTN_EngineImpProxy &) = delete;
	PTN_EngineImpProxy(PTN_EngineImpProxy &&) = delete;
	PTN_EngineImpProxy &operator=(const PTN_EngineImpProxy &) = delete;
	PTN_EngineImpProxy &operator=(PTN_EngineImpProxy &&) = delete;

	void addArc(const ArcProperties &arcProperties);

	void clearNet();

	void createTransition(const TransitionProperties &transitionProperties);

	void createPlace(const PlaceProperties &placeProperties);

	void execute(const bool log = false, std::ostream &o = std::cout);

	ACTIONS_THREAD_OPTION getActionsThreadOption() const;

	EventLoopSleepDuration getEventLoopSleepDuration() const;

	size_t getNumberOfTokens(const std::string &place) const;

	std::vector<PlaceProperties> getPlacesProperties() const;

	std::vector<TransitionProperties> getTransitionsProperties() const;

	void incrementInputPlace(const std::string &place);

	bool isEventLoopRunning() const;

	void printState(std::ostream &o) const;

	void registerAction(const std::string &name, const ActionFunction &action);

	void registerCondition(const std::string &name, const ConditionFunction &condition);

	void removeArc(const ArcProperties &arcProperties);

	void setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption);

	void setEventLoopSleepDuration(const EventLoopSleepDuration sleepDuration);

	void stop();

private:
	//! Synchronizes calls to m_ptnEngineImp
	mutable std::shared_mutex m_mutex;

	//! The PTN Engine implementation.
	PTN_EngineImp m_ptnEngineImp;
};

} // namespace ptne
