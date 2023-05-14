/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2023 Eduardo Valgôde
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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/PTN_EngineImpProxy.h"

namespace ptne
{
using namespace std;

PTN_Engine::~PTN_Engine()
{
}

PTN_Engine::PTN_Engine(ACTIONS_THREAD_OPTION actionsRuntimeThread)
: m_impProxy(new PTN_EngineImpProxy(actionsRuntimeThread))
{
}


void PTN_Engine::setEventLoopSleepDuration(const EventLoopSleepDuration sleepDuration)
{
	m_impProxy->setEventLoopSleepDuration(sleepDuration);
}

PTN_Engine::EventLoopSleepDuration PTN_Engine::getEventLoopSleepDuration() const
{
    return m_impProxy->getEventLoopSleepDuration();
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<string> &inhibitorPlaces,
                                  const vector<ConditionFunction> &additionalConditions,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
	                             inhibitorPlaces, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<string> &inhibitorPlaces,
                                  const vector<string> &additionalConditions,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
								 inhibitorPlaces, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
								 vector<string>{}, vector<ConditionFunction>{}, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<string> &inhibitorPlaces,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
								 inhibitorPlaces, vector<ConditionFunction>{}, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<ConditionFunction> &additionalConditions,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
								 vector<string>{}, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
								 vector<string>{}, vector<ConditionFunction>{}, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const vector<string> &inhibitorPlaces,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
								 inhibitorPlaces, vector<ConditionFunction>{}, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const vector<ConditionFunction> &additionalConditions,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
								 vector<string>{}, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const vector<string> &inhibitorPlaces,
                                  const vector<ConditionFunction> &additionalConditions,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
								 inhibitorPlaces, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::createTransition(const std::vector<std::string> &activationPlaces,
                                  const std::vector<std::string> &destinationPlaces,
                                  const std::vector<std::string> &inhibitorPlaces,
                                  const std::vector<std::string> &additionalConditions,
                                  const bool requireNoActionsInExecution)
{
	m_impProxy->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
								 inhibitorPlaces, additionalConditions, requireNoActionsInExecution);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             ActionFunction onEnterAction,
                             ActionFunction onExitAction,
                             const bool input)
{
    m_impProxy->createPlace(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             const string &onEnterAction,
                             const string &onExitAction,
                             const bool input)
{
    m_impProxy->createPlaceStr(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::createPlace(const string &name, const size_t initialNumberOfTokens, const bool input)
{
    m_impProxy->createPlace(name, initialNumberOfTokens, nullptr, nullptr, input);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             ActionFunction onEnterAction,
                             const bool input)
{
    m_impProxy->createPlace(name, initialNumberOfTokens, onEnterAction, nullptr, input);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             const string &onEnterAction,
                             const bool input)
{
    m_impProxy->createPlaceStr(name, initialNumberOfTokens, onEnterAction, "", input);
}

void PTN_Engine::registerAction(const string &name, ActionFunction action)
{
    m_impProxy->registerAction(name, action);
}

void PTN_Engine::registerCondition(const string &name, ConditionFunction condition)
{
    m_impProxy->registerCondition(name, condition);
}

void PTN_Engine::execute(const bool log, ostream &o)
{
    m_impProxy->execute(log, o);
}

size_t PTN_Engine::getNumberOfTokens(const string &place) const
{
    return m_impProxy->getNumberOfTokens(place);
}

void PTN_Engine::incrementInputPlace(const string &place)
{
    m_impProxy->incrementInputPlace(place);
}

void PTN_Engine::printState(ostream &o) const
{
    m_impProxy->printState(o);
}

void PTN_Engine::export_(IExporter &exporter) const
{
    m_impProxy->export_(exporter);
}

void PTN_Engine::import(const IImporter &importer)
{
    m_impProxy->import(importer);
}

void PTN_Engine::setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption)
{
    m_impProxy->setActionsThreadOption(actionsThreadOption);
}

PTN_Engine::ACTIONS_THREAD_OPTION PTN_Engine::getActionsThreadOption() const
{
    return m_impProxy->getActionsThreadOption();
}

bool PTN_Engine::isEventLoopRunning() const
{
    return m_impProxy->isEventLoopRunning();
}

void PTN_Engine::stop()
{
    m_impProxy->stop();
}

} // namespace ptne
