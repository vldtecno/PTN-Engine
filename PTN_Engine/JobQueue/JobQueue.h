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

#include "PTN_Engine/PTN_Engine.h"
#include <atomic>
#include <deque>
#include <memory>
#include <mutex>

namespace ptne
{

class JobQueue
{
public:
	JobQueue() = default;
	~JobQueue() = default;
	JobQueue(const JobQueue &) = delete;
	JobQueue(JobQueue &&) = delete;
	JobQueue &operator=(const JobQueue &) = delete;
	JobQueue &operator=(JobQueue &&) = delete;

	//! Activate the job queue.
	void activate();

	//! Deactivate the job queue.
	void inactivate();

	/*!
	 * \brief Informs on whether the job queue is running or not.
	 * \return True if the job queue is active.
	 */
	bool isActive() const;

	/*!
	 * \brief Adds a job to the job queue.
	 * \param Function/job to be executed.
	 */
	void addJob(const ActionFunction &actionFunction);

	/*!
	 * \brief Run the job queue. Should be executed in its own thread.
	 */
	void run();

private:
	/*!
	 * \brief Launch the job queue.
	 */
	void launch();

	//! Whether the job queue is active or not.
	std::atomic<bool> m_isJobQueueActive = true;

	//! Whether the job queue is running or not.
	std::atomic<bool> m_isJobQueueRunning = false;

	//! Flag to order the abort of the processing of the job queue.
	std::atomic<bool> m_abortJobQueue = false;

	//! The collection of jobs to be executed.
	std::deque<ActionFunction> m_jobQueue;

	//! Mutex to synchronize the job queue operations.
	std::mutex m_jobQueueMutex;
};

} // namespace ptne
