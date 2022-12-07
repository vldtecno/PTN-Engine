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

#include "Mocks/Dispatcher/Dispatcher.h"
#include "Mocks/Dispatcher/FreeChoicePetriNet.h"
#include "Mocks/Dispatcher/InhibitedPetriNet.h"
#include "Mocks/Dispatcher/RoundRobinPetriNet.h"
#include "Mocks/Dispatcher/WeightedPetriNet.h"

#include <iostream>

using namespace std;


Dispatcher::Dispatcher()
: m_pPetriNet(nullptr)
, m_resetCounter(false)
, m_isWaitingPackage(true)
, m_isUsingChannelA(false)
, m_isUsingChannelB(false)
, m_isChannelASelected(true)
, m_isChannelBSelected(false)
{
	setRoundRobinMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::actionWaitPackage()
{
	m_isWaitingPackage = true;
}

void Dispatcher::actionChannelA()
{
	m_isUsingChannelA = true;
}

void Dispatcher::actionChannelB()
{
	m_isUsingChannelB = true;
}

void Dispatcher::actionSelectA()
{
	m_isChannelASelected = true;
}

void Dispatcher::actionSelectB()
{
	m_isChannelBSelected = true;
}

void Dispatcher::onLeaveWaitPackage()
{
	m_isWaitingPackage = false;
}

void Dispatcher::onLeaveChannelA()
{
	m_isUsingChannelA = false;
}

void Dispatcher::onLeaveChannelB()
{
	m_isUsingChannelB = false;
}

void Dispatcher::onLeaveSelectChannelA()
{
	m_isChannelASelected = false;
}

void Dispatcher::onLeaveSelectChannelB()
{
	m_isChannelBSelected = false;
}

bool Dispatcher::resetCounter() const
{
	return m_resetCounter;
}

bool Dispatcher::stillRunning() const
{
	if (m_pPetriNet)
	{
		return m_pPetriNet->stillRunning();
	}
	return false;
}

void Dispatcher::setResetCounter(const bool resetCounter)
{
	m_resetCounter = resetCounter;
}

void Dispatcher::dispatch()
{
	if (m_pPetriNet)
	{
		m_pPetriNet->dispatch();
	}
}

void Dispatcher::setRoundRobinMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	m_pPetriNet = make_unique<RoundRobinPetriNet>(*this, actionsThreadOption);
}

void Dispatcher::setFreeChoiceMode(ptne::PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	m_pPetriNet = make_unique<FreeChoicePetriNet>(*this, actionsThreadOption);
}

void Dispatcher::setWeightedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	m_pPetriNet = make_unique<WeightedPetriNet>(actionsThreadOption);
}

void Dispatcher::setInhibitedPN(ptne::PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	m_pPetriNet = make_unique<InhibitedPetriNet>(actionsThreadOption);
}

void Dispatcher::stop()
{
	m_pPetriNet->stop();
}
