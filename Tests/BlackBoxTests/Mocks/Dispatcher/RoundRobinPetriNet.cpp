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

#include "Mocks/Dispatcher/RoundRobinPetriNet.h"
#include "PTN_Engine/Place.h"


using namespace ptne;
using namespace std;

Dispatcher::RoundRobinPetriNet::RoundRobinPetriNet(shared_ptr<Dispatcher> ptrDispatcher)
: PTN_Engine{}
{
	// Places
	createPlace("InputWaitPackage", 0, true);

	createPlace("WaitPackage", 1, bind(&Dispatcher::actionWaitPackage, ptrDispatcher),
				bind(&Dispatcher::onLeaveWaitPackage, ptrDispatcher));

	createPlace("ChannelA", 0, bind(&Dispatcher::actionChannelA, ptrDispatcher),
				bind(&Dispatcher::onLeaveChannelA, ptrDispatcher));

	createPlace("ChannelB", 0, bind(&Dispatcher::actionChannelB, ptrDispatcher),
				bind(&Dispatcher::onLeaveChannelB, ptrDispatcher));

	createPlace("SelectA", 1, bind(&Dispatcher::actionSelectA, ptrDispatcher),
				bind(&Dispatcher::onLeaveSelectChannelA, ptrDispatcher));

	createPlace("SelectB", 0, bind(&Dispatcher::actionSelectB, ptrDispatcher),
				bind(&Dispatcher::onLeaveSelectChannelB, ptrDispatcher));

	createPlace("PackageCounter", 0);


	// Transitions

	// Use A
	createTransition({ "InputWaitPackage", "WaitPackage", "SelectA" }, // activation
					 { "ChannelA", "PackageCounter" } // destination
	);

	// Use B
	createTransition({ "InputWaitPackage", "WaitPackage", "SelectB" }, // activation
					 { "ChannelB", "PackageCounter" } // destination
	);

	// Switch to A
	createTransition({ "ChannelA" }, // activation
					 { "SelectB", "WaitPackage" } // destination
	);

	// Switch to B
	createTransition({ "ChannelB" }, // activation
					 { "SelectA", "WaitPackage" } // destination
	);

	// Reset Counter
	createTransition({ "PackageCounter" }, // activation
					 {}, // destination
					 { bind(&Dispatcher::resetCounter, ptrDispatcher) });
}

void Dispatcher::RoundRobinPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
