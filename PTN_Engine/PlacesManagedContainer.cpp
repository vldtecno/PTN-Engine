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

#include "PTN_Engine/PlacesManagedContainer.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/PTN_Exception.h"
#include "PTN_Engine/Utilities/DetectRepeated.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <mutex>

namespace ptne
{
using namespace std;

void PlacesManagedContainer::clearInputPlaces()
{
	unique_lock<shared_mutex> transitionsGuard(m_placesMutex);
	for (const WeakPtrPlace &place : m_inputPlaces)
	{
		SharedPtrPlace spPlace = lockWeakPtr(place);
		spPlace->setNumberOfTokens(0);
	}
}

void PlacesManagedContainer::insert(shared_ptr<Place> place)
{
	unique_lock<shared_mutex> transitionsGuard(m_placesMutex);
	const auto& placeName = place->getName();
	if (m_places.find(placeName) != m_places.end())
	{
		throw RepeatedPlaceException(placeName);
	}

	m_places[placeName] = place;
	if (place->isInputPlace())
	{
		m_inputPlaces.push_back(place);
	}
}

void PlacesManagedContainer::clear()
{
	unique_lock<shared_mutex> transitionsGuard(m_placesMutex);
	m_inputPlaces.clear();
	m_places.clear();
}

void PlacesManagedContainer::printState(ostream &o) const
{
	shared_lock<shared_mutex> transitionsGuard(m_placesMutex);
	o << "Place; Tokens" << endl;
	for (const auto &p : m_places)
	{
		o << p.first.c_str() << ": " << p.second->getNumberOfTokens() << endl;
	}
	o << endl << endl;
}

size_t PlacesManagedContainer::getNumberOfTokens(const string &place) const
{
	shared_lock<shared_mutex> transitionsGuard(m_placesMutex);
	if (m_places.find(place) == m_places.end())
	{
		throw InvalidNameException(place);
	}
	return m_places.at(place)->getNumberOfTokens();
}

void PlacesManagedContainer::incrementInputPlace(const string &place)
{
	unique_lock<shared_mutex> transitionsGuard(m_placesMutex);
	if (m_places.find(place) == m_places.end())
	{
		throw InvalidNameException(place);
	}
	if (!m_places.at(place)->isInputPlace())
	{
		throw NotInputPlaceException(place);
	}
	m_places.at(place)->enterPlace(1);
}

void PlacesManagedContainer::export_(IExporter &exporter) const
{
	shared_lock<shared_mutex> transitionsGuard(m_placesMutex);
	for (const auto &place : m_places)
	{
		place.second->export_(exporter);
	}
}

vector<WeakPtrPlace> PlacesManagedContainer::getPlacesFromNames(const vector<string> &placesNames) const
{
	shared_lock<shared_mutex> transitionsGuard(m_placesMutex);

	utility::detectRepeatedNames<string, RepeatedPlaceNamesException>(placesNames);

	vector<WeakPtrPlace> placesVector;
	for (const auto &placeName : placesNames)
	{
		if (m_places.find(placeName) == m_places.end())
		{
			throw InvalidNameException(placeName);
		}
		placesVector.push_back(m_places.at(placeName));
	}
	return placesVector;
}
} // namespace ptne
