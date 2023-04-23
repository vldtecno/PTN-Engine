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

#include "Mocks/Dispatcher/RoundRobinPetriNet.h"

using namespace ptne;
using namespace std;

RoundRobinPetriNet::RoundRobinPetriNet(Dispatcher &dispatcher,
									   PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
{
	// Places
	createPlace("InputWaitPackage", 0, true);

	createPlace("WaitPackage", 1, bind(&Dispatcher::actionWaitPackage, &dispatcher),
				bind(&Dispatcher::onLeaveWaitPackage, &dispatcher));

	createPlace("ChannelA", 0, bind(&Dispatcher::actionChannelA, &dispatcher),
				bind(&Dispatcher::onLeaveChannelA, &dispatcher));

	createPlace("ChannelB", 0, bind(&Dispatcher::actionChannelB, &dispatcher),
				bind(&Dispatcher::onLeaveChannelB, &dispatcher));

	createPlace("SelectA", 1, bind(&Dispatcher::actionSelectA, &dispatcher),
				bind(&Dispatcher::onLeaveSelectChannelA, &dispatcher));

	createPlace("SelectB", 0, bind(&Dispatcher::actionSelectB, &dispatcher),
				bind(&Dispatcher::onLeaveSelectChannelB, &dispatcher));

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
					 { bind(&Dispatcher::resetCounter, &dispatcher) });
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
