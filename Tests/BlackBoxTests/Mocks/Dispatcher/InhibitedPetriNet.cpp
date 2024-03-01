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

#include "Mocks/Dispatcher/InhibitedPetriNet.h"

using namespace ptne;
using namespace std;

InhibitedPetriNet::InhibitedPetriNet(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
{
	// Places
	createPlace({ .name="InputWaitPackage",
				  .input=true });

	createPlace({ .name="P1",
				  .initialNumberOfTokens=1});
	createPlace({ .name="P2",
				  .initialNumberOfTokens=1});
	createPlace({ .name="P3",
				  .initialNumberOfTokens=1});
	createPlace({ .name="P4",
				  .initialNumberOfTokens=0});
	createPlace({ .name="P5",
				  .initialNumberOfTokens=0});

	// Transitions
	createTransition( {.name = "T1",
					   .activationArcs = { ArcProperties{ .placeName = "InputWaitPackage" },
										   ArcProperties{ .placeName = "P1" },
										   ArcProperties{ .placeName = "P3" } },
					   .destinationArcs = {ArcProperties{ .placeName = "P4" } } });

	createTransition({ .name = "T2",
					   .activationArcs = { ArcProperties{ .placeName = "P2" } },
					   .destinationArcs = { ArcProperties{ .placeName = "P5" } },
					   .inhibitorArcs = { ArcProperties{ .placeName = "P3" } } });

	createTransition({ .name = "T3",
					   .activationArcs = { ArcProperties{ .placeName = "InputWaitPackage" },
										   ArcProperties{ .placeName = "P4" } },
					   .destinationArcs = { ArcProperties{ .placeName = "P1" },
											ArcProperties{ .placeName = "P3" } } });

	createTransition({ .name = "T4",
					   .activationArcs = { ArcProperties{ .placeName = "P5" } },
					   .destinationArcs = { ArcProperties{ .placeName = "P2" } },
					   .inhibitorArcs = { ArcProperties{ .placeName = "P4" } } });
}

void InhibitedPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
}

bool InhibitedPetriNet::stillRunning() const
{
	return PTN_Engine::isEventLoopRunning();
}

void InhibitedPetriNet::stop()
{
	PTN_Engine::stop();
}

void InhibitedPetriNet::execute()
{
	PTN_Engine::execute();
}
