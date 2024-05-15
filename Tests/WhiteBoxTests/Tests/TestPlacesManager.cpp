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

#include "PTN_Engine/Executor/ActionsExecutorFactory.h"
#include "PTN_Engine/PlacesManager.h"
#include <gtest/gtest.h>

using namespace ptne;
using namespace std;

class PlacesManager_Obj : public testing::Test
{
public:
	PlacesManager placesManager = PlacesManager{};
	shared_ptr<IActionsExecutor> executor =
	ActionsExecutorFactory::createExecutor(PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE);
};

TEST_F(PlacesManager_Obj, clear_clears_all_places)
{
	ASSERT_NO_THROW(placesManager.clear());
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .name = "P1" };
	auto p = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p);
	ASSERT_FALSE(placesManager.getPlacesProperties().empty());
	ASSERT_NO_THROW(placesManager.clear());
	EXPECT_TRUE(placesManager.getPlacesProperties().empty());
}

TEST_F(PlacesManager_Obj, clearInputPlaces_removes_all_tokens_from_input_places)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	placeProperties.input = true;
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	auto placesProperties = placesManager.getPlacesProperties();
	ASSERT_EQ(2, placesManager.getPlacesProperties().size());
	for (const auto &_placeProperties : placesProperties)
	{
		if (_placeProperties.name == "P1")
		{
			EXPECT_EQ(2, _placeProperties.initialNumberOfTokens);
		}
		else if (_placeProperties.name == "P2")
		{
			EXPECT_EQ(2, _placeProperties.initialNumberOfTokens);
		}
		else
		{
			ASSERT_FALSE(true);
		}
	}

	ASSERT_NO_THROW(placesManager.clearInputPlaces());
	placesProperties = placesManager.getPlacesProperties();

	ASSERT_EQ(2, placesManager.getPlacesProperties().size());
	for (const auto &_placeProperties : placesProperties)
	{
		if (_placeProperties.name == "P1")
		{
			EXPECT_EQ(2, _placeProperties.initialNumberOfTokens);
		}
		else if (_placeProperties.name == "P2")
		{
			EXPECT_EQ(0, _placeProperties.initialNumberOfTokens);
		}
		else
		{
			ASSERT_FALSE(true);
		}
	}
}

TEST_F(PlacesManager_Obj, contains_returns_if_place_with_given_place_name_is_contained)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	placeProperties.input = true;
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	EXPECT_TRUE(placesManager.contains("P1"));
	EXPECT_TRUE(placesManager.contains("P2"));
	EXPECT_FALSE(placesManager.contains("P3"));
}

TEST_F(PlacesManager_Obj, getNumberOfTokens_returns_the_number_of_tokens_in_place_with_place_name)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	placeProperties.initialNumberOfTokens = 8;
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	EXPECT_EQ(2, placesManager.getNumberOfTokens("P1"));
	EXPECT_EQ(8, placesManager.getNumberOfTokens("P2"));
	ASSERT_THROW(placesManager.getNumberOfTokens("P3"), InvalidNameException);
}

TEST_F(PlacesManager_Obj, getPlace)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	EXPECT_EQ(p1, placesManager.getPlace("P1"));
	EXPECT_EQ(p2, placesManager.getPlace("P2"));
}

TEST_F(PlacesManager_Obj, getPlaces_returns_weak_pointers_to_places)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	auto places = placesManager.getPlaces({ "P1", "P2" });
	EXPECT_EQ(p1, places.at(0).lock());
	EXPECT_EQ(p2, places.at(1).lock());

	places = placesManager.getPlaces({ "P2", "P1" });
	EXPECT_EQ(p2, places.at(0).lock());
	EXPECT_EQ(p1, places.at(1).lock());
}

TEST_F(PlacesManager_Obj, getPlaces_throws_if_the_place_names_are_invalid)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	ASSERT_THROW(placesManager.getPlaces({ "P1", "P3" }), InvalidNameException);
	ASSERT_THROW(placesManager.getPlaces({ "P1", "P1" }), RepeatedPlaceNamesException);
}

TEST_F(PlacesManager_Obj, incrementInputPlace_increments_the_input_places_token_numbers)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	placeProperties.name = "P2";
	placeProperties.input = true;
	auto p2 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p2);

	EXPECT_EQ(2, placesManager.getNumberOfTokens("P1"));
	EXPECT_EQ(2, placesManager.getNumberOfTokens("P2"));
	placesManager.incrementInputPlace("P2");
	EXPECT_EQ(2, placesManager.getNumberOfTokens("P1"));
	EXPECT_EQ(3, placesManager.getNumberOfTokens("P2"));
	ASSERT_THROW(placesManager.incrementInputPlace("P1"), PTN_Exception);
	ASSERT_THROW(placesManager.incrementInputPlace("P3"), PTN_Exception);
}

TEST_F(PlacesManager_Obj, insert_inserts_a_new_place_in_the_collection)
{
	ASSERT_TRUE(placesManager.getPlacesProperties().empty());

	PlaceProperties placeProperties{ .initialNumberOfTokens = 2 };
	placeProperties.name = "P1";
	auto p1 = make_shared<Place>(placeProperties, executor);
	placesManager.insert(p1);

	ASSERT_THROW(placesManager.insert(p1), PTN_Exception);

	auto p2 = make_shared<Place>(placeProperties, executor);
	ASSERT_THROW(placesManager.insert(p2), PTN_Exception);
}
