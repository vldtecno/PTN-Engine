/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019 Eduardo Valgôde
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

#include "XML/XML_Exporter.h"
#include <PTN_Engine/PTN_Engine.h>
#include <pugixml.hpp>
#include <vector>

namespace ptne
{
using namespace pugi;
using namespace std;

XML_Exporter::XML_Exporter()
{
	m_rootNode = m_document.append_child("PTN-Engine");
	m_rootNode.append_attribute("version").set_value("2.0");
	m_rootNode.append_attribute("format").set_value("1");
	m_rootNode.append_attribute("name").set_value("blah");
	m_placesNode = m_rootNode.append_child("Places");
	m_transitionsNode = m_rootNode.append_child("Transitions");
}

XML_Exporter::~XML_Exporter()
{
}

void XML_Exporter::exportPlace(const string &name,
                               const string &tokens,
                               const string &isInput,
                               const string &onEnterAction,
                               const string &onExitAction)
{
	xml_node placeNode = m_placesNode.append_child("Place");
	placeNode.append_attribute("name").set_value(name.c_str());
	placeNode.append_attribute("tokens").set_value(tokens.c_str());
	placeNode.append_attribute("input").set_value(isInput.c_str());
	placeNode.append_attribute("onEnterAction").set_value(onEnterAction.c_str());
	placeNode.append_attribute("onExitAction").set_value(onExitAction.c_str());
}

void XML_Exporter::exportTransition(const vector<tuple<string, size_t>> &activationPlaces,
                                    const vector<tuple<string, size_t>> &destinationPlaces,
                                    const vector<string> &activationConditions,
                                    const vector<string> &inhibitorPlaces)
{
    xml_node transitionNode = m_transitionsNode.append_child("Transition");

	xml_node activationPlacesNode = transitionNode.append_child("ActivationPlaces");
	for (const auto activationPlace : activationPlaces)
	{
		xml_node activationPlaceNode = activationPlacesNode.append_child("ActivationPlace");
		activationPlaceNode.append_attribute("name").set_value(get<0>(activationPlace).c_str());
		size_t weight = get<1>(activationPlace);
		activationPlaceNode.append_attribute("weight").set_value(to_string(weight).c_str());
	}

	xml_node destinationPlacesNode = transitionNode.append_child("DestinationPlaces");
	for (const auto destinationPlace : destinationPlaces)
	{
		xml_node destinationPlaceNode = destinationPlacesNode.append_child("DestinationPlace");
		destinationPlaceNode.append_attribute("name").set_value(get<0>(destinationPlace).c_str());
		size_t weight = get<1>(destinationPlace);
		destinationPlaceNode.append_attribute("weight").set_value(to_string(weight).c_str());
	}

	xml_node activationConditionsNode = transitionNode.append_child("ActivationConditions");
	for (const auto activationCondition : activationConditions)
	{
		xml_node activationConditionNode = activationConditionsNode.append_child("ActivationCondition");
		activationConditionNode.append_attribute("name").set_value(activationCondition.c_str());
	}

	xml_node inhibitorPlacesNode = transitionNode.append_child("InhibitorPlaces");
	for (const auto inhibitorPlace : inhibitorPlaces)
	{
		xml_node inhibitorPlaceNode = inhibitorPlacesNode.append_child("InhibitorPlace");
		inhibitorPlaceNode.append_attribute("name").set_value(inhibitorPlace.c_str());
	}
}

void XML_Exporter::saveFile(const string &filePath) const
{
	m_document.save_file(filePath.c_str());
}

} // namespace ptne
