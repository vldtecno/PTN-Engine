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

#pragma once

#include <stdexcept>
#include <vector>

//! A very simple list based on std::vector
template <class T>
class List
{
public:
	List() = default;

	//!
	//! \brief Add an item to the list.
	//! \param item Item.
	//!
	void addItem(const T &item)
	{
		m_items.push_back(item);
	}

	//!
	//! \brief The number of elements.
	//! \return Number of elements.
	//!
	size_t size() const
	{
		return m_items.size();
	}

	//!
	//! \brief Get an item from the list.
	//! \param i Index of the selected item.
	//! \return A copy of the item.
	//!
	T getItem(const size_t i) const
	{
		if (i < m_items.size())
		{
			return m_items.at(i);
		}
		throw std::runtime_error("Out of bounds");
	}

private:
	//! Collection of items.
	std::vector<T> m_items;
};
