/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
 * Copyright (c) 2021 Kale Evans
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

#include "PTN_Engine/PTN_EngineImp.h"
#include "PTN_Engine/IExporter.h"
#include "PTN_Engine/IImporter.h"
#include "PTN_Engine/JobQueue/JobQueue.h"
#include "PTN_Engine/Place.h"
#include "PTN_Engine/Transition.h"
#include "PTN_Engine/Utilities/DetectRepeated.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <algorithm>
#include <chrono>
#include <future>
#include <mutex>
#include <random>
#include <thread>


namespace ptne
{
using namespace std;

PTN_Engine::PTN_EngineImp::PTN_EngineImp(PTN_Engine &parent, ACTIONS_THREAD_OPTION actionsThreadOption)
: m_ptnEngine(parent)
, m_stop(true)
, m_eventLoopThread(nullptr)
{
	setActionsThreadOption(actionsThreadOption);
}

PTN_Engine::PTN_EngineImp::~PTN_EngineImp()
{
	if (m_eventLoopThread != nullptr && m_eventLoopThread->joinable())
	{
		m_eventLoopThread->join();
	}
}

void PTN_Engine::PTN_EngineImp::createTransition(const vector<string> &activationPlaces,
                                                 const vector<size_t> &activationWeights,
                                                 const vector<string> &destinationPlaces,
                                                 const vector<size_t> &destinationWeights,
                                                 const vector<string> &inhibitorPlaces,
                                                 const vector<ConditionFunction> &additionalConditions)
{
	createTransitionImp(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
						inhibitorPlaces, createAnonymousConditions(additionalConditions));
}

void PTN_Engine::PTN_EngineImp::createTransitionImp(
const vector<string> &activationPlaces,
const vector<size_t> &activationWeights,
const vector<string> &destinationPlaces,
const vector<size_t> &destinationWeights,
const vector<string> &inhibitorPlaces,
const vector<pair<string, ConditionFunction>> &additionalConditions)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);

	vector<WeakPtrPlace> activationPlacesVector = getPlacesFromNames(activationPlaces);

	vector<WeakPtrPlace> destinationPlacesVector = getPlacesFromNames(destinationPlaces);

	vector<WeakPtrPlace> inhibitorPlacesVector = getPlacesFromNames(inhibitorPlaces);

	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	m_transitions.push_back(Transition(activationPlacesVector, activationWeights, destinationPlacesVector,
									   destinationWeights, inhibitorPlacesVector, additionalConditions));
}

void PTN_Engine::PTN_EngineImp::createTransition(const vector<string> &activationPlaces,
                                                 const vector<size_t> &activationWeights,
                                                 const vector<string> &destinationPlaces,
                                                 const vector<size_t> &destinationWeights,
                                                 const vector<string> &inhibitorPlaces,
                                                 const vector<string> &additionalConditions)
{
	createTransitionImp(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
						inhibitorPlaces, getConditionFunctions(additionalConditions));
}

void PTN_Engine::PTN_EngineImp::createTransitionImp(const vector<string> &activationPlaces,
                                                    const vector<size_t> &activationWeights,
                                                    const vector<string> &destinationPlaces,
                                                    const vector<size_t> &destinationWeights,
                                                    const vector<string> &inhibitorPlaces,
                                                    const vector<string> &additionalConditions)
{
	createTransitionImp(activationPlaces, activationWeights, destinationPlaces, destinationWeights,
						inhibitorPlaces, getConditionFunctions(additionalConditions));
}

vector<Transition *> PTN_Engine::PTN_EngineImp::collectEnabledTransitionsRandomly()
{
	vector<Transition *> enabledTransitions;
	for (Transition &transition : m_transitions)
	{
		if (transition.isEnabled())
		{
			enabledTransitions.push_back(&transition);
		}
	}
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(enabledTransitions.begin(), enabledTransitions.end(), minstd_rand0(seed));
	return enabledTransitions;
}

void PTN_Engine::PTN_EngineImp::createPlace(const string &name,
                                            const size_t initialNumberOfTokens,
                                            ActionFunction onEnterAction,
                                            ActionFunction onExitAction,
                                            const bool input)
{
    createPlaceImp(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::PTN_EngineImp::createPlaceImp(const string &name,
                                               const size_t initialNumberOfTokens,
                                               ActionFunction onEnterAction,
                                               ActionFunction onExitAction,
                                               const bool input)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	if (m_places.find(name) != m_places.end())
	{
		throw RepeatedPlaceException(name);
	}

	SharedPtrPlace place =
	make_shared<Place>(m_ptnEngine, initialNumberOfTokens, onEnterAction, onExitAction, input);

	m_places[name] = place;
	if (place->isInputPlace())
	{
		m_inputPlaces.push_back(place);
	}
}

void PTN_Engine::PTN_EngineImp::createPlaceStr(const string &name,
                                               const size_t initialNumberOfTokens,
                                               const string &onEnterAction,
                                               const string &onExitAction,
                                               const bool input)
{
    createPlaceStrImp(name, initialNumberOfTokens, onEnterAction, onExitAction, input);
}

void PTN_Engine::PTN_EngineImp::createPlaceStrImp(const string &name,
                                                  const size_t initialNumberOfTokens,
                                                  const string &onEnterActionName,
                                                  const string &onExitActionName,
                                                  const bool input)
{
    unique_lock<shared_timed_mutex> guard(m_mutex);

	ActionFunction onEnterAction = getActionFunction(onEnterActionName);
	ActionFunction onExitAction = getActionFunction(onExitActionName);

	if (m_places.find(name) != m_places.end())
	{
		throw RepeatedPlaceException(name);
	}

	SharedPtrPlace place = make_shared<Place>(m_ptnEngine, initialNumberOfTokens, onEnterActionName, onEnterAction,
											  onExitActionName, onExitAction, input);

	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	m_places[name] = place;
	if (place->isInputPlace())
	{
		m_inputPlaces.push_back(place);
	}
}

void PTN_Engine::PTN_EngineImp::registerAction(const string &name, ActionFunction action)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	if (name.empty())
	{
		throw InvalidFunctionNameException(name);
	}

	if (m_actions.find(name) != m_actions.end())
	{
		throw RepeatedActionException(name);
	}

	// verificar acções repetidas
	m_actions[name] = action;
}

void PTN_Engine::PTN_EngineImp::registerCondition(const string &name, ConditionFunction condition)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	if (name.empty())
	{
		throw InvalidFunctionNameException(name);
	}

	if (m_conditions.find(name) != m_conditions.end())
	{
		throw RepeatedConditionException(name);
	}

	// verificar se há repetidos?
	m_conditions[name] = condition;
}

void PTN_Engine::PTN_EngineImp::execute(const bool log, ostream &o)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);

	if (m_stop == false) // already running
	{
		return;
	}

	if (m_eventLoopThread && m_eventLoopThread->joinable())
	{
		stop();
	}
	m_stop = false;

	auto execution = [this, log, &o]()
	{
		do
		{
			bool firedAtLeastOneTransition = false;
			{
				std::unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

				m_newInputReceived = false;

				if (log)
				{
					printState(o);
				}

				// TODO: Create an iterator for active transitions
				// Safe to use raw pointers here. Nothing justifies deleting a
				// transition from m_transitions, so there should never be an
				// invalid pointer. At the moment this is only single threaded,
				// so synchronization problems are not an issue.
				vector<Transition *> enabledTransitions(collectEnabledTransitionsRandomly());

				for (Transition *transition : enabledTransitions)
				{
					firedAtLeastOneTransition |= transition->execute();
				}
			}

			if (!firedAtLeastOneTransition)
			{
				std::unique_lock<mutex> eventNotifierGuard(m_eventNotifierMutex);
				m_eventNotifier.wait_for(eventNotifierGuard, 100ms, [this] { return m_newInputReceived; });
			}
		} while (!m_stop);
	};

	m_eventLoopThread = std::make_unique<std::thread>(std::thread(execution));
}

void PTN_Engine::PTN_EngineImp::stop()
{
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	m_stop = true;
	if (m_eventLoopThread != nullptr && m_eventLoopThread->joinable())
	{
		m_eventLoopThread->join();
		m_eventLoopThread.release();
	}
}

void PTN_Engine::PTN_EngineImp::clearInputPlaces()
{
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	for (const WeakPtrPlace &place : m_inputPlaces)
	{
		SharedPtrPlace spPlace = lockWeakPtr(place);
		spPlace->setNumberOfTokens(0);
	}

	std::unique_lock<mutex> eventNotifierGuard(m_eventNotifierMutex);
	m_newInputReceived = false;
}

ActionFunction PTN_Engine::PTN_EngineImp::getActionFunction(const string &name) const
{
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	if (name.empty())
	{
		return nullptr;
	}

	if (m_actions.find(name) == m_actions.end())
	{
		throw InvalidFunctionNameException(name);
	}
	return m_actions.at(name);
}

vector<pair<string, ConditionFunction>>
PTN_Engine::PTN_EngineImp::getConditionFunctions(const vector<string> &names) const
{
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	vector<pair<string, ConditionFunction>> conditions;
	for_each(names.cbegin(), names.cend(),
			 [&](const string &name)
			 {
				 if (name.empty())
				 {
					 return;
				 }

				 if (m_conditions.find(name) == m_conditions.end())
				 {
					 throw InvalidFunctionNameException(name);
				 }
				 conditions.push_back(pair<string, ConditionFunction>(name, m_conditions.at(name)));
			 });
	return conditions;
}

size_t PTN_Engine::PTN_EngineImp::getNumberOfTokens(const string &place) const
{
	shared_lock<shared_timed_mutex> guard(m_mutex);
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	if (m_places.find(place) == m_places.end())
	{
		throw InvalidNameException(place);
	}
	return m_places.at(place)->getNumberOfTokens();
}

void PTN_Engine::PTN_EngineImp::incrementInputPlace(const string &place)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	if (m_places.find(place) == m_places.end())
	{
		throw InvalidNameException(place);
	}
	if (!m_places.at(place)->isInputPlace())
	{
		throw NotInputPlaceException(place);
	}
	m_places.at(place)->enterPlace(1);

	std::unique_lock<mutex> eventNotifierGuard(m_eventNotifierMutex);
	m_newInputReceived = true;
	m_eventNotifier.notify_all();
}

vector<WeakPtrPlace> PTN_Engine::PTN_EngineImp::getPlacesFromNames(const vector<string> &placesNames) const
{
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

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

void PTN_Engine::PTN_EngineImp::export_(IExporter &exporter) const
{
	shared_lock<shared_timed_mutex> guard(m_mutex);
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	std::string actionsThreadOptionStr;
	if (!ACTIONS_THREAD_OPTION_toString(m_actionsThreadOption, actionsThreadOptionStr))
	{
		throw PTN_Exception("Invalid ACTIONS_THREAD_OPTION");
	}
	exporter.exportActionsThreadOption(actionsThreadOptionStr);

	exportPlaces(exporter);
	exportTransitions(exporter);
}

void PTN_Engine::PTN_EngineImp::exportPlaces(IExporter &exporter) const
{
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	for (const auto &place : m_places)
	{
		string onEnter;
		auto itFound = find_if(m_actions.cbegin(), m_actions.cend(),
							   [&](const auto &it) { return it.first == place.second->getOnEnterActionName(); });
		if (itFound != m_actions.cend())
		{
			onEnter = itFound->first; // on enter
		}

		string onExit;
		itFound = find_if(m_actions.cbegin(), m_actions.cend(),
						  [&](const auto &it) { return it.first == place.second->getOnExitActionName(); });
		if (itFound != m_actions.cend())
		{
			onExit = itFound->first; // on enter
		}

		exporter.exportPlace(place.first, std::to_string(place.second->getNumberOfTokens()),
							 place.second->isInputPlace() ? "true" : "false", onEnter, onExit);
	}
}

void PTN_Engine::PTN_EngineImp::exportTransitions(IExporter &exporter) const
{
	shared_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);

	for (const auto &transition : m_transitions)
	{
		vector<tuple<string, size_t>> activationPlacesExport;
		for (const auto &activationPlace : transition.getActivationPlaces())
		{
			tuple<string, size_t> activationPlaceExport;
			get<0>(activationPlaceExport) = findName(get<0>(activationPlace).lock(), m_places);
			get<1>(activationPlaceExport) = get<1>(activationPlace);
			activationPlacesExport.emplace_back(activationPlaceExport);
		}

		vector<tuple<string, size_t>> destinationPlacesExport;
		for (const auto &destinationPlace : transition.getDestinationPlaces())
		{
			tuple<string, size_t> destinationPlaceExport;
			get<0>(destinationPlaceExport) = findName(get<0>(destinationPlace).lock(), m_places);
			get<1>(destinationPlaceExport) = get<1>(destinationPlace);
			destinationPlacesExport.emplace_back(destinationPlaceExport);
		}

		vector<string> activationConditionsNames;
		for (const auto &activationCondition : transition.getAdditionalActivationConditions())
		{
			if (m_conditions.find(activationCondition.first) != m_conditions.end())
			{
				activationConditionsNames.emplace_back(activationCondition.first);
			}
		}

		vector<string> inhibitorPlacesNames;
		for (const auto &inhibitorPlace : transition.getInhibitorPlaces())
		{
			inhibitorPlacesNames.emplace_back(findName(inhibitorPlace.lock(), m_places));
		}

		exporter.exportTransition(activationPlacesExport, destinationPlacesExport, activationConditionsNames,
								  inhibitorPlacesNames);
	}
}

void PTN_Engine::PTN_EngineImp::import(const IImporter &importer)
{
	// TODO: Not exception safe, maybe this should go to a constructor instead.
	clearNet();

	ACTIONS_THREAD_OPTION actionThreadOption = m_actionsThreadOption;
	std::string actionsThreadOptionStr = importer.getActionsThreadOption();
	if (ACTIONS_THREAD_OPTION_fromString(actionsThreadOptionStr, actionThreadOption))
	{
		setActionsThreadOption(actionThreadOption);
	}
	else
	{
		throw PTN_Exception("Invalid input ACTIONS_THREAD_OPTION string");
	}

	// create places
	for (const IImporter::PlaceInfo &placeInfo : importer.getPlaces())
	{
		createPlaceStrImp(get<0>(placeInfo), get<1>(placeInfo), get<2>(placeInfo), get<3>(placeInfo),
						  get<4>(placeInfo));
	}

	// create transitions
	for (const IImporter::TransitionInfo &transitionInfo : importer.getTransitions())
	{
		createTransitionImp(get<0>(transitionInfo), get<1>(transitionInfo), get<2>(transitionInfo),
							get<3>(transitionInfo), get<4>(transitionInfo), get<5>(transitionInfo));
	}
}

bool PTN_Engine::PTN_EngineImp::isEventLoopRunning() const
{
	unique_lock<shared_timed_mutex> guard(m_mutex);
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);
	return !m_stop;
}

void PTN_Engine::PTN_EngineImp::clearNet()
{
	unique_lock<shared_timed_mutex> eventLoopGuard(m_eventLoopMutex);
	m_inputPlaces.clear();
	m_places.clear();
	m_transitions.clear();
}

PTN_Engine::PTN_EngineImp::InvalidNameException::InvalidNameException(const string &name)
: PTN_Exception("Invalid name: " + name + ".")
{
}

PTN_Engine::PTN_EngineImp::RepeatedPlaceNamesException::RepeatedPlaceNamesException()
: PTN_Exception("Tried to create transition with repeated places.")
{
}

PTN_Engine::PTN_EngineImp::NotInputPlaceException::NotInputPlaceException(const string &name)
: PTN_Exception(name + " is not an input place.")
{
}

PTN_Engine::PTN_EngineImp::RepeatedPlaceException::RepeatedPlaceException(const string &name)
: PTN_Exception("Trying to add an already existing place: " + name + ".")
{
}

PTN_Engine::PTN_EngineImp::RepeatedActionException::RepeatedActionException(const string &name)
: PTN_Exception("Trying to add an already existing action: " + name + ".")
{
}

PTN_Engine::PTN_EngineImp::RepeatedConditionException::RepeatedConditionException(const string &name)
: PTN_Exception("Trying to add an already existing condition: " + name + ".")
{
}

PTN_Engine::PTN_EngineImp::InvalidFunctionNameException::InvalidFunctionNameException(const string &name)
: PTN_Exception("The function is not yet registered: " + name + ".")
{
}

void PTN_Engine::PTN_EngineImp::printState(ostream &o) const
{
	o << "Place; Tokens" << endl;
	for (const auto &p : m_places)
	{
		o << p.first.c_str() << ": " << p.second->getNumberOfTokens() << endl;
	}
	o << endl << endl;
}

const vector<pair<string, ConditionFunction>>
PTN_Engine::PTN_EngineImp::createAnonymousConditions(const vector<ConditionFunction> &conditions) const
{
	vector<pair<string, ConditionFunction>> anonymousConditionsVector;
	for (const auto &condition : conditions)
	{
		anonymousConditionsVector.emplace_back(pair<string, ConditionFunction>("", condition));
	}
	return anonymousConditionsVector;
}

void PTN_Engine::PTN_EngineImp::setActionsThreadOption(const ACTIONS_THREAD_OPTION actionsThreadOption)
{
	unique_lock<shared_timed_mutex> guard(m_mutex);

	if (m_actionsThreadOption == actionsThreadOption)
	{
		return;
	}
	if (actionsThreadOption == ACTIONS_THREAD_OPTION::JOB_QUEUE && m_jobQueue == nullptr)
	{
		m_jobQueue = make_unique<JobQueue>();
	}
	else if (actionsThreadOption != ACTIONS_THREAD_OPTION::JOB_QUEUE && m_jobQueue != nullptr)
	{
		m_jobQueue.reset();
	}
	m_actionsThreadOption = actionsThreadOption;
}

PTN_Engine::ACTIONS_THREAD_OPTION PTN_Engine::PTN_EngineImp::getActionsThreadOption()
{
	return m_actionsThreadOption;
}

void PTN_Engine::PTN_EngineImp::addJob(const ActionFunction &actionFunction)
{
	if (m_actionsThreadOption != ACTIONS_THREAD_OPTION::JOB_QUEUE || m_jobQueue == nullptr ||
		!m_jobQueue->isActive())
	{
		throw PTN_Exception("addJob incorrectly called");
	}

	m_jobQueue->addJob(actionFunction);
}

bool PTN_Engine::PTN_EngineImp::ACTIONS_THREAD_OPTION_fromString(const std::string &actionsThreadOptionStr,
																 ACTIONS_THREAD_OPTION &out)
{
	if (actionsThreadOptionStr == "DETACHED")
	{
		out = ACTIONS_THREAD_OPTION::DETACHED;
	}
	else if (actionsThreadOptionStr == "EVENT_LOOP")
	{
		out = ACTIONS_THREAD_OPTION::EVENT_LOOP;
	}
	else if (actionsThreadOptionStr == "JOB_QUEUE")
	{
		out = ACTIONS_THREAD_OPTION::JOB_QUEUE;
	}
	else
	{
		return false;
	}
	return true;
}

bool PTN_Engine::PTN_EngineImp::ACTIONS_THREAD_OPTION_toString(const ACTIONS_THREAD_OPTION actionsThreadOption,
															   std::string &out)
{
	switch (actionsThreadOption)
	{
	case ACTIONS_THREAD_OPTION::DETACHED:
		out = "DETACHED";
		break;
	case ACTIONS_THREAD_OPTION::EVENT_LOOP:
		out = "EVENT_LOOP";
		break;
	case ACTIONS_THREAD_OPTION::JOB_QUEUE:
		out = "JOB_QUEUE";
		break;
	default:
		return false;
		break;
	}
	return true;
}

} // namespace ptne
