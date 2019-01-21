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

#include "PTN_Engine/PTN_Engine/Place.h"
#include "limits.h"
#include <string>

namespace ptne
{
using namespace std;

Place::Place(const size_t initialNumberOfTokens,
			 ActionFunction onEnterEventHandler,
			 ActionFunction onExitEventHandler,
			 const bool input)
: m_onEnterAction{ onEnterEventHandler }
, m_onExitAction{ onExitEventHandler }
, m_numberOfTokens{ initialNumberOfTokens }
, m_isInputPlace{ input }
{
}

Place::Place(const size_t initialNumberOfTokens,
			 const string &onEnterActionName,
			 ActionFunction onEnterAction,
			 const string &onExitActionName,
			 ActionFunction onExitAction,
			 const bool input)
: m_onEnterActionName(onEnterActionName)
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
	if (m_onEnterAction != nullptr)
	{
		m_onEnterAction();
	}
}

void Place::exitPlace(const size_t tokens)
{
	decreaseNumberOfTokens(tokens);
	if (m_onExitAction != nullptr)
	{
		m_onExitAction();
	}
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
