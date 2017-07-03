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
#include "PTN_Engine/IActionFunctor.h"
#include "PTN_Engine/PTN_Exception.h"

namespace ptne
{
	Place::Place(const size_t initialNumberOfTokens,
			ActionFunctorPtr onEnterEventHandler,
			ActionFunctorPtr onExitEventHandler,
			const bool input):
		m_numberOfTokens{initialNumberOfTokens},
		m_onEnterAction{onEnterEventHandler},
		m_onExitAction{onExitEventHandler},
		m_isInputPlace{input}
	{}

	Place::~Place()
	{}

	void Place::enterPlace(const size_t tokens)
	{
		increaseNumberOfTokens(tokens);
		if(m_onEnterAction != nullptr)
		{
			(*m_onEnterAction)();
		}
	}

	void Place::exitPlace(const size_t tokens)
	{
		decreaseNumberOfTokens(tokens);
		if(m_onExitAction != nullptr)
		{
			(*m_onExitAction)();
		}
	}

	void Place::increaseNumberOfTokens(const size_t tokens)
	{
		if(tokens <= 0)
		{
			throw PTN_Exception("Number of tokens must greater than 0");
		}
		m_numberOfTokens += tokens;
	}

	void Place::decreaseNumberOfTokens(const size_t tokens)
	{
		if(tokens <= 0)
		{
			throw PTN_Exception("Number of tokens must greater than 0");
		}
		if(m_numberOfTokens < tokens )
		{
			throw PTN_Exception("Not enough tokens in the place.");
		}
		m_numberOfTokens -= tokens;
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

}
