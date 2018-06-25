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

using namespace std;

Controller::RegisteredFunctionsPN::RegisteredFunctionsPN(shared_ptr<Controller> controller):
	::ptne::PTN_Engine()
{
	registerAction("collectThreadId", make_shared<ControllerAction2>(controller, &Controller::collectThreadId));
	//registerCondition("someCondition", make_shared<ControllerCondition>(controller, &SimpleController::someCondition));

	createPlace("P1", 0, "collectThreadId", true);
	createPlace("P2", 0);

	//createTransition({ "P1" }, { "P2" }, "someCondition");
	createTransition({ "P1" }, { "P2" });
	createTransition({ "P2" }, { });

}

void Controller::RegisteredFunctionsPN::addExecuteP1()
{
	incrementInputPlace("P1");
	execute();
}
