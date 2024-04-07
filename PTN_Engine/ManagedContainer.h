/*
 * This file is part of PTN Engine
 * Copyright (c) 2023-2024 Eduardo Valgôde
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

#include "PTN_Engine/PTN_Exception.h"
#include <algorithm>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace ptne
{

//!
//! \brief The ManagedContainer class
//!
template <typename T>
class ManagedContainer
{
public:
	~ManagedContainer() = default;
	ManagedContainer() = default;
	ManagedContainer(const ManagedContainer &) = delete;
	ManagedContainer(ManagedContainer &&) = delete;
	ManagedContainer &operator=(const ManagedContainer &) = delete;
	ManagedContainer &operator=(const ManagedContainer &&) = delete;


	//!
	//! \brief Add item to container.
	//! \throws InvalidFunctionNameException, RepeatedFunctionException
	//! \param name - Name of the item used as key.
	//! \param item - The item itself.
	//!
	void addItem(const std::string &name, T item)
	{
		std::unique_lock lock(m_mutex);
		if (name.empty())
		{
			throw InvalidFunctionNameException(name);
		}

		if (m_items.contains(name))
		{
			throw RepeatedFunctionException(name);
		}

		m_items[name] = item;
	}

	//!
	//! \brief Retrieve a copy of the item.
	//! \param name - Identifier of the item.
	//! \throws InvalidFunctionNameException
	//! \return Copy of the item identified by name.
	//!
	T getItem(const std::string &name) const
	{
		if (name.empty())
		{
			throw InvalidFunctionNameException(name);
		}

		std::shared_lock lock(m_mutex);
		if (!m_items.contains(name))
		{
			throw InvalidFunctionNameException(name);
		}
		return m_items.at(name);
	}

	//!
	//! \brief Gets copies of the items based on their identifying names.
	//!        Invalid names result in a InvalidFunctionNameException.
	//! \throws InvalidFunctionNameException
	//! \param names of the items
	//! \return vector with the names and copies of the items found in the container.
	//!
	std::vector<std::pair<std::string, T>> getItems(const std::vector<std::string> &names) const
	{
		std::shared_lock lock(m_mutex);
		std::vector<std::pair<std::string, T>> items;
		std::ranges::for_each(names,
							  [&](const std::string &name)
							  {
								  if (name.empty())
								  {
									  throw InvalidFunctionNameException(name);
								  }

								  if (!m_items.contains(name))
								  {
									  throw InvalidFunctionNameException(name);
								  }
								  items.emplace_back(name, m_items.at(name));
							  });
		return items;
	}

private:
	//! An unordered map of items identified by a string.
	std::unordered_map<std::string, T> m_items;

	//! Protects concurrent access to m_items.
	mutable std::shared_mutex m_mutex;
};

} // namespace ptne
