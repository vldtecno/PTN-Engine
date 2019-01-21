/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
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
#include "PTN_Engine/Place.h"


using namespace ptne;
using namespace std;

Dispatcher::FreeChoicePetriNet::FreeChoicePetriNet(shared_ptr<Dispatcher> ptrDispatcher)
: PTN_Engine{}
{

	// Places
	createPlace("InputWaitPackage", 0, true);

	createPlace("WaitPackage", 1, bind(&Dispatcher::actionWaitPackage, ptrDispatcher),
				bind(&Dispatcher::onLeaveWaitPackage, ptrDispatcher));

	createPlace("ChannelA", 0, bind(&Dispatcher::actionChannelA, ptrDispatcher),
				bind(&Dispatcher::onLeaveChannelA, ptrDispatcher));

	createPlace("CounterA", 0);

	createPlace("ChannelB", 0, bind(&Dispatcher::actionChannelB, ptrDispatcher),
				bind(&Dispatcher::onLeaveChannelB, ptrDispatcher));

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
					 { bind(&Dispatcher::resetCounter, ptrDispatcher) });

	createTransition({ "CounterA" }, // activation
					 {}, // destination
					 // additional conditions
					 { bind(&Dispatcher::resetCounter, ptrDispatcher) });

	createTransition({ "CounterB" }, // activation
					 {}, // destination
					 // additional conditions
					 { bind(&Dispatcher::resetCounter, ptrDispatcher) });
}

void Dispatcher::FreeChoicePetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
