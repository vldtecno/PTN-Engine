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

#include "PTN_Engine/IActionFunctor.h"
#include "PTN_Engine/Utilities/Explicit.h"
#include "PTN_Engine/Utilities/LockWeakPtr.h"
#include <memory>

namespace ptne
{
//! Facilitates creating functors to be executed on enter and on exit.
/*!
 * Facilitates creating functors to be executed on enter and on exit of tokens in places.
 * \sa ptne::IActionFunctor
 */
template <class C>
class DLL_PUBLIC Action final : public IActionFunctor
{
public:
	//! Member function pointer type.
	using ActionMFPTR = void (C::*)();

	/*!
	 * Constructor of the Action functor.
	 * \param controller Shared pointer to controller object.
	 * \param memberFunctionPointer Member function to be invoked by the net.
	 */
	Action(std::shared_ptr<C> controller, ActionMFPTR memberFunctionPointer)
	: m_parent{ controller }
	, m_memberFunctionPointer{ memberFunctionPointer }
	{
	}

	/*!
	 * Function called by the net, which in its turn calls the controller's member function.
	 */
	void operator()() override
	{
		if (std::shared_ptr<C> sptrController = lockWeakPtr(m_parent))
		{
			C &controller = *(sptrController);
			(controller.*(m_memberFunctionPointer))();
		}
	}

	//! Weak pointer to controller.
	std::weak_ptr<C> m_parent;

	//! Function pointer to member function of the controller.
	ActionMFPTR m_memberFunctionPointer;
};
} // namespace ptne
