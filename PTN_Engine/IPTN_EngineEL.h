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
#include <iostream>

namespace ptne
{

//!
//! \brief The IPTN_EngineEL class
//!
class IPTN_EngineEL
{
public:
	virtual ~IPTN_EngineEL() = default;
	virtual bool executeInt(const bool log = false, std::ostream &o = std::cout) = 0;
	virtual bool getNewInputReceived() const = 0;
	virtual PTN_Engine::ACTIONS_THREAD_OPTION getActionsThreadOption() const = 0;
};

} // namespace ptne
