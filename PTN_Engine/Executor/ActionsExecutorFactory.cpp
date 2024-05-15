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

#include "PTN_Engine/Executor/ActionsExecutorFactory.h"
#include "PTN_Engine/Executor/DetachedExecutor.h"
#include "PTN_Engine/Executor/JobQueueExecutor.h"
#include "PTN_Engine/Executor/SingleThreadExecutor.h"
#include "PTN_Engine/PTN_Exception.h"


namespace ptne
{

using namespace std;

unique_ptr<IActionsExecutor>
ActionsExecutorFactory::createExecutor(PTN_Engine::ACTIONS_THREAD_OPTION actionsThreadOption)
{
	switch (actionsThreadOption)
	{
	default:
	{
		throw PTN_Exception("Invalid configuration");
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::SINGLE_THREAD:
	case PTN_Engine::ACTIONS_THREAD_OPTION::EVENT_LOOP:
	{
		return make_unique<SingleThreadExecutor>();
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::JOB_QUEUE:
	{
		return make_unique<JobQueueExecutor>();
	}
	case PTN_Engine::ACTIONS_THREAD_OPTION::DETACHED:
	{
		return make_unique<DetachedExecutor>();
	}
	}
}


} // namespace ptne
