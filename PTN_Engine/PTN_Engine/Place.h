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

#include "PTN_Engine/PTN_Exception.h"
#include <memory>

namespace ptne
{
class IActionFunctor;

//! Implements a place of a Petri net.
/*!
 * Implements a place of a Petri net.
 */
class Place final
{
public:
	//! Shared pointer to member functions of the controller.
	using ActionFunctorPtr = std::shared_ptr<IActionFunctor>;

	/*!
	 * Place constructor.
	 * \param initialNumberOfTokens The number of tokens the place contains originally.
	 * \param onEnterAction The action to be performed when a token enters the place.
	 * \param onExitAction The action to be performed when a token leaves the place.
	 * \param input Flag that marks the place as an input place.
	 */
	Place(const size_t initialNumberOfTokens,
		  ActionFunctorPtr onEnterAction,
		  ActionFunctorPtr onExitAction,
		  const bool input = false);

	~Place();

	//! Functor to be called when a token enters the place.
	const ActionFunctorPtr m_onEnterAction;

	//! Functor to be called when a token leaves the place.
	const ActionFunctorPtr m_onExitAction;

	//! Increase number of tokens and call on enter action.
	void enterPlace(const size_t tokens = 1);

	//! Decrease number of tokens and call on exit action.
	void exitPlace(const size_t tokens = 1);

	/*!
	 * Set the number of tokens in the place.
	 * \param tokens Number of tokens to be set.
	 */
	void setNumberOfTokens(const size_t tokens);

	/*!
	 * Return the number of tokens.
	 * \return The number of tokens in the place.
	 */
	size_t getNumberOfTokens() const;

	/*!
	 * If the place is flagged as input.
	 * \return If the place is an input place.
	 */
	bool isInputPlace() const;

	//! Exception thrown if attempted to remove 0 tokens from a place.
	class NullTokensException : public PTN_Exception
	{
	public:
		NullTokensException();
	};

	//! Exception thrown if attempted to remove more tokens than available.
	class NotEnoughTokensException : public PTN_Exception
	{
	public:
		NotEnoughTokensException();
	};

	//! Exception thrown if attempted to surpass the maximum possible number of tokens.
	class OverflowException : public PTN_Exception
	{
	public:
		explicit OverflowException(const size_t tooBig);
	};

private:
	/*!
	 * Increase number of tokens in the place.
	 * \param tokens Number of tokens to be added. Must be at least 1.
	 */
	void increaseNumberOfTokens(const size_t tokens = 1);

	/*!
	 * Decrease number of tokens in the place.
	 * \param tokens Number of tokens to be removed. Must be at least 1.
	 */
	void decreaseNumberOfTokens(const size_t tokens = 1);

	//! Number of tokens in the place.
	size_t m_numberOfTokens;

	//! Flag that determines if the place can be added tokens from outside the net.
	bool m_isInputPlace;
};

} // namespace ptne
