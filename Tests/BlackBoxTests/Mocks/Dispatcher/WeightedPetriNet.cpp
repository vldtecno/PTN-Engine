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

#include "Mocks/Dispatcher/WeightedPetriNet.h"

using namespace ptne;
using namespace std;

WeightedPetriNet::WeightedPetriNet(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
: PTN_Engine(actionsThreadOption)
{
	// Places
	createPlace("InputWaitPackage", 0, true);
	createPlace("WaitPackage", 0);
	createPlace("ChannelA", 0);
	createPlace("ChannelB", 0);


	// Transitions

	createTransition({ "InputWaitPackage" }, // activation
					 vector<size_t>{ 1 }, { "WaitPackage" }, // destination
					 vector<size_t>{ 1 });

	createTransition({ "WaitPackage" }, // activation
					 vector<size_t>{ 3 }, { "ChannelA", "ChannelB" }, // destination
					 vector<size_t>{ 4, 10 });
}

void WeightedPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
}

bool WeightedPetriNet::stillRunning() const
{
	return PTN_Engine::isEventLoopRunning();
}

void WeightedPetriNet::stop()
{
	PTN_Engine::stop();
}

void WeightedPetriNet::execute()
{
	PTN_Engine::execute();
}
