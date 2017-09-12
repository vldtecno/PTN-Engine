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

	addPlace("WaitPackage",0,
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::actionWaitPackage),
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::onLeaveWaitPackage));

	addPlace("ChannelA", 0,
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::actionChannelA),
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::onLeaveChannelA));

	addPlace("ChannelB", 0,
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::actionChannelB),
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::onLeaveChannelB));


	//Transitions

	//Use A
	createTransition(
			{"InputWaitPackage"}, //activation
			{1},
			{"WaitPackage"}, //destination
			{3},
			{} //additional conditions
			);

	//Use B
	createTransition(
			{"WaitPackage"}, //activation
			{2},
			{"ChannelA"}, //destination
			{1},
			{} //additional conditions
			);

	//From A back to waiting a package
	createTransition(
			{"WaitPackage"}, //activation
			{4},
			{"ChannelB"}, //destination
			{1},
			{} //additional conditions
			);

	//Finished
	createTransition(
			{"ChannelA", "ChannelB"}, //activation
			{}, //destination
			{} //additional conditions
			);



}

void Dispatcher::WeightedPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
