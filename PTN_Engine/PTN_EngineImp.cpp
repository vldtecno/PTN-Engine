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

#include "PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/JobQueue/JobQueue.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <algorithm>

namespace ptne
{
using namespace std;
using enum PTN_Engine::ACTIONS_THREAD_OPTION;

PTN_EngineImp::PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: m_actionsThreadOption(actionsThreadOption)
, m_eventLoop(*this)
{
	if (m_actionsThreadOption == JOB_QUEUE)
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

void PTN_EngineImp::createTransition(const TransitionProperties &transitionProperties)
{
	createTransition(transitionProperties.name,
					 transitionProperties.activationArcs,
					 transitionProperties.destinationArcs,
					 transitionProperties.inhibitorArcs,
					 !transitionProperties.additionalConditionsNames.empty() ?
						 m_conditions.getItems(transitionProperties.additionalConditionsNames) :
						 createAnonymousConditions(transitionProperties.additionalConditions),
					 transitionProperties.requireNoActionsInExecution);
}

void PTN_EngineImp::createPlace(PlaceProperties placeProperties)
{
	ActionFunction onEnterAction = placeProperties.onEnterAction;
	if (!placeProperties.onEnterActionFunctionName.empty())
	{
		placeProperties.onEnterAction = m_actions.getItem(placeProperties.onEnterActionFunctionName);
	}

	ActionFunction onExitAction = placeProperties.onExitAction;
	if (!placeProperties.onExitActionFunctionName.empty())
	{
		placeProperties.onExitAction = m_actions.getItem(placeProperties.onExitActionFunctionName);
	}

	auto place = make_shared<Place>(*this, placeProperties);
	m_places.insert(place);
}

bool PTN_EngineImp::isEventLoopRunning() const
{
	return m_eventLoop.isRunning();
}

void PTN_EngineImp::stop() noexcept
{
	m_eventLoop.stop();
}

void PTN_EngineImp::registerAction(const string &name, const ActionFunction &action)
{
	m_actions.addItem(name, action);
}

void PTN_EngineImp::registerCondition(const string &name, const ConditionFunction &condition)
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

void PTN_EngineImp::setActionsThreadOption(const PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot change actions thread option while the event loop is running.");
	}

	unique_lock actionsThreadOptionGuard(m_actionsThreadOptionMutex);

	if (m_actionsThreadOption == actionsThreadOption)
	{
		return;
	}
	if (actionsThreadOption == JOB_QUEUE && m_jobQueue == nullptr)
	{
		m_jobQueue = make_unique<JobQueue>();
	}
	else if (actionsThreadOption != JOB_QUEUE && m_jobQueue != nullptr)
	{
		m_jobQueue.reset();
	}
	m_actionsThreadOption = actionsThreadOption;
}

PTN_Engine::ACTIONS_THREAD_OPTION PTN_EngineImp::getActionsThreadOption() const
{
	shared_lock actionsThreadOptionGuard(m_actionsThreadOptionMutex);
	return m_actionsThreadOption;
}

void PTN_EngineImp::printState(ostream &o) const
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

	for (const auto &transition : enabledTransitions())
	{
		if (auto enabledTransition = lockWeakPtr(transition))
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

void PTN_EngineImp::addArc(const ArcProperties &arcProperties) const
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot add arc while the event loop is running.");
	}

	if (!m_places.contains(arcProperties.placeName))
	{
		throw PTN_Exception("The place" + arcProperties.placeName + " must already exist in order to link to an arc.");
	}
	auto spPlace = m_places.getPlace(arcProperties.placeName);

	if (!m_transitions.contains(arcProperties.transitionName))
	{
		throw PTN_Exception("The transition " + arcProperties.transitionName +" must already exist in order to link to an arc.");
	}

	auto spTransition = m_transitions.getTransition(arcProperties.transitionName);
	spTransition->addPlace(spPlace, arcProperties.type, arcProperties.weight);
}

void PTN_EngineImp::removeArc(const ArcProperties &arcProperties) const
{
	if (isEventLoopRunning())
	{
		throw PTN_Exception("Cannot remove arc while the event loop is running.");
	}

	if (!m_places.contains(arcProperties.placeName))
	{
		throw PTN_Exception("The place" + arcProperties.placeName + " must already exist in order to unlink an arc.");
	}
	auto spPlace = m_places.getPlace(arcProperties.placeName);

	if (m_transitions.contains(arcProperties.transitionName))
	{
		throw PTN_Exception("The transition " + arcProperties.transitionName +" must already exist in order to unlink an arc.");
	}

	auto spTransition = m_transitions.getTransition(arcProperties.transitionName);
	spTransition->removePlace(spPlace, arcProperties.type);
}

vector<PlaceProperties> PTN_EngineImp::getPlacesProperties() const
{
	return m_places.getPlacesProperties();
}

vector<TransitionProperties> PTN_EngineImp::getTransitionsProperties() const
{
	return m_transitions.getTransitionsProperties();
}

vector<vector<ArcProperties>> PTN_EngineImp::getArcsProperties() const
{
	return m_transitions.getArcsProperties();
}

// Private

void PTN_EngineImp::createTransition(const string &name,
                                     const vector<ArcProperties> &activationArcs,
                                     const vector<ArcProperties> &destinationArcs,
                                     const vector<ArcProperties> &inhibitorArcs,
                                     const vector<pair<string, ConditionFunction>> &additionalConditions,
                                     const bool requireNoActionsInExecution)
{
	// if a transition with this name already exists in the net, throw an exception
	if (m_transitions.hasTransition(name))
	{
		throw PTN_Exception("Cannot create transition that already exists. Name: " + name);
	}

	auto getArcsFromArcsProperties = [this](const vector<ArcProperties> &arcProperties)
	{
		vector<Arc> arcs;
		for (const auto &arcProperty : arcProperties)
		{
			arcs.emplace_back(m_places.getPlace(arcProperty.placeName), arcProperty.weight);
		}
		return arcs;
	};

	m_transitions.insert(
	make_shared<Transition>(name,
							getArcsFromArcsProperties(activationArcs),
							getArcsFromArcsProperties(destinationArcs),
							getArcsFromArcsProperties(inhibitorArcs),
							additionalConditions,
							requireNoActionsInExecution));

}

vector<pair<string, ConditionFunction>>
PTN_EngineImp::createAnonymousConditions(const vector<ConditionFunction> &conditions) const
{
	vector<pair<string, ConditionFunction>> anonymousConditionsVector;
	ranges::transform(conditions.cbegin(), conditions.cend(), back_inserter(anonymousConditionsVector),
			  [](const auto &condition) { return pair<string, ConditionFunction>("", condition); });
	return anonymousConditionsVector;
}

void PTN_EngineImp::addJob(const ActionFunction &actionFunction)
{
	if (m_actionsThreadOption != JOB_QUEUE || m_jobQueue == nullptr ||
		!m_jobQueue->isActive())
	{
		throw PTN_Exception("addJob incorrectly called");
	}

	m_jobQueue->addJob(actionFunction);
}

} // namespace ptne
