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

#include "Controller/MenuStateMachine.h"

using namespace ptne;

Controller::MenuStateMachine::MenuStateMachine(std::shared_ptr<Controller> ptrMenuController):
	PTN_Engine{}
{
	using namespace std;

	registerAction("showMainMenu",make_shared<ControllerAction>(ptrMenuController, &Controller::showMainMenu));
	registerAction("selectMessagesOption",make_shared<ControllerAction>(ptrMenuController, &Controller::selectMessagesOption));
	registerAction("selectCallsOption",make_shared<ControllerAction>(ptrMenuController, &Controller::selectCallsOption));
	registerAction("showCallsMenu",make_shared<ControllerAction>(ptrMenuController, &Controller::showCallsMenu));
	registerAction("showMessageMenu",make_shared<ControllerAction>(ptrMenuController, &Controller::showMessageMenu));
	registerAction("selectNextMessage",make_shared<ControllerAction>(ptrMenuController, &Controller::selectNextMessage));
	registerAction("showMessage",make_shared<ControllerAction>(ptrMenuController, &Controller::showMessage));

}

void Controller::MenuStateMachine::pressA()
{
	incrementInputPlace("InputA");
	execute();
}

void Controller::MenuStateMachine::pressB()
{
	incrementInputPlace("InputB");
	execute();
}

void Controller::MenuStateMachine::pressC()
{
	incrementInputPlace("InputC");
	execute();
}
