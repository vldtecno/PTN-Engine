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

#include "PTN_Engine/Transition.h"
#include "PTN_Engine/TransitionsManager.h"
#include <gtest/gtest.h>

using namespace ptne;
using namespace std;

class TransitionsManagerObj : public testing::Test
{
public:
	TransitionsManager transitionsManager = TransitionsManager{};
};

TEST_F(TransitionsManagerObj, clear)
{
	ASSERT_NO_THROW(transitionsManager.clear());
	ASSERT_TRUE(transitionsManager.getTransitionsProperties().empty());

	TransitionProperties transitionProperties;
	auto t = make_shared<Transition>("T1", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									 vector<pair<string, ConditionFunction>>{}, false);
	transitionsManager.insert(t);
	ASSERT_FALSE(transitionsManager.getTransitionsProperties().empty());
	ASSERT_NO_THROW(transitionsManager.clear());
	EXPECT_TRUE(transitionsManager.getTransitionsProperties().empty());
}

TEST_F(TransitionsManagerObj, collectEnabledTransitionsRandomly)
{
	// TO DO

	// add 3 disabled transitions
	// collect enabled should return 0

	// add 2 enabled transitions
	// collect enabled should return 2

	// make sure the order is randomized calling it repeatedly

	TransitionProperties transitionProperties;
	auto t = make_shared<Transition>("T1", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									 vector<pair<string, ConditionFunction>>{}, false);

	transitionsManager.insert(t);
	EXPECT_TRUE(transitionsManager.contains("T1"));
	EXPECT_FALSE(transitionsManager.collectEnabledTransitionsRandomly().empty());
}

TEST_F(TransitionsManagerObj, contains_returns_if_the_container_contains_an_element_with_the_name_in_the_argument)
{
	EXPECT_FALSE(transitionsManager.contains("T1"));
	TransitionProperties transitionProperties;
	auto t = make_shared<Transition>("T1", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									 vector<pair<string, ConditionFunction>>{}, false);
	transitionsManager.insert(t);
	EXPECT_TRUE(transitionsManager.contains("T1"));
}

TEST_F(TransitionsManagerObj, getTransition_throws_if_the_transition_name_is_empty)
{
	ASSERT_THROW(transitionsManager.getTransition(""), PTN_Exception);
}

TEST_F(TransitionsManagerObj, getTransition_throws_if_the_transition_name_does_not_match_an_existing_transition)
{
	ASSERT_THROW(transitionsManager.getTransition("A"), PTN_Exception);
}

TEST_F(TransitionsManagerObj, getTransition_returns_the_transition_by_name)
{
	TransitionProperties transitionProperties;
	auto t = make_shared<Transition>("T1", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									 vector<pair<string, ConditionFunction>>{}, false);
	transitionsManager.insert(t);
	auto exported_t = transitionsManager.getTransition("T1");
	EXPECT_EQ(t, exported_t);
}

TEST_F(TransitionsManagerObj, getTransitionsProperties_exports_all_properties_from_all_transitions)
{
	TransitionProperties transitionProperties;
	auto t1 = make_shared<Transition>("T1", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									  vector<pair<string, ConditionFunction>>{}, false);
	auto t2 = make_shared<Transition>("T2", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									  vector<pair<string, ConditionFunction>>{}, false);
	auto t3 = make_shared<Transition>("T3", vector<Arc>{}, vector<Arc>{}, vector<Arc>{},
									  vector<pair<string, ConditionFunction>>{}, false);
	transitionsManager.insert(t1);
	transitionsManager.insert(t2);
	transitionsManager.insert(t3);
	auto transitionsProperties = transitionsManager.getTransitionsProperties();
	ASSERT_EQ(3, transitionsProperties.size());

	for (const auto &_transitionProperties : transitionsProperties)
	{
		if (_transitionProperties.name == "T3" || _transitionProperties.name == "T2" ||
		    _transitionProperties.name == "T1")
		{
			continue;
		}
		else
		{
			ASSERT_FALSE(true);
		}
	}	
}

TEST_F(TransitionsManagerObj, insert_nullptr_throws)
{
	ASSERT_THROW(transitionsManager.insert(nullptr), PTN_Exception);
}
