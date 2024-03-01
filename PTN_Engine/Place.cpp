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

#include "PTN_Engine/Place.h"
#include "PTN_Engine/IPTN_EnginePlace.h"
#include "PTN_Engine/PTN_Exception.h"
#include <string>
#include <thread>
#include <mutex>

namespace ptne
{
using namespace std;

Place::Place(IPTN_EnginePlace &parent,
             const string &name,
             const size_t initialNumberOfTokens,
             const ActionFunction &onEnterAction,
             const ActionFunction &onExitAction,
             const bool input)
: m_ptnEngine(parent)
, m_name(name)
, m_onEnterAction(onEnterAction)
, m_onExitAction(onExitAction)
, m_numberOfTokens(initialNumberOfTokens)
, m_isInputPlace(input)
{
}

Place::Place(IPTN_EnginePlace &parent,
             const string &name,
             const size_t initialNumberOfTokens,
             const string &onEnterActionName,
             const ActionFunction &onEnterAction,
             const string &onExitActionName,
             const ActionFunction &onExitAction,
             const bool input)
: m_ptnEngine(parent)
, m_name(name)
, m_onEnterActionName(onEnterActionName)
, m_onEnterAction(onEnterAction)
, m_onExitActionName(onExitActionName)
, m_onExitAction(onExitAction)
, m_numberOfTokens(initialNumberOfTokens)
, m_isInputPlace(input)
{
}

string Place::getName() const
{
	return m_name;
}

void Place::enterPlace(const size_t tokens)
{
	unique_lock guard(m_mutex);
	increaseNumberOfTokens(tokens);
	if (m_onEnterAction == nullptr)
	{
		return;
	}
	while (m_blockStartingOnEnterActions)
	{
		// TODO make this wait period configurable
		// add a max wait time after which an exception
		// is thrown.
		this_thread::sleep_for(100ms);
	}
	executeAction(m_onEnterAction, m_onEnterActionsInExecution);
}

void Place::exitPlace(const size_t tokens)
{
	unique_lock guard(m_mutex);
	decreaseNumberOfTokens(tokens);
	if (m_onExitAction == nullptr)
	{
		return;
	}
	executeAction(m_onExitAction, m_onExitActionsInExecution);
}

void Place::increaseNumberOfTokens(const size_t tokens)
{
	if (tokens == 0)
	{
		throw NullTokensException();
	}

	if (tokens > UINT_MAX - m_numberOfTokens)
	{
		throw OverflowException(tokens);
	}

	m_numberOfTokens += tokens;
}

void Place::decreaseNumberOfTokens(const size_t tokens)
{
	if (m_numberOfTokens < tokens)
	{
		throw NotEnoughTokensException();
	}
	if (tokens == 0) // reset
	{
		m_numberOfTokens = 0;
	}
	else
	{
		m_numberOfTokens -= tokens;
	}
}

void Place::setNumberOfTokens(const size_t tokens)
{
	unique_lock guard(m_mutex);
	m_numberOfTokens = tokens;
}

size_t Place::getNumberOfTokens() const
{
	shared_lock guard(m_mutex);
	return m_numberOfTokens;
}

bool Place::isInputPlace() const
{
	shared_lock guard(m_mutex);
	return m_isInputPlace;
}

string Place::getOnEnterActionName() const
{
	shared_lock guard(m_mutex);
	return m_onEnterActionName;
}

string Place::getOnExitActionName() const
{
	shared_lock guard(m_mutex);
	return m_onExitActionName;
}

void Place::executeAction(const ActionFunction &action, atomic<size_t> &actionsInExecution)
{
	switch (m_ptnEngine.getActionsThreadOption())
	{
	default:
	{
		throw PTN_Exception("Invalid configuration");
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD:
	case PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP:
	{
		++actionsInExecution;
		action();
		--actionsInExecution;
		break;
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE:
	{
		++actionsInExecution;
		auto f = [&actionsInExecution, &action]()
		{
			action();
			--actionsInExecution;
		};
		m_ptnEngine.addJob(f);
		break;
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED:
	{
		++actionsInExecution;
		auto job = [&actionsInExecution, &action]()
		{
			action();
			--actionsInExecution;
		};
		auto t = thread(job);
		t.detach();
		break;
	}
	}
}

bool Place::isOnEnterActionInExecution() const
{
	return m_onEnterActionsInExecution > 0;
}

void Place::blockStartingOnEnterActions(const bool value)
{
	m_blockStartingOnEnterActions = value;
}

PlaceProperties Place::placeProperties() const
{
	PlaceProperties placeProperties;
	placeProperties.name = m_name;
	placeProperties.onEnterActionFunctionName = m_onEnterActionName;
	placeProperties.onExitActionFunctionName = m_onExitActionName;
	placeProperties.initialNumberOfTokens = m_numberOfTokens;
	placeProperties.input = m_isInputPlace;
	return placeProperties;
}

} // namespace ptne
