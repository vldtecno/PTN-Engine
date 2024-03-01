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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/PTN_EngineImpProxy.h"

namespace ptne
{
using namespace std;

PTN_Engine::~PTN_Engine() = default;

PTN_Engine::PTN_Engine(ACTIONS_THREAD_OPTION actionsRuntimeThread)
: m_impProxy(make_unique<PTN_EngineImpProxy>(actionsRuntimeThread))
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

void PTN_Engine::addArc(const ArcProperties &arcProperties)
{
	m_impProxy->addArc(arcProperties);
}

void PTN_Engine::removeArc(const ArcProperties &arcProperties)
{
	m_impProxy->removeArc(arcProperties);
}

void PTN_Engine::clearNet()
{
	m_impProxy->clearNet();
}

vector<PlaceProperties> PTN_Engine::getPlacesProperties() const
{
	return m_impProxy->getPlacesProperties();
}

vector<TransitionProperties> PTN_Engine::getTransitionsProperties() const
{
	return m_impProxy->getTransitionsProperties();
}

vector<vector<ArcProperties>> PTN_Engine::getArcsProperties() const
{
	return m_impProxy->getArcsProperties();
}

void PTN_Engine::createTransition(const TransitionProperties &transitionProperties)
{
	m_impProxy->createTransition(transitionProperties);
}

void PTN_Engine::createPlace(const PlaceProperties &placeProperties)
{
	m_impProxy->createPlace(placeProperties);
}

void PTN_Engine::registerAction(const string &name, const ActionFunction &action) const
{
	m_impProxy->registerAction(name, action);
}

void PTN_Engine::registerCondition(const string &name, const ConditionFunction &condition) const
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

namespace
{
	const string ACTIONS_THREAD_OPTION_SINGLE_THREAD{ "SINGLE_THREAD" };
	const string ACTIONS_THREAD_OPTION_EVENT_LOOP{ "EVENT_LOOP" };
	const string ACTIONS_THREAD_OPTION_DETACHED{ "DETACHED" };
	const string ACTIONS_THREAD_OPTION_JOB_QUEUE{ "JOB_QUEUE" };
}

PTN_Engine::ACTIONS_THREAD_OPTION
PTN_Engine::toACTIONS_THREAD_OPTION(const string &actionsThreadOptionStr)
{
	using enum ACTIONS_THREAD_OPTION;
	if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_SINGLE_THREAD)
	{
		return SINGLE_THREAD;
	}
	else if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_EVENT_LOOP)
	{
		return EVENT_LOOP;
	}
	else if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_DETACHED)
	{
		return DETACHED;
	}
	else if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_JOB_QUEUE)
	{
		return JOB_QUEUE;
	}
	else
	{
		throw PTN_Exception("Could not convert " + actionsThreadOptionStr + " to ACTIONS_THREAD_OPTION");
	}
}

string PTN_Engine::toString(ACTIONS_THREAD_OPTION actionsThreadOption)
{
	using enum ACTIONS_THREAD_OPTION;
	switch (actionsThreadOption)
	{
	default:
	{
		throw PTN_Exception("Could not convert to string");
	}
	case DETACHED:
	{
		return ACTIONS_THREAD_OPTION_DETACHED;
	}
	case SINGLE_THREAD:
	{
		return ACTIONS_THREAD_OPTION_SINGLE_THREAD;
	}
	case EVENT_LOOP:
	{
		return ACTIONS_THREAD_OPTION_EVENT_LOOP;
	}
	case JOB_QUEUE:
	{
		return ACTIONS_THREAD_OPTION_JOB_QUEUE;
	}
	}
}

} // namespace ptne
