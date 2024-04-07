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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/PTN_Exception.h"
#include "PTN_Engine/Transition.h"
#include <gtest/gtest.h>

using namespace std;
using namespace ptne;

class EventLoopPTNEngine : public testing::Test
{
public:
	PTN_Engine ptnEngine = PTN_Engine(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
};

class JobQueuePTNEngine : public testing::Test
{
public:
	PTN_Engine ptnEngine = PTN_Engine(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
};

TEST(PTN_Engine, constructors_do_not_throw)
{
	ASSERT_NO_THROW(PTN_Engine{ PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED });
	ASSERT_NO_THROW(PTN_Engine{ PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE });
	ASSERT_NO_THROW(PTN_Engine{ PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD });
	ASSERT_NO_THROW(PTN_Engine{ PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP });
}

TEST(PTN_Engine, getActionsThreadOption_returns_the_actions_thread_option)
{
	PTN_Engine ptnEngineJobQueue(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
	EXPECT_EQ(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE, ptnEngineJobQueue.getActionsThreadOption());
	PTN_Engine ptnEngineDetached(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);
	EXPECT_EQ(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED, ptnEngineDetached.getActionsThreadOption());
	PTN_Engine ptnEngineSingleThread(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);
	EXPECT_EQ(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD, ptnEngineSingleThread.getActionsThreadOption());
	PTN_Engine ptnEngineEventLoop(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	EXPECT_EQ(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP, ptnEngineEventLoop.getActionsThreadOption());

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, addArc_adds_an_arc_between_a_place_and_a_transition)
{
	ptnEngine.createPlace(PlaceProperties{ .name = "P1", .input = true });

	EXPECT_TRUE(ptnEngine.getTransitionsProperties().empty());

	ptnEngine.createTransition(TransitionProperties{ .name = "T1" });

	auto transitionsProperties = ptnEngine.getTransitionsProperties();
	EXPECT_EQ(1, transitionsProperties.size());
	EXPECT_TRUE(transitionsProperties.at(0).activationArcs.empty());
	EXPECT_TRUE(transitionsProperties.at(0).destinationArcs.empty());
	EXPECT_TRUE(transitionsProperties.at(0).inhibitorArcs.empty());

	ptnEngine.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	transitionsProperties = ptnEngine.getTransitionsProperties();
	EXPECT_EQ(1, transitionsProperties.size());
	EXPECT_EQ(1, transitionsProperties.at(0).activationArcs.size());
	EXPECT_TRUE(transitionsProperties.at(0).destinationArcs.empty());
	EXPECT_TRUE(transitionsProperties.at(0).inhibitorArcs.empty());
	EXPECT_EQ("P1", transitionsProperties.at(0).activationArcs.at(0).placeName);

	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ptne::ArcProperties::Type::DESTINATION });
	transitionsProperties = ptnEngine.getTransitionsProperties();
	EXPECT_EQ(1, transitionsProperties.size());
	EXPECT_EQ(1, transitionsProperties.at(0).activationArcs.size());
	EXPECT_EQ(1, transitionsProperties.at(0).destinationArcs.size());
	EXPECT_TRUE(transitionsProperties.at(0).inhibitorArcs.empty());
	EXPECT_EQ("P1", transitionsProperties.at(0).destinationArcs.at(0).placeName);

	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ptne::ArcProperties::Type::INHIBITOR });
	transitionsProperties = ptnEngine.getTransitionsProperties();
	EXPECT_EQ(1, transitionsProperties.size());
	EXPECT_EQ(1, transitionsProperties.at(0).activationArcs.size());
	EXPECT_EQ(1, transitionsProperties.at(0).destinationArcs.size());
	EXPECT_EQ(1, transitionsProperties.at(0).inhibitorArcs.size());
	EXPECT_EQ("P1", transitionsProperties.at(0).inhibitorArcs.at(0).placeName);

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, clearNet_clears_the_petri_net_from_all_places_and_transitons)
{
	EXPECT_TRUE(ptnEngine.getTransitionsProperties().empty());
	EXPECT_TRUE(ptnEngine.getPlacesProperties().empty());

	ptnEngine.createPlace(PlaceProperties{ .name = "P1", .input = true });
	ptnEngine.createTransition(TransitionProperties{ .name = "T1" });

	ptnEngine.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ptne::ArcProperties::Type::DESTINATION });
	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ptne::ArcProperties::Type::INHIBITOR });
	EXPECT_FALSE(ptnEngine.getTransitionsProperties().empty());
	EXPECT_FALSE(ptnEngine.getPlacesProperties().empty());

	ptnEngine.clearNet();
	EXPECT_TRUE(ptnEngine.getTransitionsProperties().empty());
	EXPECT_TRUE(ptnEngine.getPlacesProperties().empty());

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, createPlace_creates_a_place_in_the_petri_net)
{
	EXPECT_TRUE(ptnEngine.getPlacesProperties().empty());
	PlaceProperties placeProperties{
		.name = "P1",
		.initialNumberOfTokens = 3,
		.onEnterActionFunctionName = "",
		.onExitActionFunctionName = "",
		.onEnterAction = nullptr,
		.onExitAction = nullptr,
		.input = true,
	};
	ptnEngine.createPlace(placeProperties);
	EXPECT_EQ(1, ptnEngine.getPlacesProperties().size());
	EXPECT_EQ("P1", ptnEngine.getPlacesProperties().at(0).name);
	EXPECT_EQ(3, ptnEngine.getPlacesProperties().at(0).initialNumberOfTokens);
	EXPECT_EQ("", ptnEngine.getPlacesProperties().at(0).onEnterActionFunctionName);
	EXPECT_EQ("", ptnEngine.getPlacesProperties().at(0).onExitActionFunctionName);
	EXPECT_EQ(nullptr, ptnEngine.getPlacesProperties().at(0).onEnterAction);
	EXPECT_EQ(nullptr, ptnEngine.getPlacesProperties().at(0).onExitAction);
	EXPECT_TRUE(ptnEngine.getPlacesProperties().at(0).input);

	ASSERT_THROW(ptnEngine.createPlace(placeProperties), PTN_Exception);
	placeProperties.name = "";
	ASSERT_THROW(ptnEngine.createPlace(placeProperties), PTN_Exception);


	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, createTransition_creates_a_transition_in_the_petri_net)
{
	EXPECT_TRUE(ptnEngine.getTransitionsProperties().empty());
	TransitionProperties transitionProperties{
		.name = "T1",
		.activationArcs = {},
		.destinationArcs = {},
		.inhibitorArcs = {},
		.additionalConditionsNames = {},
		.additionalConditions = {},
		.requireNoActionsInExecution = false,
	};
	ptnEngine.createTransition(transitionProperties);
	auto transitionsProperties = ptnEngine.getTransitionsProperties();
	EXPECT_EQ(1, transitionsProperties.size());
	EXPECT_EQ("T1", transitionsProperties.at(0).name);
	EXPECT_TRUE(transitionsProperties.at(0).activationArcs.empty());
	EXPECT_TRUE(transitionsProperties.at(0).destinationArcs.empty());
	EXPECT_TRUE(transitionsProperties.at(0).inhibitorArcs.empty());
	EXPECT_TRUE(transitionsProperties.at(0).additionalConditionsNames.empty());
	EXPECT_TRUE(transitionsProperties.at(0).additionalConditions.empty());
	EXPECT_FALSE(transitionsProperties.at(0).requireNoActionsInExecution);

	ASSERT_THROW(ptnEngine.createTransition(transitionProperties), PTN_Exception);
	transitionProperties.name = "";
	ASSERT_THROW(ptnEngine.createTransition(transitionProperties), PTN_Exception);

	// TO DO test invoking while in execution
}

TEST(PTN_Engine, execute_starts_the_execution_of_the_petri_net)
{
	PTN_Engine ptnEngine(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);
	TransitionProperties transitionProperties{
		.name = "T1",
		.activationArcs = {},
		.destinationArcs = {},
		.inhibitorArcs = {},
		.additionalConditionsNames = {},
		.additionalConditions = {},
		.requireNoActionsInExecution = false,
	};
	ptnEngine.createTransition(transitionProperties);
	transitionProperties.name = "T2";
	ptnEngine.createTransition(transitionProperties);

	PlaceProperties placeProperties{
		.name = "P1",
		.initialNumberOfTokens = 1,
		.onEnterActionFunctionName = "",
		.onExitActionFunctionName = "",
		.onEnterAction = nullptr,
		.onExitAction = nullptr,
		.input = true,
	};
	ptnEngine.createPlace(placeProperties);

	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ArcProperties::Type::ACTIVATION });

	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T2", .type = ArcProperties::Type::ACTIVATION });

	EXPECT_EQ(1, ptnEngine.getNumberOfTokens("P1"));

	ptnEngine.execute();

	EXPECT_EQ(0, ptnEngine.getNumberOfTokens("P1"));


	// TO DO test with different actions thread options
}

TEST_F(
EventLoopPTNEngine,
event_loop_sleep_time_duration_determines_the_time_the_event_loop_waits_until_it_checks_if_a_transition_can_be_fired)
{
	EXPECT_EQ(100ms, ptnEngine.getEventLoopSleepDuration());

	ptnEngine.setEventLoopSleepDuration(1000ms);
	EXPECT_EQ(1000ms, ptnEngine.getEventLoopSleepDuration());


	auto t_start = std::chrono::high_resolution_clock::now();
	double duration_ms = 0;

	auto startTimer = [&duration_ms, &t_start]()
	{
		t_start = std::chrono::high_resolution_clock::now();
		duration_ms = 0;
	};

	auto stopTimer = [&duration_ms, &t_start]()
	{
		auto t_end = chrono::high_resolution_clock::now();
		duration_ms = chrono::duration<double, std::milli>(t_end - t_start).count();
	};

	atomic<bool> canFire = false;
	auto trigger = [&canFire]() { return canFire ? true : false; };

	TransitionProperties transitionProperties{
		.name = "T1",
		.activationArcs = {},
		.destinationArcs = {},
		.inhibitorArcs = {},
		.additionalConditionsNames = {},
		.additionalConditions = { trigger },
		.requireNoActionsInExecution = false,
	};
	ptnEngine.createTransition(transitionProperties);

	PlaceProperties placeProperties{
		.name = "P1",
		.initialNumberOfTokens = 1,
		.onEnterActionFunctionName = "",
		.onExitActionFunctionName = "",
		.onEnterAction = nullptr,
		.onExitAction = stopTimer,
		.input = true,
	};
	ptnEngine.createPlace(placeProperties);

	ptnEngine.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ArcProperties::Type::ACTIVATION });

	ptnEngine.execute();

	this_thread::sleep_for(25ms);

	startTimer();


	canFire = true;

	while (ptnEngine.getNumberOfTokens("P1") > 0)
	{
		this_thread::sleep_for(300ms);
	}

	ptnEngine.stop();

	EXPECT_LT(duration_ms, 1050.0);
	EXPECT_GT(duration_ms, 950.0);
}

TEST_F(JobQueuePTNEngine, getNumberOfTokens_throws_if_an_invalid_place_name_is_given)
{
	ASSERT_THROW(ptnEngine.getNumberOfTokens(""), PTN_Exception);
}

TEST_F(JobQueuePTNEngine, incrementInputPlace_thows_if_an_invalid_place_name_is_given)
{
	ASSERT_THROW(ptnEngine.incrementInputPlace(""), PTN_Exception);

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, isEventLoopRunning_returns_if_the_event_loop_is_running_for_JOB_QUEUE_mode)
{
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	ptnEngine.execute();
	EXPECT_TRUE(ptnEngine.isEventLoopRunning());
	ptnEngine.stop();
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	// TO DO test invoking while in execution
}

TEST_F(EventLoopPTNEngine, isEventLoopRunning_returns_if_the_event_loop_is_running_for_EVENT_LOOP_mode)
{
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	ptnEngine.execute();
	EXPECT_TRUE(ptnEngine.isEventLoopRunning());
	ptnEngine.stop();
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	// TO DO test invoking while in execution
}

TEST(PTN_Engine, isEventLoopRunning_returns_if_the_event_loop_is_running_for_DETACHED_mode)
{
	PTN_Engine ptnEngine(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	ptnEngine.execute();
	EXPECT_TRUE(ptnEngine.isEventLoopRunning());
	ptnEngine.stop();
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	// TO DO test invoking while in execution
}

TEST(PTN_Engine, isEventLoopRunning_returns_false_for_SINGLE_THREAD_mode)
{
	PTN_Engine ptnEngine(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	ptnEngine.execute();
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	ptnEngine.stop();
	EXPECT_FALSE(ptnEngine.isEventLoopRunning());
	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, registerAction_registers_an_action_in_the_petri_net)
{
	ActionFunction actionFunction = []() {};
	ASSERT_THROW(ptnEngine.registerAction("", actionFunction), InvalidFunctionNameException);

	ASSERT_NO_THROW(ptnEngine.registerAction("AF1", actionFunction));

	ASSERT_THROW(ptnEngine.registerAction("AF1", actionFunction), RepeatedFunctionException);

	ASSERT_THROW(ptnEngine.createPlace(PlaceProperties{ .name = "P1", .onEnterActionFunctionName = "AF2" }),
				 PTN_Exception);
	ASSERT_NO_THROW(ptnEngine.createPlace(PlaceProperties{ .name = "P1", .onEnterActionFunctionName = "AF1" }));

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, registerCondition_registers_a_condition_function_in_the_petri_net)
{
	ConditionFunction conditionFunction = []() { return false; };
	ASSERT_THROW(ptnEngine.registerCondition("", conditionFunction), InvalidFunctionNameException);

	ASSERT_NO_THROW(ptnEngine.registerCondition("CF1", conditionFunction));

	ASSERT_THROW(ptnEngine.registerCondition("CF1", conditionFunction), RepeatedFunctionException);

	ASSERT_THROW(ptnEngine.createTransition(
				 TransitionProperties{ .name = "P1", .additionalConditionsNames = { "CF2" } }),
				 PTN_Exception);
	ASSERT_NO_THROW(
	ptnEngine.createTransition(TransitionProperties{ .name = "P1", .additionalConditionsNames = { "CF1" } }));

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, removeArc_removes_an_arc_between_a_place_and_a_transition)
{
	ArcProperties arcProperties;
	ASSERT_THROW(ptnEngine.removeArc(arcProperties), PTN_Exception);
	ptnEngine.createPlace({ .name = "P1" });
	ptnEngine.createTransition({ .name = "T1" });
	arcProperties = ArcProperties{ .placeName = "P1", .transitionName = "T1" };
	ASSERT_TRUE(ptnEngine.getTransitionsProperties().at(0).activationArcs.empty());
	ptnEngine.addArc(arcProperties);
	ASSERT_EQ(1, ptnEngine.getTransitionsProperties().at(0).activationArcs.size());
	ASSERT_NO_THROW(ptnEngine.removeArc(arcProperties));
	EXPECT_TRUE(ptnEngine.getTransitionsProperties().at(0).activationArcs.empty());

	// TO DO test invoking while in execution
}

TEST_F(JobQueuePTNEngine, setActionsThreadOption_sets_the_runtime_mode_in_the_petri_net)
{
	ASSERT_NO_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD));
	ASSERT_NO_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED));
	ASSERT_NO_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP));
	ptnEngine.execute();
	ASSERT_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD),
				 PTN_Exception);
	ASSERT_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED), PTN_Exception);
	ASSERT_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP), PTN_Exception);
	ASSERT_THROW(ptnEngine.setActionsThreadOption(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE), PTN_Exception);
}

TEST_F(JobQueuePTNEngine, stops_the_petri_net_execution_and_never_throws)
{
	ASSERT_NO_THROW(ptnEngine.stop());
	ASSERT_NO_THROW(ptnEngine.stop());
	ptnEngine.execute();
	ASSERT_NO_THROW(ptnEngine.stop());
	ASSERT_NO_THROW(ptnEngine.stop());
}
