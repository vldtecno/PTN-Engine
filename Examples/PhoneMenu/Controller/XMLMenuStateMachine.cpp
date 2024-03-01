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

#include "Controller/XMLMenuStateMachine.h"
#include "Controller/Controller.h"
#include "PTN_Engine/ImportExport/IFileExporter.h"
#include "PTN_Engine/ImportExport/IFileImporter.h"
#include "PTN_Engine/ImportExport/ExporterFactory.h"
#include "PTN_Engine/ImportExport/ImporterFactory.h"

using namespace ptne;
using namespace std;

void XMLMenuStateMachine::setControllerFunctions(Controller &menuController)
{
	registerAction("showMainMenu", bind_front(&Controller::showMainMenu, &menuController));
	registerAction("selectMessagesOption", bind_front(&Controller::selectMessagesOption, &menuController));
	registerAction("selectCallsOption", bind_front(&Controller::selectCallsOption, &menuController));
	registerAction("showCallsMenu", bind_front(&Controller::showCallsMenu, &menuController));
	registerAction("showMessageMenu", bind_front(&Controller::showMessageMenu, &menuController));
	registerAction("selectNextMessage", bind_front(&Controller::selectNextMessage, &menuController));
	registerAction("showMessage", bind_front(&Controller::showMessage, &menuController));
	registerCondition("DummyFunction", bind_front(&Controller::justReturnTrue, &menuController));
}

void XMLMenuStateMachine::pressA()
{
	incrementInputPlace("InputA");
}

void XMLMenuStateMachine::pressB()
{
	incrementInputPlace("InputB");
}

void XMLMenuStateMachine::pressC()
{
	incrementInputPlace("InputC");
}

void XMLMenuStateMachine::exportStateMachine(const string &filePath) const
{
	unique_ptr<IFileExporter> xmlExporter = ExporterFactory::createXMLFileExporter();
	xmlExporter->_export(*this, filePath);
}

void XMLMenuStateMachine::importStateMachine(const string &filePath)
{
	unique_ptr<IFileImporter> xmlImporter = ImporterFactory::createXMLFileImporter();
	clearNet();
	xmlImporter->_import(filePath, *this);
}
