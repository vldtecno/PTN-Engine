/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018-2024 Eduardo Valgôde
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
#include "PTN_Engine/PTN_Engine.h"

using namespace std;
using namespace ptne;

RegisteredFunctionsPN::RegisteredFunctionsPN(Controller &controller,
											 PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
, m_controller(controller)
{
}

void RegisteredFunctionsPN::registerCallbacks()
{
	registerAction("actionPlace1", bind(&Controller::actionPlace1, &m_controller));
	registerAction("actionPlace2", bind(&Controller::actionPlace2, &m_controller));
	registerCondition("externalCondition1", bind(&Controller::externalCondition1, &m_controller));
	registerCondition("externalCondition2", bind(&Controller::externalCondition2, &m_controller));
	registerCondition("externalCondition3", bind(&Controller::externalCondition3, &m_controller));
}

void RegisteredFunctionsPN::createPetriNetStructure()
{
	createPlace({ .name="P0",
				  .input=true });
	createPlace({ .name="P1",
				  .onEnterActionFunctionName="actionPlace1" });
	createPlace({ .name="P2",
				  .onEnterActionFunctionName="actionPlace2" });

	createTransition({ .name = "T1",
					   .activationArcs = { ArcProperties{ .placeName = "P0" } },
					   .destinationArcs = { ArcProperties{ .placeName = "P1" } },
					   .additionalConditionsNames = { "externalCondition1", "externalCondition2" } });
	createTransition({ .name = "T2",
					   .activationArcs = { ArcProperties{ .placeName = "P0" } },
					   .destinationArcs = { ArcProperties{ .placeName = "P2" } },
					   .additionalConditionsNames = {  "externalCondition3" }  });
}

void RegisteredFunctionsPN::addExecuteP0()
{
	incrementInputPlace("P0");
	execute();
}

size_t RegisteredFunctionsPN::getNumberOfTokens(const string &placeName) const
{
	return PTN_Engine::getNumberOfTokens(placeName);
}
