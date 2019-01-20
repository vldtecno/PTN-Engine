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

#include "Mocks/RegisteredFunctions/RegisteredFunctionsPN.h"
#include "Mocks/RegisteredFunctions/Controller.h"

using namespace std;

RegisteredFunctionsPN::RegisteredFunctionsPN(shared_ptr<Controller> controller)
: ptne::PTN_Engine()
, m_controller(controller)
{
}

void RegisteredFunctionsPN::registerCallbacks()
{
	auto controller = lockWeakPtr(m_controller);
	registerAction("actionPlace1", bind(&Controller::actionPlace1, controller));
	registerAction("actionPlace2", bind(&Controller::actionPlace2, controller));
	registerCondition("externalCondition1",
					  make_shared<ControllerFireCondition>(controller, &Controller::externalCondition1));
	registerCondition("externalCondition2",
					  make_shared<ControllerFireCondition>(controller, &Controller::externalCondition2));
	registerCondition("externalCondition3",
					  make_shared<ControllerFireCondition>(controller, &Controller::externalCondition3));
}

void RegisteredFunctionsPN::createPetriNetStructure()
{
	createPlace("P0", 0, true);
	createPlace("P1", 0, "actionPlace1", false);
	createPlace("P2", 0, "actionPlace2", false);

	createTransition({ "P0" }, { "P1" }, {}, vector<string>{ "externalCondition1", "externalCondition2" });
	createTransition({ "P0" }, { "P2" }, {}, vector<string>{ "externalCondition3" });
}

void RegisteredFunctionsPN::addExecuteP0()
{
	incrementInputPlace("P0");
	execute();
}
