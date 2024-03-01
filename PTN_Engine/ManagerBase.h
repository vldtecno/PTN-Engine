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

#pragma once

#include "PTN_Engine/PTN_Exception.h"
#include <memory>
#include <unordered_map>

namespace ptne
{

template <typename T>
class ManagerBase
{
protected:
	ManagerBase() = default;
	~ManagerBase() = default;
	ManagerBase(const ManagerBase &) = delete;
	ManagerBase(ManagerBase &&) = delete;
	ManagerBase &operator=(const ManagerBase &) = delete;
	ManagerBase &operator=(const ManagerBase &&) = delete;

	//!
	//! \brief contains
	//! \param name
	//! \return
	//!
	bool contains(const std::string &itemName) const
	{
		return m_items.contains(itemName);
	}

	//!
	//! \brief insert
	//! \param item
	//!
	void insert(const std::shared_ptr<T> &item)
	{
		const auto &itemName = item->getName();
		if (m_items.contains(itemName))
		{
			throw RepeatedPlaceException(itemName);
		}
		m_items[itemName] = item;
	}

	//!
	//! \brief Removes all places from the container.
	//!
	void clear()
	{
		m_items.clear();
	}

	//!
	//! \brief getPlace
	//! \param itemName
	//! \return
	//!
	std::shared_ptr<T> getItem(const std::string &itemName) const
	{
		if (!m_items.contains(itemName))
		{
			throw PTN_Exception("Cannot get:" + itemName);
		}
		return m_items.at(itemName);
	}

	//!
	//! \brief Vector with the items.
	//!
	std::unordered_map<std::string, std::shared_ptr<T>> m_items;
};

} // namespace ptne
