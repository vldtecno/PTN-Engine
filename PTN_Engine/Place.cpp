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

#include "PTN_Engine/Place.h"
#include "PTN_Engine/PTN_Engine.h"
#include "limits.h"
#include <future>
#include <string>

namespace ptne
{
using namespace std;

Place::Place(PTN_Engine &parent,
             const size_t initialNumberOfTokens,
             ActionFunction onEnterEventHandler,
             ActionFunction onExitEventHandler,
             const bool input)
: m_ptnEngine(parent)
, m_onEnterAction(onEnterEventHandler)
, m_onExitAction(onExitEventHandler)
, m_numberOfTokens(initialNumberOfTokens)
, m_isInputPlace(input)
{
}

Place::Place(PTN_Engine &parent,
             const size_t initialNumberOfTokens,
             const string &onEnterActionName,
             ActionFunction onEnterAction,
             const string &onExitActionName,
             ActionFunction onExitAction,
             const bool input)
: m_ptnEngine(parent)
, m_onEnterActionName(onEnterActionName)
, m_onEnterAction(onEnterAction)
, m_onExitActionName(onExitActionName)
, m_onExitAction(onExitAction)
, m_numberOfTokens(initialNumberOfTokens)
, m_isInputPlace(input)
{
}

Place::~Place()
{
}

void Place::enterPlace(const size_t tokens)
{
	increaseNumberOfTokens(tokens);
	if (m_onEnterAction == nullptr)
	{
		return;
	}
	executeAction(m_onEnterAction);
}

void Place::exitPlace(const size_t tokens)
{
	decreaseNumberOfTokens(tokens);
	if (m_onExitAction == nullptr)
	{
		return;
	}
	executeAction(m_onExitAction);
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
	m_numberOfTokens = tokens;
}

size_t Place::getNumberOfTokens() const
{
	return m_numberOfTokens;
}

bool Place::isInputPlace() const
{
	return m_isInputPlace;
}

const string Place::getOnEnterActionName() const
{
	return m_onEnterActionName;
}

const string Place::getOnExitActionName() const
{
	return m_onExitActionName;
}

void Place::executeAction(const ActionFunction &action)
{
	switch (m_ptnEngine.getActionsThreadOption())
	{
	default:
	{
		throw std::runtime_error("Invalid configuration");
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP:
	{
		action();
		break;
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE:
	{
		m_ptnEngine.addJob(action);
		break;
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED:
	{
		auto t = std::thread(action);
		t.detach();
		break;
	}
	}
}

Place::NullTokensException::NullTokensException()
: PTN_Exception("Number of tokens must greater than 0")
{
}

Place::OverflowException::OverflowException(const size_t tooBig)
: PTN_Exception("Cannot add " + to_string(tooBig) + " tokens to place without overflowing.")
{
}

Place::NotEnoughTokensException::NotEnoughTokensException()
: PTN_Exception("Not enough tokens in the place.")
{
}

} // namespace ptne
