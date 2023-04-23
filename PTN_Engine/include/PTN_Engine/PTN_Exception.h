/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2017-2023 Eduardo Valgôde
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
#include <string>

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

/*!
 * Exception to be thrown when trying to use a place with a wrong name.
 */
class DLL_PUBLIC InvalidNameException : public PTN_Exception
{
public:
	explicit InvalidNameException(const std::string &name)
	: PTN_Exception("Invalid name: " + name + ".")
	{
	}
};

/*!
 * Exception to be thrown if names of places to construct a transition are repeated.
 */
class DLL_PUBLIC RepeatedPlaceNamesException : public PTN_Exception
{
public:
	RepeatedPlaceNamesException()
	: PTN_Exception("Tried to create transition with repeated places.")
	{
	}
};

/*!
 * Exception to be thrown if the user tries to increment the number of tokens of a non input place.
 */
class DLL_PUBLIC NotInputPlaceException : public PTN_Exception
{
public:
	explicit NotInputPlaceException(const std::string &name)
	: PTN_Exception(name + " is not an input place.")
	{
	}
};

/*!
 * Exception to be thrown if the user tries to add a Place to the net that with the name of an already
 * existing one.
 */
class DLL_PUBLIC RepeatedPlaceException : public PTN_Exception
{
public:
	explicit RepeatedPlaceException(const std::string &name)
	: PTN_Exception("Trying to add an already existing place: " + name + ".")
	{
	}
};

/*!
 * Exception to be thrown if the user tries to add a Condition to the net that with the name of an already
 * existing one.
 */
class DLL_PUBLIC RepeatedActionException : public PTN_Exception
{
public:
	explicit RepeatedActionException(const std::string &name)
	: PTN_Exception("Trying to add an already existing action: " + name + ".")
	{
	}
};

/*!
 * Exception to be thrown if the user tries to add an Action to the net that with the name of an already
 * existing one.
 */
class DLL_PUBLIC RepeatedConditionException : public PTN_Exception
{
public:
	explicit RepeatedConditionException(const std::string &name)
	: PTN_Exception("Trying to add an already existing condition: " + name + ".")
	{
	}
};

/*!
 * Exception to be thrown if the user tries to use an invalid function name
 */
class DLL_PUBLIC InvalidFunctionNameException : public PTN_Exception
{
public:
	explicit InvalidFunctionNameException(const std::string &name)
	: PTN_Exception("The function is not yet registered: " + name + ".")
	{
	}
};

//! Exception thrown if attempted to remove 0 tokens from a place.
class DLL_PUBLIC NullTokensException : public PTN_Exception
{
public:
	NullTokensException()
	: PTN_Exception("Number of tokens must greater than 0")
	{
	}
};

//! Exception thrown if attempted to remove more tokens than available.
class DLL_PUBLIC NotEnoughTokensException : public PTN_Exception
{
public:
	NotEnoughTokensException()
	: PTN_Exception("Not enough tokens in the place.")
	{
	}
};

//! Exception thrown if attempted to surpass the maximum possible number of tokens.
class DLL_PUBLIC OverflowException : public PTN_Exception
{
public:
	explicit OverflowException(const size_t tooBig)
	: PTN_Exception("Cannot add " + std::to_string(tooBig) + " tokens to place without overflowing.")
	{
	}
};

/*!
 * Exception to be thrown when the dimension of the vector containing the activation weights does not match
 * the dimension of the vector containing the names of the activation places.
 */
class DLL_PUBLIC ActivationWeightDimensionException : public PTN_Exception
{
public:
	ActivationWeightDimensionException()
	: PTN_Exception("The number of activation weights must be the same as the number of activation places.")
	{}
};

/*!
 * Exception to be thrown when the dimension of the vector containing the destination weights does not match
 * the dimension of the vector containing the names of the destination places.
 */
class DLL_PUBLIC DestinationWeightDimensionException : public PTN_Exception
{
public:
	DestinationWeightDimensionException()
	: PTN_Exception("The number of destination weights must be the same as the number of destination places.")
	{}
};

//! Exception to be thrown when the weight value is 0.
class DLL_PUBLIC ZeroValueWeightException : public PTN_Exception
{
public:
	ZeroValueWeightException() 
	: PTN_Exception("Weights cannot be 0.")
	{
	}
};

/*!
 * Exception to be thrown when activation places in the constructor are repeated.
 */
class DLL_PUBLIC ActivationPlaceRepetitionException : public PTN_Exception
{
public:
	ActivationPlaceRepetitionException()
	: PTN_Exception("Repetition of activation places is not permitted.")
	{
	}
};

/*!
 * Exception to be thrown when activation places in the constructor are repeated.
 */
class DLL_PUBLIC DestinationPlaceRepetitionException : public PTN_Exception
{
public:
	DestinationPlaceRepetitionException()
	: PTN_Exception("Repetition of destination places is not permitted.")
	{
	}
};

} // namespace ptne
