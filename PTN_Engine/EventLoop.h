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

#pragma once

#include <atomic>
#include <barrier>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>

namespace ptne
{

class IPTN_EngineEL;
class Transition;

/*!
 * \brief The EventLoop class manages an event loop thread, where all the events are passed to the petri net
 * engine.
 */
class EventLoop
{
public:
	using SleepDuration = std::chrono::duration<long, std::ratio<1, 1000>>;

	/*!
	 * \brief EventLoop
	 * \param ptnEngineInternal
	 */
	explicit EventLoop(IPTN_EngineEL &ptnEngineInternal);

	~EventLoop();

	EventLoop(const EventLoop &) = delete;
	EventLoop(EventLoop &&) = delete;
	EventLoop &operator=(const EventLoop &) = delete;
	EventLoop &operator=(EventLoop &&) = delete;

	/*!
	 * \brief Inform if the event loop is running.
	 * \return True if the event loop thread is running.
	 */
	bool isRunning() const;

	/*!
	 * \brief Order the event loop to stop.
	 */
	void stop() noexcept;

	/*!
	 * \brief Start the event loop thread.
	 * \param log Whether to log or not.
	 * \param o The output stream to write the log messages to.
	 */
	void start(const bool log, std::ostream &o);

	/*!
	 * \brief Notify the event loop thread of a new event.
	 */
	void notifyNewEvent();

	/*!
	 * \brief Set the event loop watchdog timer period.
	 * \param sleepTime The event loop watchdog timer period.
	 */
	void setSleepDuration(const SleepDuration sleepDuration);

	/*!
	 * \brief Get the event loop watchdog timer period.
	 * \return The event loop watchdog timer period.
	 */
	SleepDuration getSleepDuration() const;

private:
	/*!
	 * \brief Event loop function.
	 * \param log Flag to turn on logging on or off.
	 * \param o Log where to write log messages.
	 */
	void run(std::stop_token stopToken, const bool log, std::ostream &o);

	//! Reference to the petri net engine.
	IPTN_EngineEL &m_ptnEngine;

	//! Flag if the event loop thread is running.
	std::atomic<bool> m_eventLoopThreadRunning = false;

	//! Mutex protecting the event notifier condition variable m_eventNotifier.
	mutable std::mutex m_eventNotifierMutex;

	//! Condition variable to wake up the event loop thread when some event happens.
	std::condition_variable m_eventNotifier;

	//! Mutex protecting m_sleepDuration.
	mutable std::shared_mutex m_sleepDurationMutex;

	//! While idle, watchdog timer period.
	SleepDuration m_sleepDuration;

	//! Event loop thread.
	std::jthread m_eventLoopThread;

	//! Barrier to synchronize the event loop thread.
	std::barrier<> m_barrier;
};

} // namespace ptne
