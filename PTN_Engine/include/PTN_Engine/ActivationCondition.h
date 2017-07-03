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

#include "PTN_Engine/Explicit.h"
#include "PTN_Engine/IConditionFunctor.h"
#include "PTN_Engine/PTN_Exception.h"
#include <memory>

namespace ptne
{
	//! Facilitates creating functors of additional conditions to be used by the net.
	/*!
	 * Facilitates creating functors of additional conditions to be used by the net.
	 * On evaluating if a transition can be fired an additional condition can be used.
	 * This condition is defined by the controller.
	 * \sa ptne::IConditionFunctor
	 */
	template <class C>
	class DLL_PUBLIC ActivationCondition final:
		public IConditionFunctor
	{
	public:

		//! Member function pointer type.
		using FireConditionMFPTR = bool (C::*)() const;

		/*!
		 * Constructor of the ActivationCondition functor.
		 * \param controller Shared pointer to controller object.
		 * \param memberFunctionPointer Member function to be invoked by the net.
		 */
		ActivationCondition(std::shared_ptr<C> controller, FireConditionMFPTR memberFunctionPointer):
			m_parent{controller},
			m_memberFunctionPointer{memberFunctionPointer}
		{
		}

		/*!
		 * Function called by the net, which in its turn calls the
		 * controller's member function.
		 */
		bool operator()() const override
		{
			if(std::shared_ptr<C> sptrController = m_parent.lock())
			{
				C& controller = *(sptrController);
				return (controller.*(m_memberFunctionPointer))();
			}
			throw PTN_Exception("Unexpected invalid pointer");
		}

		//! Weak pointer to controller.
		std::weak_ptr<C> m_parent;

		//! Function pointer to member function of the controller.
		FireConditionMFPTR m_memberFunctionPointer;

	};
}
