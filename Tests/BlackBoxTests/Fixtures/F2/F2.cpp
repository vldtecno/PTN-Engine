/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018 Eduardo Valgôde
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

#include "Fixtures/F2/F2.h"
#include "Mocks/RegisteredFunctions/Controller.h"
#include "Mocks/RegisteredFunctions/RegisteredFunctionsPN.h"

using namespace std;

F2::F2():
	m_dispatcher(make_shared<Dispatcher>())
{
	m_dispatcher->initialize();
}

void F2::testRegisteredMethods()
{
	std::shared_ptr<Controller> controller = make_shared<Controller>();
	controller->initialize();

	controller->startExecution();

	//TODO get a better way to test
	EXPECT_EQ(1, controller->m_petriNet->getNumberOfTokens("P1"));
	EXPECT_EQ(0, controller->m_petriNet->getNumberOfTokens("P2"));

	EXPECT_EQ("actionPlace1", controller->getSomeString());
}

void F2::testRepeatedRegisteredMethods()
{
	std::shared_ptr<Controller> controller = make_shared<Controller>();
	controller->initialize();

	ASSERT_THROW(
	controller->initialize(), ptne::PTN_Exception);
}

