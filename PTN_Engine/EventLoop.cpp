/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2023 Eduardo Valgôde
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

#include "PTN_Engine/EventLoop.h"
#include "PTN_Engine/IPTN_EngineEL.h"
#include "PTN_Engine/PTN_Exception.h"
#include <thread>

namespace ptne
{
using namespace std;

EventLoop::EventLoop(IPTN_EngineEL &ptnEngineInternal)
: m_ptnEngine(ptnEngineInternal)
, m_sleepDuration(100ms)
{
}

EventLoop::~EventLoop()
{
}

bool EventLoop::isRunning() const
{
	return m_eventLoopThreadRunning;
}

void EventLoop::stop() noexcept
{
	if (!isRunning())
	{
		return;
	}

	if (m_eventLoopThread.get_stop_token().stop_possible())
	{
		m_eventLoopThread.request_stop();
		m_barrier->arrive_and_wait();
	}
}

void EventLoop::start(const bool log, ostream &o)
{
	if (isRunning())
	{
		throw PTN_Exception("Cannot call execute while the event loop is still running.");
	}
	if (m_ptnEngine.getActionsThreadOption() == PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD)
	{
		while (m_ptnEngine.executeInt(log, o))
			;
	}
	else
	{
		m_barrier = make_unique<barrier<>>(2);
		m_eventLoopThreadRunning = true;
		m_eventLoopThread = jthread(bind_front(&EventLoop::run, this), log, ref(o));
	}
}

void EventLoop::notifyNewEvent()
{
	unique_lock<mutex> eventNotifierGuard(m_eventNotifierMutex);
	m_eventNotifier.notify_all();
}

void EventLoop::setSleepDuration(const SleepDuration sleepDuration)
{
	unique_lock<shared_mutex> lock(m_sleepDurationMutex);
	// TODO validate input
	m_sleepDuration = sleepDuration;
}

EventLoop::SleepDuration EventLoop::getSleepDuration() const
{
	shared_lock<shared_mutex> lock(m_sleepDurationMutex);
	return m_sleepDuration;
}

void EventLoop::run(std::stop_token stopToken, const bool log, ostream &o)
{
	while (!stopToken.stop_requested())
	{
		if (!m_ptnEngine.executeInt(log, o))
		{
			shared_lock<shared_mutex> lock(m_sleepDurationMutex);
			unique_lock<mutex> eventNotifierGuard(m_eventNotifierMutex);
			m_eventNotifier.wait_for(eventNotifierGuard, m_sleepDuration,
									 [this] { return m_ptnEngine.getNewInputReceived(); });
		}
	}
	m_eventLoopThreadRunning = false;
	m_barrier->arrive_and_wait();
}
} // namespace ptne
