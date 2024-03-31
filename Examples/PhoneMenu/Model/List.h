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
	~List() = default;
	List() = default;
	List(const List &) = delete;
	List(List &&) = delete;
	List &operator=(const List &) = delete;
	List &operator=(List &&) = delete;

	void addItem(const T &item)
	{
		m_items.push_back(item);
	}

	size_t size() const
	{
		return m_items.size();
	}

	T getItem(const size_t i) const
	{
		if (i < m_items.size())
		{
			return m_items.at(i);
		}
		throw std::runtime_error("Out of bounds");
	}

private:
	std::vector<T> m_items;
};
