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

#include "PTN_Engine/PTN_EngineImpProxy.h"

namespace ptne
{
using namespace std;

PTN_Engine::PTN_EngineImpProxy::PTN_EngineImpProxy(ACTIONS_THREAD_OPTION actionsThreadOption)
: m_ptnEngineImp(actionsThreadOption)
{
	setActionsThreadOption(actionsThreadOption);
}

PTN_Engine::PTN_EngineImpProxy::~PTN_EngineImpProxy()
{
	m_ptnEngineImp.stop();
}

void PTN_Engine::PTN_EngineImpProxy::setEventLoopSleepDuration(const EventLoopSleepDuration sleepDuration)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.setEventLoopSleepDuration(sleepDuration);
}

PTN_Engine::EventLoopSleepDuration PTN_Engine::PTN_EngineImpProxy::getEventLoopSleepDuration() const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.getEventLoopSleepDuration();
}

void PTN_Engine::PTN_EngineImpProxy::addArc(const ArcProperties &arcProperties)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.addArc(arcProperties);
}

void PTN_Engine::PTN_EngineImpProxy::removeArc(const ArcProperties &arcProperties)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.removeArc(arcProperties);
}

void PTN_Engine::PTN_EngineImpProxy::clearNet()
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.clearNet();
}

vector<PlaceProperties> PTN_Engine::PTN_EngineImpProxy::getPlacesProperties() const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.getPlacesProperties();
}

vector<TransitionProperties> PTN_Engine::PTN_EngineImpProxy::getTransitionsProperties() const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.getTransitionsProperties();
}

vector<vector<ArcProperties>> PTN_Engine::PTN_EngineImpProxy::getArcsProperties() const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.getArcsProperties();
}

void PTN_Engine::PTN_EngineImpProxy::createTransition(const TransitionProperties &transitionProperties)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.createTransition(transitionProperties);
}

void PTN_Engine::PTN_EngineImpProxy::createPlace(const PlaceProperties &placeProperties)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.createPlace(placeProperties);
}

void PTN_Engine::PTN_EngineImpProxy::registerAction(const string &name, const ActionFunction &action)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.registerAction(name, action);
}

void PTN_Engine::PTN_EngineImpProxy::registerCondition(const string &name, const ConditionFunction &condition)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.registerCondition(name, condition);
}

void PTN_Engine::PTN_EngineImpProxy::execute(const bool log, ostream &o)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.execute(log, o);
}

void PTN_Engine::PTN_EngineImpProxy::stop()
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.stop();
}

size_t PTN_Engine::PTN_EngineImpProxy::getNumberOfTokens(const string &place) const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.getNumberOfTokens(place);
}

void PTN_Engine::PTN_EngineImpProxy::incrementInputPlace(const string &place)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.incrementInputPlace(place);
}

void PTN_Engine::PTN_EngineImpProxy::printState(ostream &o) const
{
	shared_lock guard(m_mutex);
	m_ptnEngineImp.printState(o);
}

bool PTN_Engine::PTN_EngineImpProxy::isEventLoopRunning() const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.isEventLoopRunning();
}

void PTN_Engine::PTN_EngineImpProxy::setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption)
{
	unique_lock guard(m_mutex);
	m_ptnEngineImp.setActionsThreadOption(actionsThreadOption);
}

PTN_Engine::ACTIONS_THREAD_OPTION PTN_Engine::PTN_EngineImpProxy::getActionsThreadOption() const
{
	shared_lock guard(m_mutex);
	return m_ptnEngineImp.getActionsThreadOption();
}

} // namespace ptne
