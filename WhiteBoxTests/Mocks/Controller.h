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

#include "PTN_Engine/ActivationCondition.h"
#include "PTN_Engine/Action.h"

//! Sample controller.
/*!
 * Sample controller.
 */
class Controller final
{
public:

	//! Constructor.
	Controller();

	//! On enter action.
	void onEnter();

	//! On exit action.
	void onExit();

	//! How many times enter was called.
	size_t getEnterCounter() const;

	//! How many times exit was called.
	size_t getExitCounter() const;

	//! If something is active.
	bool activationCondition() const;

	//! Set if the activation condition should return true or false.
	void setFireCondition(const bool);

private:

	//! Counter of times entered.
	size_t m_enterCounter;

	//! Counter of times exited.
	size_t m_exitCounter;

	//! Activation condition.
	bool m_canFire;

};

template class ptne::Action<Controller>;
using ControllerAction = ptne::Action<Controller>;

template class ptne::ActivationCondition<Controller>;
using ControllerFireCondition = ptne::ActivationCondition<Controller>;



