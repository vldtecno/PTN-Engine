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

#include "Controller/Controller.h"

using namespace std;

Controller::Controller(MainMenuView &mainMenuView,
                       MessagesMenuView &messagesMenuView,
                       CallLogView &callLogView,
                       MessageList &messageList,
                       CallList &callLog)
: m_mainMenuView(mainMenuView)
, m_messagesMenuView(messagesMenuView)
, m_callLogView(callLogView)
, m_messageList(messageList)
, m_callLog(callLog)
{
}

Controller::~Controller() = default;

void Controller::pressA()
{
	m_menuStateMachine->pressA();
}

void Controller::pressB()
{
	m_menuStateMachine->pressB();
}

void Controller::pressC()
{
	m_menuStateMachine->pressC();
}

void Controller::showMainMenu() const
{
	m_mainMenuView.showMenu();
}

void Controller::selectCallsOption()
{
	m_mainMenuView.select(0);
}

void Controller::selectMessagesOption()
{
	m_mainMenuView.select(1);
}

void Controller::showCallsMenu() const
{
	m_callLogView.viewCallLog();
}

void Controller::showMessageMenu() const
{
	m_messagesMenuView.showMessagesList(m_messageSelected);
}

void Controller::selectNextMessage()
{
	++m_messageSelected;
	if (m_messageSelected == m_messageList.size())
	{
		m_messageSelected = 0;
	}
}

void Controller::showMessage() const
{
	m_messagesMenuView.displayMessage(m_messageList.getItem(m_messageSelected));
}

void Controller::execute()
{
	if (m_menuStateMachine == nullptr)
	{
		return;
	}
	m_menuStateMachine->execute();
}

void Controller::stop()
{
	m_menuStateMachine->stop();
}

void Controller::setStateMachine(shared_ptr<IMenuStateMachine> stateMachine)
{
	if (m_menuStateMachine != nullptr)
	{
		m_menuStateMachine->stop();
	}
	stateMachine->setControllerFunctions(*this);
	m_menuStateMachine = stateMachine;
}

bool Controller::justReturnTrue() const
{
	return true;
}
