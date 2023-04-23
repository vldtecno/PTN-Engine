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

#include "PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/IImporter.h"
#include "PTN_Engine/JobQueue/JobQueue.h"

namespace ptne
{
using namespace std;

PTN_EngineImp::PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: m_eventLoop(*this)
, m_actionsThreadOption(actionsThreadOption)
{
	if (m_actionsThreadOption == PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE)
	{
		m_jobQueue = make_unique<JobQueue>();
	}
}

PTN_EngineImp::~PTN_EngineImp()
{
	stop();
}

void PTN_EngineImp::clearInputPlaces()
{
	m_places.clearInputPlaces();
	m_newInputReceived = false;
}

void PTN_EngineImp::clearNet()
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot clear net while the event loop is running.");
	}
	m_transitions.clear();
	m_places.clear();
}

void PTN_EngineImp::createPlace(const string &name,
                                const size_t initialNumberOfTokens,
                                ActionFunction onEnterAction,
                                ActionFunction onExitAction,
                                const bool input)
{
	auto place = make_shared<Place>(*this, name, initialNumberOfTokens, onEnterAction, onExitAction, input);
	m_places.insert(name, place);
}

void PTN_EngineImp::createPlaceStr(const string &name,
                                   const size_t initialNumberOfTokens,
                                   const string &onEnterActionName,
                                   const string &onExitActionName,
                                   const bool input)
{
	ActionFunction onEnterAction;
	if (!onEnterActionName.empty())
	{
		onEnterAction = m_actions.getItem(onEnterActionName);
	}

	ActionFunction onExitAction;
	if (!onExitActionName.empty())
	{
		onExitAction = m_actions.getItem(onExitActionName);
	}

	auto place = make_shared<Place>(*this, name, initialNumberOfTokens, onEnterActionName, onEnterAction,
									onExitActionName, onExitAction, input);
	m_places.insert(name, place);
}

void PTN_EngineImp::createTransition(const vector<string> &activationPlaces,
                                     const vector<size_t> &activationWeights,
                                     const vector<string> &destinationPlaces,
                                     const vector<size_t> &destinationWeights,
                                     const vector<string> &inhibitorPlaces,
                                     const vector<string> &additionalConditions,
                                     const bool requireNoActionsInExecution)
{
	createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights, inhibitorPlaces,
					 m_conditions.getItems(additionalConditions), requireNoActionsInExecution);
}

void PTN_EngineImp::createTransition(const vector<string> &activationPlaces,
                                     const vector<size_t> &activationWeights,
                                     const vector<string> &destinationPlaces,
                                     const vector<size_t> &destinationWeights,
                                     const vector<string> &inhibitorPlaces,
                                     const vector<ConditionFunction> &additionalConditions,
                                     const bool requireNoActionsInExecution)
{
	createTransition(activationPlaces, activationWeights, destinationPlaces, destinationWeights, inhibitorPlaces,
					 createAnonymousConditions(additionalConditions), requireNoActionsInExecution);
}

bool PTN_EngineImp::isEventLoopRunning() const
{
	return m_eventLoop.isRunning();
}

void PTN_EngineImp::stop() noexcept
{
	m_eventLoop.stop();
}

void PTN_EngineImp::registerAction(const string &name, ActionFunction action)
{
	m_actions.addItem(name, action);
}

void PTN_EngineImp::registerCondition(const string &name, ConditionFunction condition)
{
	m_conditions.addItem(name, condition);
}

size_t PTN_EngineImp::getNumberOfTokens(const string &place) const
{
	return m_places.getNumberOfTokens(place);
}

void PTN_EngineImp::incrementInputPlace(const string &place)
{
	m_places.incrementInputPlace(place);
	m_newInputReceived = true;
	m_eventLoop.notifyNewEvent();
}

void PTN_EngineImp::export_(IExporter &exporter) const
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot change actions thread option while the event loop is running.");
	}

	string actionsThreadOptionStr;
	if (!ACTIONS_THREAD_OPTION_toString(m_actionsThreadOption, actionsThreadOptionStr))
	{
		std::unique_ptr<std::thread> m_eventLoopThread;
		throw PTN_Exception("Invalid ACTIONS_THREAD_OPTION");
	}
	exporter.exportActionsThreadOption(actionsThreadOptionStr);

	m_places.export_(exporter);
	m_transitions.export_(exporter);
}

void PTN_EngineImp::import(const IImporter &importer)
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot import while the event loop is running.");
	}

	// TODO: Not exception safe, maybe this should go to a constructor instead.
	clearNet();

	PTN_Engine::ACTIONS_THREAD_OPTION actionThreadOption = m_actionsThreadOption;
	string actionsThreadOptionStr = importer.getActionsThreadOption();
	if (ACTIONS_THREAD_OPTION_fromString(actionsThreadOptionStr, actionThreadOption))
	{
		setActionsThreadOption(actionThreadOption);
	}
	else
	{
		throw PTN_Exception("Invalid input ACTIONS_THREAD_OPTION string");
	}

	// create places
	for (const IImporter::PlaceInfo &placeInfo : importer.getPlaces())
	{
		createPlaceStr(get<0>(placeInfo), get<1>(placeInfo), get<2>(placeInfo), get<3>(placeInfo),
					   get<4>(placeInfo));
	}

	// create transitions
	for (const IImporter::TransitionInfo &transitionInfo : importer.getTransitions())
	{
		createTransition(get<0>(transitionInfo), get<1>(transitionInfo), get<2>(transitionInfo),
		                 get<3>(transitionInfo), get<4>(transitionInfo), get<5>(transitionInfo),
		                 get<6>(transitionInfo));
	}
}

void PTN_EngineImp::setActionsThreadOption(const PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot change actions thread option while the event loop is running.");
	}

	unique_lock<shared_mutex> actionsThreadOptionGuard(m_actionsThreadOptionMutex);

	if (m_actionsThreadOption == actionsThreadOption)
	{
		return;
	}
	if (actionsThreadOption == PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE && m_jobQueue == nullptr)
	{
		m_jobQueue = make_unique<JobQueue>();
	}
	else if (actionsThreadOption != PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE && m_jobQueue != nullptr)
	{
		m_jobQueue.reset();
	}
	m_actionsThreadOption = actionsThreadOption;
}

PTN_Engine::ACTIONS_THREAD_OPTION PTN_EngineImp::getActionsThreadOption() const
{
	shared_lock<shared_mutex> actionsThreadOptionGuard(m_actionsThreadOptionMutex);
	return m_actionsThreadOption;
}

void PTN_EngineImp::printState(std::ostream &o) const
{
	m_places.printState(o);
}

void PTN_EngineImp::execute(const bool log, ostream &o)
{
	m_eventLoop.start(log, o);
}

bool PTN_EngineImp::executeInt(const bool log, ostream &o)
{
	bool firedAtLeastOneTransition = false;
	setNewInputReceived(false);

	if (log)
	{
		printState(o);
	}
	vector<weak_ptr<Transition>> _enabledTransitions = enabledTransitions();
	for (auto &transition : _enabledTransitions)
	{
		if (auto enabledTransition = transition.lock())
		{
			firedAtLeastOneTransition |= enabledTransition->execute();
		}
	}
	return firedAtLeastOneTransition;
}

bool PTN_EngineImp::getNewInputReceived() const
{
	return m_newInputReceived;
}

void PTN_EngineImp::setNewInputReceived(const bool newInputReceived)
{
	m_newInputReceived = newInputReceived;
}

vector<weak_ptr<Transition>> PTN_EngineImp::enabledTransitions() const
{
	return m_transitions.collectEnabledTransitionsRandomly();
}

void PTN_EngineImp::setEventLoopSleepDuration(const PTN_Engine::EventLoopSleepDuration sleepDuration)
{
	m_eventLoop.setSleepDuration(sleepDuration);
}

PTN_Engine::EventLoopSleepDuration PTN_EngineImp::getEventLoopSleepDuration() const
{
	return m_eventLoop.getSleepDuration();
}

// Private

void PTN_EngineImp::createTransition(const vector<string> &activationPlaces,
                                     const vector<size_t> &activationWeights,
                                     const vector<string> &destinationPlaces,
                                     const vector<size_t> &destinationWeights,
                                     const vector<string> &inhibitorPlaces,
                                     const vector<pair<string, ConditionFunction>> &additionalConditions,
                                     const bool requireNoActionsInExecution)
{
	vector<WeakPtrPlace> activationPlacesVector = m_places.getPlacesFromNames(activationPlaces);
	vector<WeakPtrPlace> destinationPlacesVector = m_places.getPlacesFromNames(destinationPlaces);
	vector<WeakPtrPlace> inhibitorPlacesVector = m_places.getPlacesFromNames(inhibitorPlaces);

	m_transitions.insert(
	make_shared<Transition>(activationPlacesVector, activationWeights, destinationPlacesVector, destinationWeights,
							inhibitorPlacesVector, additionalConditions, requireNoActionsInExecution));
}

const vector<pair<string, ConditionFunction>>
PTN_EngineImp::createAnonymousConditions(const vector<ConditionFunction> &conditions) const
{
	vector<pair<string, ConditionFunction>> anonymousConditionsVector;
	for (const auto &condition : conditions)
	{
		anonymousConditionsVector.emplace_back(pair<string, ConditionFunction>("", condition));
	}
	return anonymousConditionsVector;
}

void PTN_EngineImp::addJob(const ActionFunction &actionFunction)
{
	if (m_actionsThreadOption != PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE || m_jobQueue == nullptr ||
		!m_jobQueue->isActive())
	{
		throw PTN_Exception("addJob incorrectly called");
	}

	m_jobQueue->addJob(actionFunction);
}

bool PTN_EngineImp::ACTIONS_THREAD_OPTION_fromString(const string &actionsThreadOptionStr,
													 PTN_Engine::ACTIONS_THREAD_OPTION &out)
{
	if (actionsThreadOptionStr == "DETACHED")
	{
		out = PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED;
	}
	else if (actionsThreadOptionStr == "EVENT_LOOP")
	{
		out = PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP;
	}
	else if (actionsThreadOptionStr == "JOB_QUEUE")
	{
		out = PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE;
	}
	else
	{
		return false;
	}
	return true;
}

bool PTN_EngineImp::ACTIONS_THREAD_OPTION_toString(const PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption,
												   string &out)
{
	switch (actionsThreadOption)
	{
	case PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED:
		out = "DETACHED";
		break;
	case PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP:
		out = "EVENT_LOOP";
		break;
	case PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE:
		out = "JOB_QUEUE";
		break;
	default:
		return false;
		break;
	}
	return true;
}

} // namespace ptne
