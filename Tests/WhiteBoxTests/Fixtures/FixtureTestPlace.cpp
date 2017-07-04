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

#include "Fixtures/FixtureTestPlace.h"

using namespace ptne;

FixtureTestPlace::FixtureTestPlace():
	m_controller{std::make_shared<Controller>(Controller{})},
	m_place{
		size_t(0),
		std::make_shared<ControllerAction>(m_controller, &Controller::onEnter),
		std::make_shared<ControllerAction>(m_controller, &Controller::onExit)
		}
{}

void FixtureTestPlace::enterPlace()
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t enterCounter = m_controller->getEnterCounter();

	m_place.enterPlace();

	EXPECT_EQ(numberOfTokens+1, m_place.getNumberOfTokens());
	EXPECT_EQ(enterCounter+1, m_controller->getEnterCounter());
}

void FixtureTestPlace::exitPlace()
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t exitCounter = m_controller->getExitCounter();
	bool shouldFire = numberOfTokens > 0;

	size_t expectedNumberOfTokens = numberOfTokens;
	size_t expectedExitCounter = exitCounter;
	if(shouldFire)
	{
		expectedNumberOfTokens = expectedNumberOfTokens-1;
		expectedExitCounter = exitCounter+1;
	}

	m_place.exitPlace();


	EXPECT_EQ(expectedNumberOfTokens, m_place.getNumberOfTokens());
	EXPECT_EQ(expectedExitCounter, m_controller->getExitCounter());
}

void FixtureTestPlace::enterPlace(const size_t tokens)
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t enterCounter = m_controller->getEnterCounter();

	m_place.enterPlace(tokens);

	EXPECT_EQ(numberOfTokens+tokens, m_place.getNumberOfTokens());
	EXPECT_EQ(enterCounter+1, m_controller->getEnterCounter());
}

void FixtureTestPlace::exitPlace(const size_t tokens)
{
	size_t numberOfTokens = m_place.getNumberOfTokens();
	size_t exitCounter = m_controller->getExitCounter();
	bool shouldFire = numberOfTokens >= tokens;

	size_t expectedNumberOfTokens = numberOfTokens;
	size_t expectedExitCounter = exitCounter;
	if(shouldFire)
	{
		expectedNumberOfTokens = expectedNumberOfTokens-tokens;
		expectedExitCounter = exitCounter+1;
	}

	m_place.exitPlace(tokens);

	EXPECT_EQ(expectedNumberOfTokens, m_place.getNumberOfTokens());
	EXPECT_EQ(expectedExitCounter, m_controller->getExitCounter());
}

void FixtureTestPlace::inputPlace()
{
	ptne::Place inputPlace(
			size_t(0),
			std::make_shared<ControllerAction>(m_controller, &Controller::onEnter),
			std::make_shared<ControllerAction>(m_controller, &Controller::onExit)
			);
	EXPECT_EQ(false, inputPlace.isInputPlace());
}


