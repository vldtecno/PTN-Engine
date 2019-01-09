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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/IImporter.h"
#include "PTN_Engine/PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/Place.h"

namespace ptne
{
using namespace std;

PTN_Engine::~PTN_Engine()
{
}

PTN_Engine::PTN_Engine()
: m_implementation(new PTN_EngineImp())
{
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<string> &inhibitorPlaces,
                                  const vector<ConditionFunctorPtr> &additionalConditions)
{
	m_implementation->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									   inhibitorPlaces, additionalConditions);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<string> &inhibitorPlaces,
                                  const vector<string> &additionalConditions)
{
	m_implementation->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									   inhibitorPlaces, additionalConditions);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights)
{
	m_implementation->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									   vector<string>{}, vector<ConditionFunctorPtr>{});
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<string> &inhibitorPlaces)
{
	m_implementation->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									   inhibitorPlaces, vector<ConditionFunctorPtr>{});
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<size_t> &activationWeights,
                                  const vector<string> &destinationPlaces,
                                  const vector<size_t> &destinationWeights,
                                  const vector<ConditionFunctorPtr> &additionalConditions)
{
	m_implementation->createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
									   vector<string>{}, additionalConditions);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces, const vector<string> &destinationPlaces)
{
	m_implementation->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
									   vector<string>{}, vector<ConditionFunctorPtr>{});
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const vector<string> &inhibitorPlaces)
{
	m_implementation->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
									   inhibitorPlaces, vector<ConditionFunctorPtr>{});
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const vector<ConditionFunctorPtr> &additionalConditions)
{
	m_implementation->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
									   vector<string>{}, additionalConditions);
}

void PTN_Engine::createTransition(const vector<string> &activationPlaces,
                                  const vector<string> &destinationPlaces,
                                  const vector<string> &inhibitorPlaces,
                                  const vector<ConditionFunctorPtr> &additionalConditions)
{
	m_implementation->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
									   inhibitorPlaces, additionalConditions);
}

void PTN_Engine::createTransition(const std::vector<std::string> &activationPlaces,
                                  const std::vector<std::string> &destinationPlaces,
                                  const std::vector<std::string> &inhibitorPlaces,
                                  const std::vector<std::string> &additionalConditions)
{
	m_implementation->createTransition(activationPlaces, vector<size_t>{}, destinationPlaces, vector<size_t>{},
									   inhibitorPlaces, additionalConditions);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             ActionFunctorPtr onEnterAction,
                             ActionFunctorPtr onExitAction,
                             const bool input)
{
    m_implementation->createPlace(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             const string &onEnterAction,
                             const string &onExitAction,
                             const bool input)
{
    m_implementation->createPlaceStr(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::createPlace(const string &name, const size_t initialNumberOfTokens, const bool input)
{
    m_implementation->createPlace(name, initialNumberOfTokens, nullptr, nullptr, input);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             ActionFunctorPtr onEnterAction,
                             const bool input)
{
    m_implementation->createPlace(name, initialNumberOfTokens, onEnterAction, nullptr, input);
}

void PTN_Engine::createPlace(const string &name,
                             const size_t initialNumberOfTokens,
                             const string &onEnterAction,
                             const bool input)
{
    m_implementation->createPlaceStr(name, initialNumberOfTokens, onEnterAction, "", input);
}

void PTN_Engine::registerAction(const string &name, ActionFunctorPtr action)
{
    m_implementation->registerAction(name, action);
}

void PTN_Engine::registerCondition(const string &name, ConditionFunctorPtr condition)
{
    m_implementation->registerCondition(name, condition);
}

void PTN_Engine::execute(const bool log, ostream &o)
{
    m_implementation->execute(log, o);
}

void PTN_Engine::execute()
{
    m_implementation->execute(false);
}

size_t PTN_Engine::getNumberOfTokens(const string &place) const
{
    return m_implementation->getNumberOfTokens(place);
}

void PTN_Engine::incrementInputPlace(const string &place)
{
    m_implementation->incrementInputPlace(place);
}

void PTN_Engine::printState(ostream &o) const
{
    m_implementation->printState(o);
}

void PTN_Engine::export_(IExporter &exporter) const
{
    m_implementation->export_(exporter);
}

void PTN_Engine::import(const IImporter &importer)
{
    m_implementation->import(importer);
}

} // namespace ptne
