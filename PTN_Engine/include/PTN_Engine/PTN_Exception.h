/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017 Eduardo Valgôde
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

#ifdef _MSC_FULL_VER
// Disable warning about exporting PTN_Exception inheriting from
// std::runtime_error, which might have been compiled differently
// by the client linking to PTN Engine.
#pragma warning(disable : 4275)
#endif

#include "PTN_Engine/Utilities/Explicit.h"
#include <stdexcept>

namespace ptne
{
//! Exception base type used in the PTN Engine.
class DLL_PUBLIC PTN_Exception : public std::runtime_error
{
public:
	explicit PTN_Exception(const std::string &msg)
	: std::runtime_error(msg)
	{
	}
};
} // namespace ptne
