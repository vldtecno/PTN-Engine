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

#include "Mocks/RoundRobinPetriNet.h"
#include "PTN_Engine/Place.h"


using namespace ptne;
using namespace std;

Dispatcher::RoundRobinPetriNet::RoundRobinPetriNet(shared_ptr<Dispatcher> ptrDispatcher):
	PTN_Engine{}
{


	//Places
	addPlace("InputWaitPackage", 0, true);

	addPlace("WaitPackage",1,
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::actionWaitPackage),
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::onLeaveWaitPackage));

	addPlace("ChannelA", 0,
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::actionChannelA),
			make_shared<DispatcherAction>(ptrDispatcher, &Dispatcher::onLeaveChannelA));

	addPlace("ChannelB", 0,
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::actionChannelB),
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::onLeaveChannelB));

	addPlace("SelectA", 1,
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::actionSelectA),
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::onLeaveSelectChannelA));

	addPlace("SelectB", 0,
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::actionSelectB),
			make_shared<DispatcherAction>(ptrDispatcher,&Dispatcher::onLeaveSelectChannelB));

	addPlace("PackageCounter", 0);


	//Transitions

	//Use A
	createTransition(
			{"InputWaitPackage", "WaitPackage", "SelectA"}, //activation
			{ "ChannelA", "PackageCounter"} //destination
			);

	//Use B
	createTransition(
			{"InputWaitPackage", "WaitPackage", "SelectB"}, //activation
			{"ChannelB", "PackageCounter"}//destination
			);

	//Switch to A
	createTransition({"ChannelA"}, //activation
			{"SelectB", "WaitPackage"} //destination
			);

	//Switch to B
	createTransition(
			{"ChannelB"}, //activation
			{"SelectA", "WaitPackage"} //destination
			);

	//Reset Counter
	createTransition(
			{"PackageCounter"}, //activation
			{}, //destination
			{make_shared<DispatcherFireCondition>(ptrDispatcher,&Dispatcher::resetCounter)}
			);

}

void Dispatcher::RoundRobinPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
