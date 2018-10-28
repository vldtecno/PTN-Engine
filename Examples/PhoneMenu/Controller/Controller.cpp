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

#include "Controller/Controller.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"

#include <iostream>

using namespace std;

Controller::Controller():
	m_pPetriNet{nullptr},
	m_messageSelected(0)
{
}

Controller::~Controller()
{}

void Controller::pressA()
{
	if(m_pPetriNet)
	{
		m_pPetriNet->pressA();
	}
}

void Controller::pressB()
{
	if(m_pPetriNet)
	{
		m_pPetriNet->pressB();
	}
}

void Controller::pressC()
{
	if(m_pPetriNet)
	{
		m_pPetriNet->pressC();
	}
}

void Controller::initialize()
{
	if(m_pPetriNet)
	{
		return;
	}

	m_pPetriNet = PtrPetriNet(new MenuStateMachine(shared_from_this()));
}

void Controller::showMainMenu()
{
	shared_ptr<MainMenuView> mainMenu = lockWeakPtr(m_mainMenu);
	mainMenu->showMenu();
}

void Controller::selectCallsOption()
{
	shared_ptr<MainMenuView> mainMenu = lockWeakPtr(m_mainMenu);
	mainMenu->select(0);
}

void Controller::selectMessagesOption()
{
	shared_ptr<MainMenuView> mainMenu = lockWeakPtr(m_mainMenu);
	mainMenu->select(1);
}

void Controller::showCallsMenu()
{
	shared_ptr<CallList> callLog = lockWeakPtr(m_callLog);
	shared_ptr<CallLogView> callLogView = lockWeakPtr(m_callLogView);
	callLogView->viewCallLog(*callLog);
}

void Controller::showMessageMenu()
{
	shared_ptr<MessageList> messageList = lockWeakPtr(m_messageList);
	shared_ptr<MessagesMenuView> messagesMenu = lockWeakPtr(m_messagesMenu);
	messagesMenu->showMessagesList(*messageList, m_messageSelected);
}

void Controller::selectNextMessage()
{
	shared_ptr<MessageList> messageList = lockWeakPtr(m_messageList);
	++m_messageSelected;
	if(m_messageSelected == messageList->size())
	{
		m_messageSelected = 0;
	}
}

void Controller::showMessage()
{
	shared_ptr<MessageList> messageList = lockWeakPtr(m_messageList);
	shared_ptr<MessagesMenuView> messagesMenu = lockWeakPtr(m_messagesMenu);
	messagesMenu->displayMessage(messageList->getItem(m_messageSelected));
}

void Controller::setMessageList(shared_ptr<MessageList> messageList)
{
	m_messageList = messageList;
}

void Controller::setCallLogView(shared_ptr<CallLogView> callLogView)
{
	m_callLogView = callLogView;
}

void Controller::setMainMenuView(shared_ptr<MainMenuView> mainMenu)
{
	m_mainMenu = mainMenu;
}

void Controller::setMessagesMenuView(shared_ptr<MessagesMenuView> messagesMenu)
{
	m_messagesMenu = messagesMenu;
}

void Controller::setCallLog(shared_ptr<CallList> callLog)
{
	m_callLog = callLog;
}
