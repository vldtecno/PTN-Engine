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

#include "PTN_Engine/PlacesManager.h"
#include "PTN_Engine/PTN_Exception.h"
#include "PTN_Engine/Utilities/DetectRepeated.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <mutex>

namespace ptne
{
using namespace std;

PlacesManager::~PlacesManager() = default;
PlacesManager::PlacesManager() = default;

bool PlacesManager::contains(const string &itemName) const
{
	shared_lock itemsGuard(m_itemsMutex);
	return ManagerBase<Place>::contains(itemName);
}

void PlacesManager::insert(const shared_ptr<Place> &spPlace)
{
	unique_lock itemsGuard(m_itemsMutex);
	ManagerBase<Place>::insert(spPlace);
	if (spPlace->isInputPlace())
	{
		m_inputPlaces.push_back(spPlace);
	}
}

void PlacesManager::clear()
{
	unique_lock itemsGuard(m_itemsMutex);
	ManagerBase<Place>::clear();
	m_inputPlaces.clear();
}

shared_ptr<Place> PlacesManager::getPlace(const string &placeName) const
{
	shared_lock itemsGuard(m_itemsMutex);
	return ManagerBase<Place>::getItem(placeName);
}

void PlacesManager::clearInputPlaces() const
{
	unique_lock placesGuard(m_itemsMutex);
	for (const WeakPtrPlace &place : m_inputPlaces)
	{
		SharedPtrPlace spPlace = lockWeakPtr(place);
		spPlace->setNumberOfTokens(0);
	}
}

void PlacesManager::printState(ostream &o) const
{
	shared_lock placesGuard(m_itemsMutex);
	o << "Place; Tokens" << endl;
	for (const auto &[placeName, place] : m_items)
	{
		o << placeName.c_str() << ": " << place->getNumberOfTokens() << endl;
	}
	o << endl << endl;
}

size_t PlacesManager::getNumberOfTokens(const string &place) const
{
	shared_lock placesGuard(m_itemsMutex);
	if (!m_items.contains(place))
	{
		throw InvalidNameException(place);
	}
	return m_items.at(place)->getNumberOfTokens();
}

void PlacesManager::incrementInputPlace(const string &place)
{
	unique_lock placesGuard(m_itemsMutex);
	if (!m_items.contains(place))
	{
		throw InvalidNameException(place);
	}
	if (!m_items.at(place)->isInputPlace())
	{
		throw NotInputPlaceException(place);
	}
	m_items.at(place)->enterPlace(1);
}

vector<PlaceProperties> PlacesManager::getPlacesProperties() const
{
	shared_lock placesGuard(m_itemsMutex);

	vector<PlaceProperties> placesProperties;
	for (const auto &[_, place] : m_items)
	{
		placesProperties.push_back(place->placeProperties());
	}
	return placesProperties;
}

vector<WeakPtrPlace> PlacesManager::getPlaces(const vector<string> &placesNames) const
{
	shared_lock placesGuard(m_itemsMutex);

	utility::detectRepeatedNames<string, RepeatedPlaceNamesException>(placesNames);

	vector<WeakPtrPlace> placesVector;
	for (const auto &placeName : placesNames)
	{
		if (!m_items.contains(placeName))
		{
			throw InvalidNameException(placeName);
		}
		placesVector.push_back(m_items.at(placeName));
	}
	return placesVector;
}
} // namespace ptne
