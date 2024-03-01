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

#include "Mocks/Dispatcher/RoundRobinPetriNet.h"

using namespace ptne;
using namespace std;

RoundRobinPetriNet::RoundRobinPetriNet(Dispatcher &dispatcher,
									   PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
{
	// Places
	createPlace({ .name="InputWaitPackage",
				  .input=true });

	createPlace({ .name="WaitPackage",
				  .initialNumberOfTokens=1,
				  .onEnterAction=bind_front(&Dispatcher::actionWaitPackage, &dispatcher),
				  .onExitAction=bind_front(&Dispatcher::onLeaveWaitPackage, &dispatcher) });

	createPlace({ .name="ChannelA",
				  .onEnterAction=bind_front(&Dispatcher::actionChannelA, &dispatcher),
				  .onExitAction=bind_front(&Dispatcher::onLeaveChannelA, &dispatcher) });

	createPlace({ .name="ChannelB",
				  .onEnterAction=bind_front(&Dispatcher::actionChannelB, &dispatcher),
				  .onExitAction=bind_front(&Dispatcher::onLeaveChannelB, &dispatcher) });

	createPlace({ .name="SelectA",
				  .initialNumberOfTokens=1,
				  .onEnterAction=bind_front(&Dispatcher::actionSelectA, &dispatcher),
				  .onExitAction=bind_front(&Dispatcher::onLeaveSelectChannelA, &dispatcher) });

	createPlace({ .name="SelectB",
				  .onEnterAction=bind_front(&Dispatcher::actionSelectB, &dispatcher),
				  .onExitAction=bind_front(&Dispatcher::onLeaveSelectChannelB, &dispatcher) });

	createPlace({ .name="PackageCounter" });


	// Transitions

	// Use A
	createTransition({ .name = "T1",
					   .activationArcs = { ArcProperties{ .placeName = "InputWaitPackage" },
										   ArcProperties{ .placeName = "WaitPackage" },
										   ArcProperties{ .placeName = "SelectA" } },
					   .destinationArcs = { ArcProperties{ .placeName = "ChannelA" },
											ArcProperties{ .placeName = "PackageCounter" } } });

	// Use B
	createTransition({ .name = "T2",
					   .activationArcs = { ArcProperties{ .placeName = "InputWaitPackage" },
										   ArcProperties{ .placeName = "WaitPackage" },
										   ArcProperties{ .placeName = "SelectB" } },
					   .destinationArcs = { ArcProperties{ .placeName = "ChannelB" },
											ArcProperties{ .placeName = "PackageCounter" } } });

	// Switch to A
	createTransition({ .name = "T3",
					   .activationArcs = { ArcProperties{ .placeName = "ChannelA" } },
					   .destinationArcs = { ArcProperties{ .placeName = "SelectB" },
											ArcProperties{ .placeName = "WaitPackage" } } });

	// Switch to B
	createTransition({ .name = "T4",
					   .activationArcs = { ArcProperties{ .placeName = "ChannelB" } },
					   .destinationArcs = { ArcProperties{ .placeName = "SelectA" },
											ArcProperties{ .placeName = "WaitPackage" } } });

	// Reset Counter
	createTransition({ .name = "T5",
					   .activationArcs = { ArcProperties{ .placeName = "PackageCounter" } },
					   .additionalConditions={ bind_front(&Dispatcher::resetCounter, &dispatcher) } });
}

void RoundRobinPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
}

bool RoundRobinPetriNet::stillRunning() const
{
	return PTN_Engine::isEventLoopRunning();
}

void RoundRobinPetriNet::stop()
{
	PTN_Engine::stop();
}

void RoundRobinPetriNet::execute()
{
	PTN_Engine::execute();
}
