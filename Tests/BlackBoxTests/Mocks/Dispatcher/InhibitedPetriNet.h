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

#pragma once

#include "Mocks/Dispatcher/Dispatcher.h"
#include "Mocks/Dispatcher/IDispatcherPetriNet.h"
#include "PTN_Engine/PTN_Engine.h"

//! Implements PTN Engine net with a Petri net that uses the inhibitor arc.
/*!
 * The behaviour is defined in the constructor.
 */
class Dispatcher::InhibitedPetriNet : public IDispatcherPetriNet, private ptne::PTN_Engine
{
	// For testing purposes.
	friend class FixturePetriNet;

public:
	/*!
	 * Constructor.
	 * \param ptrDispatcher Shared pointer to the dispatcher.
	 */
	InhibitedPetriNet(std::shared_ptr<Dispatcher> ptrDispatcher);

	//! Trigger the dispatch process.
	void dispatch() override;
};
