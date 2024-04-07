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

#include "PTN_Engine/ManagedContainer.h"
#include <gtest/gtest.h>

template class ptne::ManagedContainer<int>;

using namespace std;
using namespace ptne;

class ManagedContainer_Int : public testing::Test
{
public:
	ManagedContainer<int> managedContainer;
};

TEST_F(ManagedContainer_Int, addItem_adds_item)
{
	managedContainer.addItem("a", 1);
	managedContainer.addItem("b", 2);
	EXPECT_EQ(1, managedContainer.getItem("a"));
	EXPECT_EQ(2, managedContainer.getItem("b"));
}

TEST_F(ManagedContainer_Int, addItem_add_repeated_item_name_throws)
{
	managedContainer.addItem("a", 1);
	EXPECT_EQ(1, managedContainer.getItem("a"));
	ASSERT_THROW(managedContainer.addItem("a", 2), RepeatedFunctionException);
}

TEST_F(ManagedContainer_Int, addItem_add_item_without_name_throws)
{
	ASSERT_THROW(managedContainer.addItem("", 1), InvalidFunctionNameException);
}

TEST_F(ManagedContainer_Int, getItem_get_item_with_empty_name_throws)
{
	ASSERT_THROW(managedContainer.getItem(""), InvalidFunctionNameException);
}

TEST_F(ManagedContainer_Int, getItem_get_item_with_non_existing_name_throws)
{
	ASSERT_THROW(managedContainer.getItem("a"), InvalidFunctionNameException);
}

TEST_F(ManagedContainer_Int, getItems_returns_items_and_in_the_correct_order)
{
	managedContainer.addItem("a", 1);
	managedContainer.addItem("b", 2);
	vector<string> names = { "a", "b" };
	auto items = managedContainer.getItems(names);
	vector<int> expectedItems{ 1, 2 };
	for (size_t i = 0; i < 2; ++i)
	{
		EXPECT_EQ(names[i], items[i].first);
		EXPECT_EQ(expectedItems[i], items[i].second);
	}

	names = { "b", "a" };
	items = managedContainer.getItems(names);
	expectedItems = { 2, 1 };
	for (size_t i = 0; i < 2; ++i)
	{
		EXPECT_EQ(names[i], items[i].first);
		EXPECT_EQ(expectedItems[i], items[i].second);
	}
}

TEST_F(ManagedContainer_Int, getItems_with_invalid_name_throws)
{
	managedContainer.addItem("a", 1);
	managedContainer.addItem("b", 2);

	vector<string> names = { "a", "c" };
	ASSERT_THROW(managedContainer.getItems(names), InvalidFunctionNameException);
}

TEST_F(ManagedContainer_Int, getItems_with_empty_name_throws)
{
	managedContainer.addItem("a", 1);
	managedContainer.addItem("b", 2);

	vector<string> names = { "", "b" };
	ASSERT_THROW(managedContainer.getItems(names), InvalidFunctionNameException);
}

// TO DO add test using add item and get item concurrently
