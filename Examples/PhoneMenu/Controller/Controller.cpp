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

#include "Controller/Controller.h"
#include <iostream>

using namespace std;

Controller::Controller()
: m_petriNet(*this)
, m_messageSelected(0)
{
}

Controller::~Controller()
{
}

void Controller::pressA()
{
	m_petriNet.pressA();
}

void Controller::pressB()
{
	m_petriNet.pressB();
}

void Controller::pressC()
{
	m_petriNet.pressC();
}

Controller::MessageList &Controller::messageList()
{
	return m_messageList;
}

Controller::CallList &Controller::callLog()
{
	return m_callLog;
}

void Controller::showMainMenu() const
{
	m_mainMenu.showMenu();
}

void Controller::selectCallsOption()
{
	m_mainMenu.select(0);
}

void Controller::selectMessagesOption()
{
	m_mainMenu.select(1);
}

void Controller::showCallsMenu()
{
	m_callLogView.viewCallLog(m_callLog);
}

void Controller::showMessageMenu()
{
	m_messagesMenu.showMessagesList(m_messageList, m_messageSelected);
}

void Controller::selectNextMessage()
{
	++m_messageSelected;
	if (m_messageSelected == m_messageList.size())
	{
		m_messageSelected = 0;
	}
}

void Controller::showMessage()
{
	m_messagesMenu.displayMessage(m_messageList.getItem(m_messageSelected));
}

void Controller::execute()
{
	m_petriNet.execute();
}

void Controller::stop()
{
	m_petriNet.stop();
}
