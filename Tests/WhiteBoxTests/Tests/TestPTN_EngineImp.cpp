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

#include "PTN_Engine/PTN_EngineImp.h"
#include <gtest/gtest.h>

using namespace std;
using namespace ptne;

class PTN_EngineImp_JobQueue : public testing::Test
{
public:
    PTN_EngineImp ptnEngineImp = PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
};

TEST(PTN_EngineImp_, getNewInputReceived_incrementing_a_input_place_sets_the_new_inputReceived_flag)
{
	PTN_EngineImp ptnEngineImp(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
	EXPECT_FALSE(ptnEngineImp.getNewInputReceived());
	ptnEngineImp.createPlace(PlaceProperties{ .name = "P1", .input = true });
	ptnEngineImp.createTransition(TransitionProperties{ .name = "T1" });
	ptnEngineImp.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	EXPECT_FALSE(ptnEngineImp.getNewInputReceived());
	ptnEngineImp.incrementInputPlace("P1");
	EXPECT_TRUE(ptnEngineImp.getNewInputReceived());

	PTN_EngineImp ptnEngineImpEventLoop(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
	EXPECT_FALSE(ptnEngineImpEventLoop.getNewInputReceived());
	ptnEngineImpEventLoop.createPlace(PlaceProperties{ .name = "P1", .input = true });
	ptnEngineImpEventLoop.createTransition(TransitionProperties{ .name = "T1" });
	ptnEngineImpEventLoop.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	EXPECT_FALSE(ptnEngineImpEventLoop.getNewInputReceived());
	ptnEngineImpEventLoop.incrementInputPlace("P1");
	EXPECT_TRUE(ptnEngineImpEventLoop.getNewInputReceived());

	PTN_EngineImp ptnEngineImpDetached(PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED);
	EXPECT_FALSE(ptnEngineImpDetached.getNewInputReceived());
	ptnEngineImpDetached.createPlace(PlaceProperties{ .name = "P1", .input = true });
	ptnEngineImpDetached.createTransition(TransitionProperties{ .name = "T1" });
	ptnEngineImpDetached.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	EXPECT_FALSE(ptnEngineImpDetached.getNewInputReceived());
	ptnEngineImpDetached.incrementInputPlace("P1");
	EXPECT_TRUE(ptnEngineImpDetached.getNewInputReceived());

	PTN_EngineImp ptnEngineImpSingleThread(PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD);
	EXPECT_FALSE(ptnEngineImpSingleThread.getNewInputReceived());
	ptnEngineImpSingleThread.createPlace(PlaceProperties{ .name = "P1", .input = true });
	ptnEngineImpSingleThread.createTransition(TransitionProperties{ .name = "T1" });
	ptnEngineImpSingleThread.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	EXPECT_FALSE(ptnEngineImpSingleThread.getNewInputReceived());
	ptnEngineImpSingleThread.incrementInputPlace("P1");
	EXPECT_TRUE(ptnEngineImpSingleThread.getNewInputReceived());

	// TO DO test invoking while in execution
}

TEST_F(PTN_EngineImp_JobQueue, clearInputPlaces_resets_the_new_inputReceived_flag)
{
	EXPECT_FALSE(ptnEngineImp.getNewInputReceived());
	ptnEngineImp.createPlace(PlaceProperties{ .name = "P1", .input = true });
	ptnEngineImp.createTransition(TransitionProperties{ .name = "T1" });
	ptnEngineImp.addArc(ArcProperties{ .placeName = "P1", .transitionName = "T1" });
	EXPECT_FALSE(ptnEngineImp.getNewInputReceived());
	ptnEngineImp.incrementInputPlace("P1");
	EXPECT_TRUE(ptnEngineImp.getNewInputReceived());
	ptnEngineImp.clearInputPlaces();
	EXPECT_FALSE(ptnEngineImp.getNewInputReceived());

	// TO DO test invoking while in execution
}

TEST_F(PTN_EngineImp_JobQueue, enabledTransitions_returns_all_enabled_transitions)
{
	TransitionProperties transitionProperties{
		.name = "T1",
		.activationArcs = {},
		.destinationArcs = {},
		.inhibitorArcs = {},
		.additionalConditionsNames = {},
		.additionalConditions = {},
		.requireNoActionsInExecution = false,
	};
	ptnEngineImp.createTransition(transitionProperties);
	EXPECT_EQ(1, ptnEngineImp.enabledTransitions().size());

	PlaceProperties placeProperties{
		.name = "P1",
		.initialNumberOfTokens = 0,
		.onEnterActionFunctionName = "",
		.onExitActionFunctionName = "",
		.onEnterAction = nullptr,
		.onExitAction = nullptr,
		.input = true,
	};
	ptnEngineImp.createPlace(placeProperties);

	ptnEngineImp.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ArcProperties::Type::ACTIVATION });

	EXPECT_EQ(0, ptnEngineImp.enabledTransitions().size());

	transitionProperties.name = "T2";
	ptnEngineImp.createTransition(transitionProperties);
	EXPECT_EQ(1, ptnEngineImp.enabledTransitions().size());


	ptnEngineImp.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T2", .type = ArcProperties::Type::ACTIVATION });
	EXPECT_EQ(0, ptnEngineImp.enabledTransitions().size());


	// TO DO test invoking while in execution
}

TEST_F(PTN_EngineImp_JobQueue, enabledTransitions_returns_all_enabled_transitions_in_a_random_order)
{
	TransitionProperties transitionProperties{
		.name = "T1",
		.activationArcs = {},
		.destinationArcs = {},
		.inhibitorArcs = {},
		.additionalConditionsNames = {},
		.additionalConditions = {},
		.requireNoActionsInExecution = false,
	};
	ptnEngineImp.createTransition(transitionProperties);
	transitionProperties.name = "T2";
	ptnEngineImp.createTransition(transitionProperties);

	PlaceProperties placeProperties{
		.name = "P1",
		.initialNumberOfTokens = 1,
		.onEnterActionFunctionName = "",
		.onExitActionFunctionName = "",
		.onEnterAction = nullptr,
		.onExitAction = nullptr,
		.input = true,
	};
	ptnEngineImp.createPlace(placeProperties);

	ptnEngineImp.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T1", .type = ArcProperties::Type::ACTIVATION });

	ptnEngineImp.addArc(
	ArcProperties{ .placeName = "P1", .transitionName = "T2", .type = ArcProperties::Type::ACTIVATION });

	EXPECT_EQ(2, ptnEngineImp.enabledTransitions().size());

	int t1First = 0;
	for (int i = 0; i < 1000; ++i)
	{
		if (ptnEngineImp.enabledTransitions().at(0).lock()->getName() == "T1")
		{
			++t1First;
		}
	}

	EXPECT_GT(t1First, 475);
	EXPECT_LT(t1First, 525);

	// TO DO test invoking while in execution
}
