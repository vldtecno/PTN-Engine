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

#include "PTN_Engine/EventLoop.h"
#include "PTN_Engine/PTN_EngineImp.h"
#include <gtest/gtest.h>

using namespace ptne;
using namespace std;

class PTNEnginImpObj : public testing::Test
{
public:
	PTN_EngineImp ptnEngineImp = PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	EventLoop eventLoop = EventLoop(ptnEngineImp);
};

TEST_F(PTNEnginImpObj, isRunning_returns_if_event_loop_is_running)
{
	EXPECT_FALSE(eventLoop.isRunning());
}

TEST_F(PTNEnginImpObj, stop_stops_the_event_loop)
{
	EXPECT_FALSE(eventLoop.isRunning());
	eventLoop.stop();
	EXPECT_FALSE(eventLoop.isRunning());
}

TEST_F(PTNEnginImpObj, start_starts_the_event_loop)
{
	EXPECT_FALSE(eventLoop.isRunning());
	eventLoop.start(false, std::cout);
	EXPECT_TRUE(eventLoop.isRunning());
	eventLoop.stop();
}

TEST(EventLoop, start_single_threaded_stops_immediately_after_start)
{
	PTN_EngineImp ptnEngineImp(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);
	EventLoop eventLoop(ptnEngineImp);
	EXPECT_FALSE(eventLoop.isRunning());
	eventLoop.start(false, std::cout);
	EXPECT_FALSE(eventLoop.isRunning());
}

TEST_F(PTNEnginImpObj, start_again_throws)
{
	EXPECT_FALSE(eventLoop.isRunning());
	eventLoop.start(false, std::cout);
	ASSERT_THROW(eventLoop.start(false, std::cout), PTN_Exception);
	eventLoop.stop();
}

// void notifyNewEvent();
TEST_F(PTNEnginImpObj, notifyNewEvent)
{
	ASSERT_FALSE(eventLoop.isRunning());
	// TO DO
	// fake the ptn_engine
}

TEST_F(PTNEnginImpObj, setSleepDuration)
{
	EXPECT_EQ(100ms, eventLoop.getSleepDuration());
	eventLoop.setSleepDuration(10ms);
	EXPECT_EQ(10ms, eventLoop.getSleepDuration());
	// eventLoop.start();
	// eventLoop.notifyNewEvent();

	// TO DO measure the sleep time until a new check for a transition active
}
