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
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/IImporter.h"
#include "PTN_Engine/ImportExport/ExportFactory.h"
#include "PTN_Engine/ImportExport/ImportFactory.h"
#include <iostream>

using namespace std;

Controller::Controller()
: m_petriNet(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP)
, m_messageSelected(0)
{
	m_petriNet.registerAction("showMainMenu", bind(&Controller::showMainMenu, this));
	m_petriNet.registerAction("selectMessagesOption", bind(&Controller::selectMessagesOption, this));
	m_petriNet.registerAction("selectCallsOption", bind(&Controller::selectCallsOption, this));
	m_petriNet.registerAction("showCallsMenu", bind(&Controller::showCallsMenu, this));
	m_petriNet.registerAction("showMessageMenu", bind(&Controller::showMessageMenu, this));
	m_petriNet.registerAction("selectNextMessage", bind(&Controller::selectNextMessage, this));
	m_petriNet.registerAction("showMessage", bind(&Controller::showMessage, this));
	m_petriNet.registerCondition("DummyFunction", bind(&Controller::justReturnTrue, this));
}

Controller::~Controller()
{
}

void Controller::pressA()
{
	m_petriNet.incrementInputPlace("InputA");
}

void Controller::pressB()
{
	m_petriNet.incrementInputPlace("InputB");
}

void Controller::pressC()
{
	m_petriNet.incrementInputPlace("InputC");
}

Controller::MessageList &Controller::messageList()
{
	return m_messageList;
}

Controller::CallList &Controller::callLog()
{
	return m_callLog;
}

void Controller::execute()
{
	m_petriNet.execute();
}

void Controller::stop()
{
	m_petriNet.stop();
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

bool Controller::justReturnTrue()
{
	return true;
}
