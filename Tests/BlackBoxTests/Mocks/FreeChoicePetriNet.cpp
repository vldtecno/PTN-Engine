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

#include "Mocks/FreeChoicePetriNet.h"
#include "PTN_Engine/Place.h"


using namespace ptne;
using namespace std;

Dispatcher::FreeChoicePetriNet::FreeChoicePetriNet(shared_ptr<Dispatcher> ptrDispatcher):
	PTN_Engine{}
{

	//Places
	addPlace("InputWaitPackage", 0, nullptr, nullptr, true);

	addPlace("WaitPackage",1,
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::actionWaitPackage),
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::onLeaveWaitPackage));

	addPlace("ChannelA", 0,
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::actionChannelA),
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::onLeaveChannelA));

	addPlace("CounterA", 0,	nullptr, nullptr);

	addPlace("ChannelB", 0,
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::actionChannelB),
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::onLeaveChannelB));

	addPlace("CounterB", 0,	nullptr, nullptr);

	addPlace("PackageCounter", 0, nullptr, nullptr);


	//Transitions

	//Use A
	createTransition(
			{"InputWaitPackage", "WaitPackage"}, //activation
			{ "ChannelA", "PackageCounter"}, //destination
			{} //additional conditions
			);

	//Use B
	createTransition(
			{"InputWaitPackage", "WaitPackage"}, //activation
			{"ChannelB", "PackageCounter"}, //destination
			{} //additional conditions
			);

	//From A back to waiting a package
	createTransition(
			{"ChannelA"}, //activation
			{"WaitPackage", "CounterA"}, //destination
			{} //additional conditions
			);

	//From B back to waiting a package
	createTransition(
			{"ChannelB"}, //activation
			{"WaitPackage", "CounterB"}, //destination
			{} //additional conditions
			);

	//Reset Counters
	createTransition(
			{"PackageCounter"}, //activation
			{}, //destination
			//additional conditions
			{make_shared<DispatcherFireCondition>(ptrDispatcher,&Dispatcher::resetCounter)}
			);

	createTransition(
			{"CounterA"}, //activation
			{}, //destination
			//additional conditions
			{make_shared<DispatcherFireCondition>(ptrDispatcher,&Dispatcher::resetCounter)}
			);

	createTransition(
			{"CounterB"}, //activation
			{}, //destination
			//additional conditions
			{make_shared<DispatcherFireCondition>(ptrDispatcher,&Dispatcher::resetCounter)}
			);

}

void Dispatcher::FreeChoicePetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
