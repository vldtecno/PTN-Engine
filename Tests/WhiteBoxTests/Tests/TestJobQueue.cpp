/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2024 Eduardo Valgôde
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
#include <gtest/gtest.h>

using namespace ptne;
using namespace std;

class JobQueue_Obj : public testing::Test
{
public:
	JobQueue jobQueue;
};

TEST_F(JobQueue_Obj, activate_activates_the_job_queue)
{
	EXPECT_TRUE(jobQueue.isActive());
	jobQueue.deactivate();
	EXPECT_FALSE(jobQueue.isActive());
	jobQueue.activate();
	EXPECT_TRUE(jobQueue.isActive());
}

TEST_F(JobQueue_Obj, add_job_when_deactivated_runs_job_when_again_active)
{
	bool executed = false;
	auto f = [&executed]() { executed = true; };

	jobQueue.deactivate();
	EXPECT_FALSE(jobQueue.isActive());
	jobQueue.addJob(f);
	EXPECT_FALSE(jobQueue.isActive());
	EXPECT_FALSE(executed);
	jobQueue.activate();
	this_thread::sleep_for(20ms);
	EXPECT_TRUE(executed);
}

TEST_F(JobQueue_Obj, run_add_job_runs_job_immediately_if_active)
{
	bool executed = false;
	auto f = [&executed]() { executed = true; };

	EXPECT_TRUE(jobQueue.isActive());
	jobQueue.addJob(f);
	EXPECT_TRUE(jobQueue.isActive());
	this_thread::sleep_for(20ms);
	EXPECT_TRUE(executed);
}
