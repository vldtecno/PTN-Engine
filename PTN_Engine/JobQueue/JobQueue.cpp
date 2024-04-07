/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2023-2024 Eduardo Valgôde
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

#include "PTN_Engine/JobQueue/JobQueue.h"
#include <thread>

namespace ptne
{
using namespace std;

JobQueue::~JobQueue() = default;
JobQueue::JobQueue() = default;

void JobQueue::activate()
{
	if (m_isJobQueueActive)
	{
		return;
	}
	m_isJobQueueActive = true;
	launch();
}

void JobQueue::deactivate()
{
	if (!m_isJobQueueActive)
	{
		return;
	}
	if (m_workerThread.get_stop_token().stop_possible())
	{
		m_workerThread.request_stop();
	}
	m_isJobQueueActive = false;
}

bool JobQueue::isActive() const
{
	return m_isJobQueueActive;
}

void JobQueue::launch()
{
	lock_guard l(m_jobQueueMutex);
	if (!m_isJobQueueRunning && !m_jobQueue.empty() && m_isJobQueueActive)
	{
		m_isJobQueueRunning = true;
		m_workerThread = jthread(bind_front(&JobQueue::run, this));
	}
}

void JobQueue::run(stop_token stopToken)
{
	unique_lock l(m_jobQueueMutex);
	while (!m_jobQueue.empty() && !stopToken.stop_requested())
	{
		ActionFunction job = m_jobQueue.back();
		m_jobQueue.pop_back();
		l.unlock();
		job();
		l.lock();
	}
	m_isJobQueueRunning = false;
}

void JobQueue::addJob(const ActionFunction &actionFunction)
{
	{
		lock_guard l(m_jobQueueMutex);
		m_jobQueue.push_front(actionFunction);
	}
	launch();
}

} // namespace ptne
