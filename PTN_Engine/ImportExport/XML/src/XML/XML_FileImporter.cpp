/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2019-2024 Eduardo Valgôde
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

#include "PTN_Engine/PTN_Engine.h"
#include "PTN_Engine/PTN_Exception.h"
#include "XML/XML_FileImporter.h"
#include <charconv>
#include <iterator>
#include <algorithm>

using namespace pugi;
using namespace std;
using namespace ptne;

namespace
{
string getAttributeValue(const xml_node &node, const string &attrName)
{
	return node.attribute(attrName.c_str()).value();
}

vector<string> collectTransitionAttributes(const xml_node &transition, const string &attribute)
{
	vector<string> activationConditions;
	const auto &childAttributes = transition.child(attribute.c_str());
	ranges::transform(begin(childAttributes), end(childAttributes), back_inserter(activationConditions),
			  [](const auto &activationCondition) { return activationCondition.attribute("name").value(); });
	return activationConditions;
}

vector<ArcProperties> collectArcAttributes(const xml_node &transition, const string &attribute, const ArcProperties::Type type)
{
	vector<ArcProperties> arcsProperties;
	for (xml_node activationCondition : transition.child(attribute.c_str()))
	{
		const string activationConditionName(activationCondition.attribute("name").value());
		size_t weight = 1;
		const string weightStr(activationCondition.attribute("weight").value());
		if (!string(activationCondition.attribute("weight").value()).empty())
		{
			weight = static_cast<size_t>(atol(weightStr.c_str()));
		}

		arcsProperties.emplace_back(weight,
			activationCondition.attribute("name").value(),
			activationConditionName,
			type);
	}
	return arcsProperties;
}

template <typename T>
T getNodeValue(const string &nodeName, const xml_node &xmlNode)
{
	return xmlNode.child(nodeName.c_str()).attribute("value").value();
}

template <>
bool getNodeValue(const string &nodeName, const xml_node &xmlNode)
{
	const string v = xmlNode.child(nodeName.c_str()).attribute("value").value();
	if (v == "true")
	{
		return true;
	}
	else if (v == "false")
	{
		return false;
	}
	else
	{
		throw PTN_Exception("Invalid value for " + nodeName + ": " + v);
	}
}

template <>
size_t getNodeValue(const string &nodeName, const xml_node &xmlNode)
{
	const string v = xmlNode.child(nodeName.c_str()).attribute("value").value();

	size_t result{};
	auto [ptr, ec] = from_chars(v.data(), v.data() + v.size(), result);

	if (ec != errc())
	{
		throw PTN_Exception("Could not convert from string to int");
	}

	return result;
}

} // namespace

namespace ptne
{
void XML_FileImporter::_import(const string &filePath, PTN_Engine &ptnEngine)
{
	xml_parse_result result = m_document.load_file(filePath.c_str());

	if (!result)
	{
		throw PTN_Exception(result.description());
	}

	IFileImporter::_import(ptnEngine);
}

string XML_FileImporter::importActionsThreadOption() const
{
	return m_document.child("PTN-Engine").attribute("actionsThreadOption").as_string();
}

vector<PlaceProperties> XML_FileImporter::importPlaces() const
{
	vector<PlaceProperties> placesInfoCollection;
	for (const xml_node &place : m_document.child("PTN-Engine").child("Places"))
	{
		PlaceProperties placeProperties;
		bool isInput = false;
		const string isInputStr = getAttributeValue(place, "input");
		if (!isInputStr.empty())
		{
			isInput = getAttributeValue(place, "input") == "true" ? true : false;
		}

		size_t numberOfTokens = 0;
		const string numberOfTokensStr = getAttributeValue(place, "tokens");
		if (!numberOfTokensStr.empty())
		{
			numberOfTokens = static_cast<size_t>(atol(numberOfTokensStr.c_str()));
		}

		placeProperties.name = getAttributeValue(place, "name");
		placeProperties.initialNumberOfTokens = numberOfTokens;
		placeProperties.onEnterActionFunctionName = getAttributeValue(place, "onEnterAction");
		placeProperties.onExitActionFunctionName = getAttributeValue(place, "onExitAction");
		placeProperties.input = isInput;

		placesInfoCollection.emplace_back(placeProperties);
	}
	return placesInfoCollection;
}

vector<TransitionProperties> XML_FileImporter::importTransitions() const
{
	vector<TransitionProperties> transitionInfoCollection;
	for (xml_node transition : m_document.child("PTN-Engine").child("Transitions"))
	{
		TransitionProperties transitionProperties;

		const auto activationConditions = collectTransitionAttributes(transition, "ActivationConditions");
		transitionProperties.name = getNodeValue<string>("Name", transition);
		transitionProperties.activationArcs = collectArcAttributes(transition, "ActivationPlaces",
																   ArcProperties::Type::ACTIVATION);
		transitionProperties.destinationArcs = collectArcAttributes(transition, "DestinationPlaces",
																	ArcProperties::Type::DESTINATION);
		transitionProperties.inhibitorArcs = collectArcAttributes(transition, "InhibitorPlaces",
																  ArcProperties::Type::INHIBITOR);;
		transitionProperties.additionalConditionsNames = activationConditions;
		transitionProperties.requireNoActionsInExecution = getNodeValue<bool>("RequireNoActionsInExecution", transition);
		transitionInfoCollection.emplace_back(transitionProperties);
	}
	return transitionInfoCollection;
}

vector<ArcProperties> XML_FileImporter::importArcs() const
{
	vector<ArcProperties> arcInfoCollection;
	for (xml_node arc : m_document.child("PTN-Engine").child("Arcs"))
	{
		ArcProperties arcProperties;

		arcProperties.placeName = getNodeValue<string>("Place", arc);
		arcProperties.transitionName = getNodeValue<string>("Transition", arc);
		arcProperties.weight = getNodeValue<size_t>("Weight", arc);

		using enum ArcProperties::Type;
		auto typeStr = getNodeValue<string>("Type", arc);
		if (typeStr == "Activation")
		{
			arcProperties.type = ACTIVATION;
		}
		else if (typeStr == "Bidirectional")
		{
			arcProperties.type = BIDIRECTIONAL;
		}
		else if (typeStr == "Destination")
		{
			arcProperties.type = DESTINATION;
		}
		else if (typeStr == "Inhibitor")
		{
			arcProperties.type = INHIBITOR;
		}
		else
		{
			throw PTN_Exception("Type string not supported");
		}

		arcInfoCollection.emplace_back(arcProperties);
	}
	return arcInfoCollection;
}

} // namespace ptne
