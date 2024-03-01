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

#include "Mocks/Dispatcher/FreeChoicePetriNet.h"

using namespace ptne;
using namespace std;

FreeChoicePetriNet::FreeChoicePetriNet(Dispatcher &dispatcher,
									   PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
{
	// Places
	PlaceProperties placeProperties;
	placeProperties.name = "InputWaitPackage";
	placeProperties.input = true;

	createPlace({ .name="InputWaitPackage",
				  .input=true });

    createPlace({ .name="WaitPackage",
                  .initialNumberOfTokens=1,
                  .onEnterAction=bind(&Dispatcher::actionWaitPackage, &dispatcher),
                  .onExitAction=bind(&Dispatcher::onLeaveWaitPackage, &dispatcher) });

	createPlace({ .name="ChannelA",
				  .onEnterAction=bind(&Dispatcher::actionChannelA, &dispatcher),
				  .onExitAction=bind(&Dispatcher::onLeaveChannelA, &dispatcher) });

	createPlace({ .name="CounterA"});

	createPlace({ .name="ChannelB",
				  .onEnterAction=bind(&Dispatcher::actionChannelB, &dispatcher),
				  .onExitAction=bind(&Dispatcher::onLeaveChannelB, &dispatcher) });

	createPlace({ .name="CounterB" });

	createPlace({ .name="PackageCounter" });


	// Transitions

	// Use A
	createTransition({ .name = "T1",
					   .activationArcs = { ArcProperties{ .placeName = "InputWaitPackage" },
										   ArcProperties{ .placeName = "WaitPackage" } },
					   .destinationArcs = { ArcProperties{ .placeName = "ChannelA" },
											ArcProperties{ .placeName = "PackageCounter" } } });

	// Use B
	createTransition({ .name = "T2",
					   .activationArcs = { ArcProperties{ .placeName = "InputWaitPackage" },
										   ArcProperties{ .placeName = "WaitPackage" } },
					   .destinationArcs = { ArcProperties{ .placeName = "ChannelB" },
											ArcProperties{ .placeName = "PackageCounter" } } });

	// From A back to waiting a package
	createTransition({ .name = "T3",
					   .activationArcs = { ArcProperties{ .placeName = "ChannelA" } },
					   .destinationArcs = { ArcProperties{ .placeName = "WaitPackage" },
											ArcProperties{ .placeName = "CounterA" } } });

	// From B back to waiting a package
	createTransition({ .name = "T4",
					   .activationArcs = { ArcProperties{ .placeName = "ChannelB" } },
					   .destinationArcs = { ArcProperties{ .placeName = "WaitPackage" },
											ArcProperties{ .placeName = "CounterB" } } });

	// Reset Counters
	createTransition({ .name = "T5",
					   .activationArcs = { ArcProperties{ .placeName = "PackageCounter" } },
					   .additionalConditions={ bind(&Dispatcher::resetCounter, &dispatcher) } });

	createTransition({ .name = "T6",
					   .activationArcs = { ArcProperties{ .placeName = "CounterA" } },
					   .additionalConditions={ bind(&Dispatcher::resetCounter, &dispatcher) } });

	createTransition({ .name = "T7",
					   .activationArcs = { ArcProperties{ .placeName = "CounterB" } },
					   .additionalConditions={ bind(&Dispatcher::resetCounter, &dispatcher) } });
}

void FreeChoicePetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
}

bool FreeChoicePetriNet::stillRunning() const
{
	return PTN_Engine::isEventLoopRunning();
}

void FreeChoicePetriNet::stop()
{
	PTN_Engine::stop();
}

void FreeChoicePetriNet::execute()
{
	PTN_Engine::execute();
}
