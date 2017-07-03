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

#include <iostream>
#include "Controller/Controller.h"

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

	using namespace std;
	m_pPetriNet = move(
		PtrPetriNet(new MenuStateMachine(shared_from_this()))
		);
}

void Controller::showMainMenu()
{
	if(std::shared_ptr<MainMenuView> mainMenu = m_mainMenu.lock())
	{
		mainMenu->showMenu();
	}
}

void Controller::selectCallsOption()
{
	if(std::shared_ptr<MainMenuView> mainMenu = m_mainMenu.lock())
	{
		mainMenu->select(0);
	}
}

void Controller::selectMessagesOption()
{
	if(std::shared_ptr<MainMenuView> mainMenu = m_mainMenu.lock())
	{
		mainMenu->select(1);
	}
}

void Controller::showCallsMenu()
{
	if(std::shared_ptr<CallList> callLog = m_callLog.lock())
	{
		if(std::shared_ptr<CallLogView> callLogView = m_callLogView.lock())
		{
			callLogView->viewCallLog(*callLog);
		}
	}
}

void Controller::showMessageMenu()
{
	if(std::shared_ptr<MessageList> messageList = m_messageList.lock())
	{
		if(std::shared_ptr<MessagesMenuView> messagesMenu = m_messagesMenu.lock())
		{
			messagesMenu->showMessagesList(*messageList, m_messageSelected);
		}
	}
}

void Controller::selectNextMessage()
{
	if(std::shared_ptr<MessageList> messageList = m_messageList.lock())
	{
		++m_messageSelected;
		if(m_messageSelected == messageList->size())
		{
			m_messageSelected = 0;
		}
	}
}

void Controller::showMessage()
{
	if(std::shared_ptr<MessageList> messageList = m_messageList.lock())
	{
		if(std::shared_ptr<MessagesMenuView> messagesMenu = m_messagesMenu.lock())
		{
			messagesMenu->displayMessage(messageList->getItem(m_messageSelected));
		}
	}
}

void Controller::setMessageList(std::shared_ptr<MessageList> messageList)
{
	m_messageList = messageList;
}

void Controller::setCallLogView(std::shared_ptr<CallLogView> callLogView)
{
	m_callLogView = callLogView;
}

void Controller::setMainMenuView(std::shared_ptr<MainMenuView> mainMenu)
{
	m_mainMenu = mainMenu;
}

void Controller::setMessagesMenuView(std::shared_ptr<MessagesMenuView> messagesMenu)
{
	m_messagesMenu = messagesMenu;
}

void Controller::setCallLog(std::shared_ptr<CallList> callLog)
{
	m_callLog = callLog;
}





