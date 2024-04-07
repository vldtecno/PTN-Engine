/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2024 Eduardo Valgôde
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
#include "PTN_Engine/Place.h"
#include <gtest/gtest.h>

using namespace ptne;
using namespace std;

class Place_PTNEngineObject : public testing::Test
{
public:
	PTN_EngineImp ptnEngine = PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP);
};

TEST_F(Place_PTNEngineObject, create_place_with_default_PlaceProperties)
{
	PlaceProperties placeProperties;
	ASSERT_NO_THROW(Place place(ptnEngine, placeProperties));
}

TEST_F(Place_PTNEngineObject, missing_on_enter_throws)
{
	auto onExit = []() {};
	PlaceProperties placeProperties{
		.onEnterActionFunctionName = "on_enter_function",
		.onExitActionFunctionName = "on_exit_function",
		.onEnterAction = nullptr,
		.onExitAction = onExit,
	};
	ASSERT_THROW(Place place(ptnEngine, placeProperties), PTN_Exception);
}

TEST_F(Place_PTNEngineObject, missing_on_exit_throws)
{
	auto onEnter = []() {};
	PlaceProperties placeProperties{
		.onEnterActionFunctionName = "on_enter_function",
		.onExitActionFunctionName = "on_exit_function",
		.onEnterAction = onEnter,
		.onExitAction = nullptr,
	};
	ASSERT_THROW(Place place(ptnEngine, placeProperties), PTN_Exception);
}

TEST_F(Place_PTNEngineObject, no_missing_functions_does_not_throw)
{
	auto onEnter = []() {};
	auto onExit = onEnter;

	PlaceProperties placeProperties{
		.onEnterActionFunctionName = "on_enter_function",
		.onExitActionFunctionName = "on_exit_function",
		.onEnterAction = onEnter,
		.onExitAction = onExit,
	};
	ASSERT_NO_THROW(Place place(ptnEngine, placeProperties));
}

TEST_F(Place_PTNEngineObject, unnamed_onEnter_onExit_functions_does_not_throw)
{
	auto onEnter = []() {};
	auto onExit = onEnter;

	PlaceProperties placeProperties{
		.onEnterAction = onEnter,
		.onExitAction = onExit,
	};
	ASSERT_NO_THROW(Place place(ptnEngine, placeProperties));
}

TEST_F(Place_PTNEngineObject, unspecified_onEnter_onExit_functions_does_not_throw)
{
	PlaceProperties placeProperties{
		.onEnterAction = nullptr,
		.onExitAction = nullptr,
	};
	ASSERT_NO_THROW(Place place(ptnEngine, placeProperties));
}

TEST_F(Place_PTNEngineObject, blockStartingOnEnterActions_prevents_on_enter_action_from_starting)
{
	atomic<bool> stop = false;
	auto onEnter = [&stop]()
	{
		while (!stop)
		{
			this_thread::sleep_for(20ms);
		}
	};
	auto ptnEngine = make_unique<PTN_EngineImp>(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
	PlaceProperties placeProperties{ .onEnterAction = onEnter };
	Place place(*ptnEngine, placeProperties);
	EXPECT_FALSE(place.isOnEnterActionInExecution());
	place.blockStartingOnEnterActions(true);
	jthread t([&place]() { place.enterPlace(1); });
	EXPECT_FALSE(place.isOnEnterActionInExecution());
	place.blockStartingOnEnterActions(false);
	this_thread::sleep_for(100ms);
	EXPECT_TRUE(place.isOnEnterActionInExecution());
	stop = true;
	this_thread::sleep_for(100ms);
	EXPECT_FALSE(place.isOnEnterActionInExecution());
}

TEST_F(Place_PTNEngineObject, enter_place_increases_number_of_tokens)
{
	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	ASSERT_EQ(0, place.getNumberOfTokens());
	place.enterPlace();
	EXPECT_EQ(1, place.getNumberOfTokens());
	place.enterPlace(2);
	EXPECT_EQ(3, place.getNumberOfTokens());
}

TEST_F(Place_PTNEngineObject, enter_place_overflow_throws)
{
	PlaceProperties placeProperties{ .initialNumberOfTokens = ULLONG_MAX };
	Place place(ptnEngine, placeProperties);
	ASSERT_EQ(ULLONG_MAX, place.getNumberOfTokens());
	ASSERT_THROW(place.enterPlace(), OverflowException);
	ASSERT_THROW(place.enterPlace(0), NullTokensException);
}

TEST_F(Place_PTNEngineObject, exit_place_decreases_number_of_tokens)
{
	PlaceProperties placeProperties{ .initialNumberOfTokens = 10 };
	Place place(ptnEngine, placeProperties);
	ASSERT_EQ(10, place.getNumberOfTokens());
	place.exitPlace();
	EXPECT_EQ(9, place.getNumberOfTokens());
	place.exitPlace(2);
	EXPECT_EQ(7, place.getNumberOfTokens());
}

TEST_F(Place_PTNEngineObject, exit_place_underflow_throws)
{
	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	ASSERT_EQ(0, place.getNumberOfTokens());
	ASSERT_THROW(place.exitPlace(), NotEnoughTokensException);
}

TEST_F(Place_PTNEngineObject, get_name_returns_name)
{
	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	EXPECT_TRUE(place.getName().empty());
	placeProperties.name = "P1 1";
	Place place2(ptnEngine, placeProperties);
	EXPECT_EQ("P1 1", place2.getName());
}

TEST_F(Place_PTNEngineObject, get_number_of_tokens_returns_number_of_tokens)
{
	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	EXPECT_EQ(0, place.getNumberOfTokens());
	placeProperties.initialNumberOfTokens = ULLONG_MAX;
	Place place2(ptnEngine, placeProperties);
	EXPECT_EQ(ULLONG_MAX, place2.getNumberOfTokens());
	placeProperties.initialNumberOfTokens = -1;
	Place place3(ptnEngine, placeProperties);
	EXPECT_EQ(ULLONG_MAX, place3.getNumberOfTokens());
}

TEST_F(Place_PTNEngineObject, get_action_name_returns_action_name)
{
	auto onEnter = []() {};
	auto onExit = onEnter;

	PlaceProperties placeProperties{
		.onEnterActionFunctionName = "on_enter_function",
		.onExitActionFunctionName = "on_exit_function",
		.onEnterAction = onEnter,
		.onExitAction = onExit,
	};
	Place place(ptnEngine, placeProperties);
	EXPECT_EQ("on_enter_function", place.getOnEnterActionName());
	EXPECT_EQ("on_exit_function", place.getOnExitActionName());
}

TEST_F(Place_PTNEngineObject, isInputPlace_returns_if_place_is_input_place)
{
	auto onEnter = []() {};
	auto onExit = onEnter;

	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	EXPECT_FALSE(place.isInputPlace());
	placeProperties.input = true;
	Place inputPlace(ptnEngine, placeProperties);
	EXPECT_TRUE(inputPlace.isInputPlace());
}

TEST(Place_, isOnEnterActionInExecutione_returns_if_on_enter_action_is_in_execution)
{
	atomic<bool> stop = false;
	auto onEnter = [&stop]()
	{
		while (!stop)
		{
			this_thread::sleep_for(20ms);
		}
	};
	PlaceProperties placeProperties{ .onEnterAction = onEnter };
	PTN_EngineImp ptnEngine = PTN_EngineImp(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
	Place place(ptnEngine, placeProperties);
	EXPECT_FALSE(place.isOnEnterActionInExecution());
	place.enterPlace(1);
	EXPECT_TRUE(place.isOnEnterActionInExecution());
	stop = true;
	this_thread::sleep_for(100ms);
	EXPECT_FALSE(place.isOnEnterActionInExecution());
}

TEST_F(Place_PTNEngineObject, placeProperties_returns_place_properties)
{
	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	place.setNumberOfTokens(10);
	EXPECT_EQ(10, place.placeProperties().initialNumberOfTokens);
	EXPECT_EQ(false, place.placeProperties().input);
	EXPECT_EQ("", place.placeProperties().name);
	EXPECT_EQ("", place.placeProperties().onEnterActionFunctionName);
	EXPECT_EQ("", place.placeProperties().onExitActionFunctionName);
	EXPECT_EQ(nullptr, place.placeProperties().onEnterAction);
	EXPECT_EQ(nullptr, place.placeProperties().onExitAction);

	string str = "abc";
	auto f = [&str]() { str.append("1"); };
	placeProperties.initialNumberOfTokens = 2;
	placeProperties.input = true;
	placeProperties.name = "P0";
	placeProperties.onEnterActionFunctionName = "onEnter";
	placeProperties.onExitActionFunctionName = "onExit";
	placeProperties.onEnterAction = f;
	placeProperties.onExitAction = f;

	Place place2(ptnEngine, placeProperties);
	EXPECT_EQ(2, place2.placeProperties().initialNumberOfTokens);
	EXPECT_EQ(true, place2.placeProperties().input);
	EXPECT_EQ("P0", place2.placeProperties().name);
	EXPECT_EQ("onEnter", place2.placeProperties().onEnterActionFunctionName);
	EXPECT_EQ("onExit", place2.placeProperties().onExitActionFunctionName);
	place2.placeProperties().onEnterAction();
	EXPECT_EQ("abc1", str);
	place2.placeProperties().onExitAction();
	EXPECT_EQ("abc11", str);
}

TEST_F(Place_PTNEngineObject, setNumberOfTokens_returns_number_of_tokens)
{
	PlaceProperties placeProperties;
	Place place(ptnEngine, placeProperties);
	ASSERT_EQ(0, place.getNumberOfTokens());
	place.setNumberOfTokens(1);
	EXPECT_EQ(1, place.getNumberOfTokens());
	place.setNumberOfTokens(ULLONG_MAX);
	EXPECT_EQ(ULLONG_MAX, place.getNumberOfTokens());
	place.setNumberOfTokens(-1);
	EXPECT_EQ(ULLONG_MAX, place.getNumberOfTokens());
}
