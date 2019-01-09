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

#include "XML/XML_Importer.h"
#include <PTN_Engine/PTN_Engine.h>

namespace ptne
{
using namespace pugi;
using namespace std;

XML_Importer::XML_Importer(const string &filePath)
{
	xml_parse_result result = m_document.load_file(filePath.c_str());

	if (!result)
	{
		throw runtime_error(result.description());
	}

	// collectDocumentInformation(petriNet, doc);
}

// void XML_Importer::collectDocumentInformation(PTN_Engine &petriNet, const xml_document &doc) const
//{
//	const xml_node root_node = doc.child("PTN-Engine");
//	// cout << "version: " << getAttributeValue(root_node, "version") << endl;
//	// cout << "format: " << getAttributeValue(root_node, "format") << endl;
//	//// PTN verify compatibility
//	// cout << "name: " << getAttributeValue(root_node, "name") << endl;
//	// PTN - set name?
//}

XML_Importer::~XML_Importer()
{
}

vector<XML_Importer::PlaceInfo> XML_Importer::getPlaces() const
{
	vector<PlaceInfo> placesInfoCollection;
	for (const xml_node &place : m_document.child("PTN-Engine").child("Places"))
	{
		PlaceInfo placeInfo;
		bool isInput = false;
		const string isInputStr = getAttributeValue(place, "isInput");
		if (!isInputStr.empty())
		{
			isInput = getAttributeValue(place, "isInput") == "true" ? true : false;
		}

		size_t numberOfTokens = 0;
		const string numberOfTokensStr = getAttributeValue(place, "tokens");
		if (!numberOfTokensStr.empty())
		{
			numberOfTokens = static_cast<size_t>(std::atol(numberOfTokensStr.c_str()));
		}

		get<0>(placeInfo) = getAttributeValue(place, "name");
		get<1>(placeInfo) = numberOfTokens;
		get<2>(placeInfo) = getAttributeValue(place, "onEnterAction");
		get<3>(placeInfo) = getAttributeValue(place, "onExitAction");
		get<4>(placeInfo) = isInput;

		placesInfoCollection.emplace_back(placeInfo);
	}
	return placesInfoCollection;
}

vector<XML_Importer::TransitionInfo> XML_Importer::getTransitions() const
{
	vector<TransitionInfo> transitionInfoCollection;
	for (xml_node transition : m_document.child("PTN-Engine").child("Transitions"))
	{
		TransitionInfo transitionInfo;
		const vector<string> activationConditions =
		collectTransitionAttributes(transition, "ActivationConditions");
		const vector<string> inhibitorPlaces = collectTransitionAttributes(transition, "InhibitorPlaces");
		const auto activationArcs = collectArcAttributes(transition, "ActivationPlaces");
		const auto destinationArcs = collectArcAttributes(transition, "DestinationPlaces");

		get<0>(transitionInfo) = activationArcs.first;
		get<1>(transitionInfo) = activationArcs.second;
		get<2>(transitionInfo) = destinationArcs.first;
		get<3>(transitionInfo) = destinationArcs.second;
		get<4>(transitionInfo) = inhibitorPlaces;
		get<5>(transitionInfo) = activationConditions;

		transitionInfoCollection.emplace_back(transitionInfo);
	}
	return transitionInfoCollection;
}

string XML_Importer::getAttributeValue(const xml_node &node, const string &attrName) const
{
	return node.attribute(attrName.c_str()).value();
}

vector<string> XML_Importer::collectTransitionAttributes(const xml_node &transition, const string &attribute) const
{
	vector<string> activationConditions;
	for (xml_node activationCondition : transition.child(attribute.c_str()))
	{
		activationConditions.emplace_back(activationCondition.attribute("name").value());
	}

	return activationConditions;
}

pair<vector<string>, vector<size_t>>
XML_Importer::collectArcAttributes(const xml_node &transition, const string &attribute) const
{
	vector<string> places;
	vector<size_t> weights;
	for (xml_node activationCondition : transition.child(attribute.c_str()))
	{
		places.emplace_back(activationCondition.attribute("name").value());

		size_t weight = 1;
		string weightStr = activationCondition.attribute("weight").value();
		if (!weightStr.empty())
		{
			weight = static_cast<size_t>(std::atol(weightStr.c_str()));
		}

		weights.emplace_back(weight);
	}

	return pair<vector<string>, vector<size_t>>{ places, weights };
}


} // namespace ptne
