/*
 * This file is part of PTN Engine
 *
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

#include "PTN_Engine/ManagerBase.h"
#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/Place.h"
#include <shared_mutex>

namespace ptne
{

using SharedPtrPlace = std::shared_ptr<Place>;
using WeakPtrPlace = std::weak_ptr<Place>;

//!
//! \brief Holds and manages a collection of places.
//!
class PlacesManager : private ManagerBase<Place>
{
public:
	~PlacesManager();
	PlacesManager();
	PlacesManager(const PlacesManager &) = delete;
	PlacesManager(PlacesManager &&) = delete;
	PlacesManager &operator=(PlacesManager &) = delete;
	PlacesManager &operator=(PlacesManager &&) = delete;

	//!
	//! \brief Removes all places.
	//!
	void clear();

	//!
	//! \brief Sets the number of tokens in the input places to 0.
	//!
	void clearInputPlaces() const;

	bool contains(const std::string &itemName) const;

	//!
	//! \brief Gets the number of tokens in a given place.
	//! \param place - identifier of a place.
	//! \return Number of tokens inside place.
	//!
	size_t getNumberOfTokens(const std::string &place) const;

	std::shared_ptr<Place> getPlace(const std::string &placeName) const;

	std::vector<WeakPtrPlace> getPlaces(const std::vector<std::string> &placesNames) const;

	std::vector<PlaceProperties> getPlacesProperties() const;

	//!
	//! \brief Increment the number of tokens in an input place.
	//! \param place - Identifier of the input place to increment.
	//!
	void incrementInputPlace(const std::string &place);

	void insert(const std::shared_ptr<Place> &place);

	//!
	//! Print the petri net places and number of tokens.
	//! \param o Output stream.
	//!
	void printState(std::ostream &o) const;

	//!
	//! \brief Set the action executor in each place.
	//! \param actionsExecutor - the new actions executor to be used.
	//!
	void setActionsExecutor(std::shared_ptr<IActionsExecutor> &actionsExecutor);

private:
	//!
	//! Shared mutex to synchronize the access to the items(readers-writer lock).
	//!
	mutable std::shared_mutex m_itemsMutex;

	//!
	//! \brief Vector with the input places.
	//!
	std::vector<WeakPtrPlace> m_inputPlaces;
};

} // namespace ptne
