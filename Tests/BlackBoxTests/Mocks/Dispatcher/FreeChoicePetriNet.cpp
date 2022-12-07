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

#include "Mocks/Dispatcher/FreeChoicePetriNet.h"

using namespace ptne;
using namespace std;

FreeChoicePetriNet::FreeChoicePetriNet(Dispatcher &dispatcher,
									   PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
{
	// Places
	createPlace("InputWaitPackage", 0, true);

	createPlace("WaitPackage", 1, bind(&Dispatcher::actionWaitPackage, &dispatcher),
				bind(&Dispatcher::onLeaveWaitPackage, &dispatcher));

	createPlace("ChannelA", 0, bind(&Dispatcher::actionChannelA, &dispatcher),
				bind(&Dispatcher::onLeaveChannelA, &dispatcher));

	createPlace("CounterA", 0);

	createPlace("ChannelB", 0, bind(&Dispatcher::actionChannelB, &dispatcher),
				bind(&Dispatcher::onLeaveChannelB, &dispatcher));

	createPlace("CounterB", 0);

	createPlace("PackageCounter", 0);


	// Transitions

	// Use A
	createTransition({ "InputWaitPackage", "WaitPackage" }, // activation
					 { "ChannelA", "PackageCounter" } // destination
	);

	// Use B
	createTransition({ "InputWaitPackage", "WaitPackage" }, // activation
					 { "ChannelB", "PackageCounter" } // destination
	);

	// From A back to waiting a package
	createTransition({ "ChannelA" }, // activation
					 { "WaitPackage", "CounterA" } // destination
	);

	// From B back to waiting a package
	createTransition({ "ChannelB" }, // activation
					 { "WaitPackage", "CounterB" } // destination
	);

	// Reset Counters
	createTransition({ "PackageCounter" }, // activation
					 {}, // destination
					 // additional conditions
					 { bind(&Dispatcher::resetCounter, &dispatcher) });

	createTransition({ "CounterA" }, // activation
					 {}, // destination
					 // additional conditions
					 { bind(&Dispatcher::resetCounter, &dispatcher) });

	createTransition({ "CounterB" }, // activation
					 {}, // destination
					 // additional conditions
					 { bind(&Dispatcher::resetCounter, &dispatcher) });
}

void FreeChoicePetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}

bool FreeChoicePetriNet::stillRunning() const
{
	return PTN_Engine::isEventLoopRunning();
}

void FreeChoicePetriNet::stop()
{
	PTN_Engine::stop();
}
