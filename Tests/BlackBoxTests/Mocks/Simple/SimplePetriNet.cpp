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

#include "Mocks/Simple/SimplePetriNet.h"
#include "Mocks/Simple/SimpleController.h"

using namespace std;
using namespace ptne;

SimplePetriNet::SimplePetriNet(SimpleController &controller)
: PTN_Engine(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP)
{
	createPlace({ .name="P1",
				  .onEnterAction=bind(&SimpleController::collectThreadId, &controller),
				  .input=true });
	createPlace({ .name="P2" });

	createTransition({ .name = "T1",
					   .activationArcs = { ArcProperties{ .placeName = "P1" } },
					   .destinationArcs = { ArcProperties{ .placeName = "P2" } } });
	createTransition({ .name = "T2", .activationArcs = { ArcProperties{ .placeName = "P2" } } });
}

void SimplePetriNet::addExecuteP1()
{
	incrementInputPlace("P1");
}

size_t SimplePetriNet::getNumberOfTokens(const string &placeName) const
{
	return ptne::PTN_Engine::getNumberOfTokens(placeName);
}
