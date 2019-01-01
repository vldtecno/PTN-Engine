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

#include "Mocks/RegisteredFunctions/Controller.h"
#include "Mocks/RegisteredFunctions/RegisteredFunctionsPN.h"
#include <thread>

using namespace std;

Controller::Controller()
: m_petriNet(nullptr)
{
}

void Controller::initialize()
{
	if (!m_petriNet)
	{
		m_petriNet = make_unique<RegisteredFunctionsPN>(shared_from_this());
	}
	m_petriNet->registerCallbacks();
	m_petriNet->createPetriNetStructure();
}

void Controller::startExecution()
{
	m_petriNet->addExecuteP0();
}

string Controller::getSomeString() const
{
	return m_someString;
}

void Controller::actionPlace1()
{
	m_someString = "actionPlace1";
}

void Controller::actionPlace2()
{
	m_someString = "actionPlace2";
}

bool Controller::externalCondition1() const
{
	return true;
}

bool Controller::externalCondition2() const
{
	return true;
}

bool Controller::externalCondition3() const
{
	return false;
}
