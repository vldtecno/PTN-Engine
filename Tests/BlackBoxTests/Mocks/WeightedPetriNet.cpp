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

#include "Mocks/WeightedPetriNet.h"
#include "PTN_Engine/Place.h"


using namespace ptne;
using namespace std;

Dispatcher::WeightedPetriNet::WeightedPetriNet(shared_ptr<Dispatcher> ptrDispatcher):
	PTN_Engine{}
{

	//Places
	addPlace("InputWaitPackage", 0, nullptr, nullptr, true);
	addPlace("WaitPackage",0, nullptr, nullptr);
	addPlace("ChannelA", 0, nullptr, nullptr);
	addPlace("ChannelB", 0, nullptr, nullptr);


	//Transitions

	createTransition(
			{"InputWaitPackage"}, //activation
			{1},
			{"WaitPackage"}, //destination
			{1});

	createTransition(
			{"WaitPackage"}, //activation
			{3},
			{"ChannelA", "ChannelB"}, //destination
			{4, 10});

}

void Dispatcher::WeightedPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
