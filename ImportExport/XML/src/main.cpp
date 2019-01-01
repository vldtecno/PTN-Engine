#include "pugixml.hpp"
#include <iostream>
#include <string>
#include <vector>

bool parseDocument(pugi::xml_document &doc, const std::wstring &filePath)
{
	using namespace pugi;
	using namespace std;
	xml_parse_result result = doc.load_file(filePath.c_str());

	if (!result)
	{
		return false;
	}
	return true;
}

std::wstring getAttributeValue(const pugi::xml_node &node, const std::wstring &attrName)
{
	return node.attribute(attrName.c_str()).value();
}

void collectDocumentInformation(const pugi::xml_document &doc)
{
	using namespace std;
	using namespace pugi;

	const xml_node root_node = doc.child(L"PTN-Engine");
	wcout << L"version: " << getAttributeValue(root_node, L"version") << endl;
	wcout << L"format: " << getAttributeValue(root_node, L"format") << endl;
	// PTN verify compatibility
	wcout << L"name: " << getAttributeValue(root_node, L"name") << endl;
	// PTN - set name?
}


void createPlaces(const pugi::xml_document &doc)
{
	using namespace std;
	using namespace pugi;

	for (const xml_node &place : doc.child(L"PTN-Engine").child(L"Places"))
	{
		wcout << getAttributeValue(place, L"name") << L" " << getAttributeValue(place, L"tokens") << L" "
			  << getAttributeValue(place, L"isInput") << L" " << getAttributeValue(place, L"onEnterAction") << L" "
			  << getAttributeValue(place, L"onExitAction") << endl;

		// Create place here
		// Place (getAttributeValue(place, L"name") ,getAttributeValue(place, L"tokens")
		// ,getAttributeValue(place, L"isInput") ,getAttributeValue(place, L"onEnterAction")
		// ,getAttributeValue(place, L"onExitAction") << endl;
	}
}

std::vector<std::wstring>
collectTransitionAttributes(const pugi::xml_node &transition, const std::wstring &attribute)
{
	using namespace std;
	using namespace pugi;

	std::vector<std::wstring> activationConditions;
	for (xml_node activationCondition : transition.child(attribute.c_str()))
	{
		wcout << activationCondition.attribute(L"name").value() << endl;
		activationConditions.emplace_back(activationCondition.attribute(L"name").value());
	}

	return activationConditions;
}

void createTransitions(const pugi::xml_document &doc)
{
	using namespace std;
	using namespace pugi;


	for (xml_node transition : doc.child(L"PTN-Engine").child(L"Transitions"))
	{
		wcout << L"Transition: " << getAttributeValue(transition, L"name") << endl;

		vector<wstring> activationConditions = collectTransitionAttributes(transition, L"ActivationConditions");
		vector<wstring> inhibitorPlaces = collectTransitionAttributes(transition, L"InhibitorPlaces");
		vector<wstring> activationPlaces = collectTransitionAttributes(transition, L"ActivationPlaces");
		vector<wstring> destinationPlaces = collectTransitionAttributes(transition, L"DestinationPlaces");


		// Create Transition
		// Transition (getAttributeValue(transition,L"name"), activationConditions, inhibitorPlaces,
		// activationPlaces, destinationPlaces);
	}
}

int main()
{
	using namespace pugi;
	using namespace std;

	xml_document doc;
	if (!parseDocument(doc, L"/home/eduardo/Bancada/PTN-Engine-Interpreter/assets/PetriNet.xml"))
	{
		cout << "Failed to parse document" << endl;
		return -1;
	}

	cout << "XML parsed successfully" << endl;

	collectDocumentInformation(doc);
	createPlaces(doc);
	createTransitions(doc);


	return 0;
}
