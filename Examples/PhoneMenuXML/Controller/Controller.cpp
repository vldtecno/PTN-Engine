/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019 Eduardo Valgôde
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
#include "ImportExport/ExportFactory.h"
#include "ImportExport/ImportFactory.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/IImporter.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"

#include <iostream>

using namespace std;

Controller::Controller()
: m_petriNet()
, m_messageSelected(0)
{
}

Controller::~Controller()
{
}

void Controller::initialize()
{
	m_petriNet.registerAction("showMainMenu", bind(&Controller::showMainMenu, shared_from_this()));
	m_petriNet.registerAction("selectMessagesOption", bind(&Controller::selectMessagesOption, shared_from_this()));
	m_petriNet.registerAction("selectCallsOption", bind(&Controller::selectCallsOption, shared_from_this()));
	m_petriNet.registerAction("showCallsMenu", bind(&Controller::showCallsMenu, shared_from_this()));
	m_petriNet.registerAction("showMessageMenu", bind(&Controller::showMessageMenu, shared_from_this()));
	m_petriNet.registerAction("selectNextMessage", bind(&Controller::selectNextMessage, shared_from_this()));
	m_petriNet.registerAction("showMessage", bind(&Controller::showMessage, shared_from_this()));
	m_petriNet.registerCondition("DummyFunction", bind(&Controller::justReturnTrue, shared_from_this()));
}

void Controller::pressA()
{
	m_petriNet.incrementInputPlace("InputA");
	m_petriNet.execute();
}

void Controller::pressB()
{
	m_petriNet.incrementInputPlace("InputB");
	m_petriNet.execute();
}

void Controller::pressC()
{
	m_petriNet.incrementInputPlace("InputC");
	m_petriNet.execute();
}

void Controller::exportStateMachine(const string &filePath) const
{
	unique_ptr<ptne::IExporter> xmlExporter = ptne::ExportFactory::createXMLExporter();
	m_petriNet.export_(*xmlExporter);
	xmlExporter->saveFile(filePath);
}

void Controller::importStateMachine(const string &filePath)
{
	unique_ptr<ptne::IImporter> xmlImporter = ptne::ImportFactory::createXMLImporter(filePath);
	m_petriNet.import(*xmlImporter);
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
	if (m_messageSelected == messageList->size())
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

bool Controller::justReturnTrue()
{
	return true;
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
