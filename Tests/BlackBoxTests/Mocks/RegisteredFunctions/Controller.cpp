/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018-2024 Eduardo Valgôde
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

using namespace std;

mutex Controller::s_mut;

Controller::Controller()
: m_menuStateMachine(*this)
{
}

Controller::~Controller()
{
}

void Controller::initialize()
{
	m_menuStateMachine.registerCallbacks();
	m_menuStateMachine.createPetriNetStructure();
}

void Controller::startExecution()
{
	m_menuStateMachine.addExecuteP0();
}

string Controller::getSomeString() const
{
	lock_guard g(s_mut);
	return m_someString;
}

void Controller::actionPlace1()
{
	lock_guard g(s_mut);
	m_someString = "actionPlace1";
}

void Controller::actionPlace2()
{
	lock_guard g(s_mut);
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

size_t Controller::getNumberOfTokens(const string &placeName) const
{
	return m_menuStateMachine.getNumberOfTokens(placeName);
}

void Controller::stop()
{
	m_menuStateMachine.stop();
}
