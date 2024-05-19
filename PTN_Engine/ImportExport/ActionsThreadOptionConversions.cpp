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

#include "PTN_Engine/ImportExport/ActionsThreadOptionConversions.h"
#include "PTN_Engine/PTN_Exception.h"

namespace ptne
{
using namespace std;

const string ActionsThreadOptionConversions::ACTIONS_THREAD_OPTION_SINGLE_THREAD = "SINGLE_THREAD";
const string ActionsThreadOptionConversions::ACTIONS_THREAD_OPTION_EVENT_LOOP = "EVENT_LOOP";
const string ActionsThreadOptionConversions::ACTIONS_THREAD_OPTION_DETACHED = "DETACHED";
const string ActionsThreadOptionConversions::ACTIONS_THREAD_OPTION_JOB_QUEUE = "JOB_QUEUE";

PTN_Engine::ACTIONS_THREAD_OPTION
ActionsThreadOptionConversions::toACTIONS_THREAD_OPTION(const string &actionsThreadOptionStr)
{
	using enum PTN_Engine::ACTIONS_THREAD_OPTION;
	if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_SINGLE_THREAD)
	{
		return SINGLE_THREAD;
	}
	else if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_EVENT_LOOP)
	{
		return EVENT_LOOP;
	}
	else if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_DETACHED)
	{
		return DETACHED;
	}
	else if (actionsThreadOptionStr == ACTIONS_THREAD_OPTION_JOB_QUEUE)
	{
		return JOB_QUEUE;
	}
	else
	{
		throw PTN_Exception("Could not convert " + actionsThreadOptionStr + " to ACTIONS_THREAD_OPTION");
	}
}

string ActionsThreadOptionConversions::toString(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	using enum PTN_Engine::ACTIONS_THREAD_OPTION;
	switch (actionsThreadOption)
	{
	default:
	{
		throw PTN_Exception("Could not convert to string");
	}
	case DETACHED:
	{
		return ACTIONS_THREAD_OPTION_DETACHED;
	}
	case SINGLE_THREAD:
	{
		return ACTIONS_THREAD_OPTION_SINGLE_THREAD;
	}
	case EVENT_LOOP:
	{
		return ACTIONS_THREAD_OPTION_EVENT_LOOP;
	}
	case JOB_QUEUE:
	{
		return ACTIONS_THREAD_OPTION_JOB_QUEUE;
	}
	}
}

} // namespace ptne
