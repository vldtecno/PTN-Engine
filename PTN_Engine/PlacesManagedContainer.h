/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2023 Eduardo Valgôde
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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/Place.h"
#include <shared_mutex>

namespace ptne
{
using SharedPtrPlace = std::shared_ptr<Place>;
using WeakPtrPlace = std::weak_ptr<Place>;

/*!
 * \brief Holds and manages a collection of places.
 */
class PlacesManagedContainer
{
public:
	PlacesManagedContainer() = default;
	~PlacesManagedContainer() = default;
	PlacesManagedContainer(const PlacesManagedContainer &) = delete;
	PlacesManagedContainer(PlacesManagedContainer &&) = delete;
	PlacesManagedContainer &operator=(const PlacesManagedContainer &) = delete;
	PlacesManagedContainer &operator=(const PlacesManagedContainer &&) = delete;

	/*!
	 * \brief clearInputPlaces
	 */
	void clearInputPlaces();

	/*!
	 * \brief Inserts a shared pointer of a place in the container.
	 * \param name - Indetifier of the place
	 * \param place - Shared pointer to a place.
	 */
	void insert(const std::string &name, std::shared_ptr<Place> place);

	/*!
	 * \brief Removes all places from the container.
	 */
	void clear();

	/*!
	 * Print the petri net places and number of tokens.
	 * \param o Output stream.
	 */
	void printState(std::ostream &o) const;

	/*!
	 * \brief Gets the number of tokens in a given place.
	 * \param place - identifier of a place.
	 * \return Number of tokens inside place.
	 */
	size_t getNumberOfTokens(const std::string &place) const;

	/*!
	 * \brief Increment the number of tokens in an input place.
	 * \param place - Identifier of the input place to increment.
	 */
	void incrementInputPlace(const std::string &place);

	/*!
	 * \brief Exports the collection of places.
	 * \param exporter - Exporter.
	 */
	void export_(IExporter &exporter) const;

	/*!
	 * \brief Translates a vector of names of places to a vector of weak pointers to those places.
	 * \param names Names of the places.
	 * \return Vector of weakt_ptr for each place given in "names".
	 */
	std::vector<WeakPtrPlace> getPlacesFromNames(const std::vector<std::string> &placesNames) const;

private:
	/*!
	 * Shared mutex to synchronize the access to the transitions(readers-writer lock).
	 */
	mutable std::shared_mutex m_placesMutex;

	/*!
	 * \brief Map of places.
	 * Insertion is only performed on construction and there are no removals during the lifetime of the net.
	 * On the other hand many access operations are expected.
	 */
	std::unordered_map<std::string, SharedPtrPlace> m_places;

	/*!
	 * \brief Vector with the input places.
	 * Insertions on construction. Otherwise (should remain) unchanged.
	 */
	std::vector<WeakPtrPlace> m_inputPlaces;
};
} // namespace ptne
