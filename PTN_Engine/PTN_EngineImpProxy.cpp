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
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.setEventLoopSleepDuration(sleepDuration);
}

PTN_Engine::EventLoopSleepDuration PTN_Engine::PTN_EngineImpProxy::getEventLoopSleepDuration() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_ptnEngineImp.getEventLoopSleepDuration();
}

void PTN_Engine::PTN_EngineImpProxy::createTransition(const vector<string> &activationPlaces,
                                                      const vector<size_t> &activationWeights,
                                                      const vector<string> &destinationPlaces,
                                                      const vector<size_t> &destinationWeights,
                                                      const vector<string> &inhibitorPlaces,
                                                      const vector<ConditionFunction> &additionalConditions,
                                                      const bool requireNoActionsInExecution)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									inhibitorPlaces, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::PTN_EngineImpProxy::createTransition(const vector<string> &activationPlaces,
                                                      const vector<size_t> &activationWeights,
                                                      const vector<string> &destinationPlaces,
                                                      const vector<size_t> &destinationWeights,
                                                      const vector<string> &inhibitorPlaces,
                                                      const vector<string> &additionalConditions,
                                                      const bool requireNoActionsInExecution)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									inhibitorPlaces, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::PTN_EngineImpProxy::createPlace(const string &name,
                                                 const size_t initialNumberOfTokens,
                                                 ActionFunction onEnterAction,
                                                 ActionFunction onExitAction,
                                                 const bool input)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.createPlace(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::PTN_EngineImpProxy::createPlaceStr(const string &name,
                                                    const size_t initialNumberOfTokens,
                                                    const string &onEnterAction,
                                                    const string &onExitAction,
                                                    const bool input)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.createPlaceStr(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::PTN_EngineImpProxy::registerAction(const string &name, ActionFunction action)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.registerAction(name, action);
}

void PTN_Engine::PTN_EngineImpProxy::registerCondition(const string &name, ConditionFunction condition)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.registerCondition(name, condition);
}

void PTN_Engine::PTN_EngineImpProxy::execute(const bool log, ostream &o)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.execute(log, o);
}

void PTN_Engine::PTN_EngineImpProxy::stop()
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.stop();
}

size_t PTN_Engine::PTN_EngineImpProxy::getNumberOfTokens(const string &place) const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_ptnEngineImp.getNumberOfTokens(place);
}

void PTN_Engine::PTN_EngineImpProxy::incrementInputPlace(const string &place)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.incrementInputPlace(place);
}

void PTN_Engine::PTN_EngineImpProxy::printState(ostream &o) const
{
	shared_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.printState(o);
}

void PTN_Engine::PTN_EngineImpProxy::export_(IExporter &exporter) const
{
	shared_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.export_(exporter);
}

void PTN_Engine::PTN_EngineImpProxy::import(const IImporter &importer)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.import(importer);
}

bool PTN_Engine::PTN_EngineImpProxy::isEventLoopRunning() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_ptnEngineImp.isEventLoopRunning();
}

void PTN_Engine::PTN_EngineImpProxy::setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption)
{
	unique_lock<shared_mutex> guard(m_mutex);
	m_ptnEngineImp.setActionsThreadOption(actionsThreadOption);
}

PTN_Engine::ACTIONS_THREAD_OPTION PTN_Engine::PTN_EngineImpProxy::getActionsThreadOption() const
{
	shared_lock<shared_mutex> guard(m_mutex);
	return m_ptnEngineImp.getActionsThreadOption();
}

} // namespace ptne
