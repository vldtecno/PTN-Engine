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

#include "Mocks/InhibitedPetriNet.h"
#include "PTN_Engine/Place.h"


using namespace ptne;
using namespace std;

Dispatcher::InhibitedPetriNet::InhibitedPetriNet(shared_ptr<Dispatcher> ptrDispatcher):
	PTN_Engine{}
{

	//Places
	addPlace("InputWaitPackage", 0, nullptr, nullptr, true);

	addPlace("P1",1, nullptr, nullptr);
	addPlace("P2",1, nullptr, nullptr);
	addPlace("P3",1, nullptr, nullptr);
	addPlace("P4",0, nullptr, nullptr);
	addPlace("P5",0, nullptr, nullptr);

	//Transitions


	createTransition(
			{"InputWaitPackage", "P1", "P3"}, //activation
			{"P4"} //destination
			);


	createTransition(
			{"P2"}, //activation
			{"P5"}, //destination
			{"P3"} //inhibitor arc
			);


	createTransition(
			{"InputWaitPackage", "P4"}, //activation
			{"P1", "P3"} //destination
			);


	createTransition(
			{"P5"}, //activation
			{"P2"}, //destination
			{"P4"} //inhibitor arc
			);

}

void Dispatcher::InhibitedPetriNet::dispatch()
{
	incrementInputPlace("InputWaitPackage");
	execute();
}
