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

#include "Fixtures/FixtureTestPlace.h"

using namespace ptne;

FixtureTestPlace::FixtureTestPlace()
: m_controller(std::make_shared<Controller>())
, m_place(m_ptnEngine,
          size_t(0),
          std::bind(&Controller::onEnter, m_controller),
          std::bind(&Controller::onExit, m_controller))
{
    m_ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
}

void FixtureTestPlace::enterPlace()
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t enterCounter = m_controller->getEnterCounter();

	m_place.enterPlace();

	EXPECT_EQ(numberOfTokens + 1, m_place.getNumberOfTokens());
	EXPECT_EQ(enterCounter + 1, m_controller->getEnterCounter());
}

void FixtureTestPlace::exitPlace()
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t exitCounter = m_controller->getExitCounter();
	bool shouldFire = numberOfTokens > 0;

	size_t expectedNumberOfTokens = numberOfTokens;
	size_t expectedExitCounter = exitCounter;
	if (shouldFire)
	{
		expectedNumberOfTokens = expectedNumberOfTokens - 1;
		expectedExitCounter = exitCounter + 1;

		m_place.exitPlace();
	}
	else
	{
		ASSERT_THROW(m_place.exitPlace(), PTN_Exception);
	}

	EXPECT_EQ(expectedNumberOfTokens, m_place.getNumberOfTokens());
	EXPECT_EQ(expectedExitCounter, m_controller->getExitCounter());
}

void FixtureTestPlace::inputPlace()
{
	ptne::Place inputPlace(m_ptnEngine, size_t(0), std::bind(&Controller::onEnter, m_controller),
						   std::bind(&Controller::onExit, m_controller));
	EXPECT_EQ(false, inputPlace.isInputPlace());
}

void FixtureTestPlace::enterPlace(const size_t tokens)
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t enterCounter = m_controller->getEnterCounter();

	m_place.enterPlace(tokens);

	EXPECT_EQ(numberOfTokens + tokens, m_place.getNumberOfTokens());
	EXPECT_EQ(enterCounter + 1, m_controller->getEnterCounter());
}

void FixtureTestPlace::exitPlace(const size_t tokens)
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t exitCounter = m_controller->getExitCounter();
	bool shouldFire = numberOfTokens >= tokens;

	size_t expectedNumberOfTokens = numberOfTokens;
	size_t expectedExitCounter = exitCounter;
	if (shouldFire)
	{
		expectedNumberOfTokens = expectedNumberOfTokens - tokens;
		expectedExitCounter = exitCounter + 1;

		m_place.exitPlace(tokens);
	}
	else
	{
		ASSERT_THROW(m_place.exitPlace(tokens), PTN_Exception);
	}

	EXPECT_EQ(expectedNumberOfTokens, m_place.getNumberOfTokens());
	EXPECT_EQ(expectedExitCounter, m_controller->getExitCounter());
}
