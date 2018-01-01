/*
* 2017 September 17
*
* The author disclaims copyright to the source code in this file.
*/

#pragma once

#ifdef _MSC_FULL_VER
 // Disable warning about exporting PTN_Exception inheriting from 
 // std::runtime_error, which might have been compiled differently
 // by the client linking to PTN Engine.
#pragma warning( disable : 4275 )  
#endif

#include <memory>
#include <stdexcept>
#include "PTN_Engine/Utilities/Explicit.h"

namespace
{

	//! Exception to be thrown if attempting to lock an expired pointer.
	class DLL_PUBLIC ExpiredSharedPtrException : public std::runtime_error
	{
	public:
		ExpiredSharedPtrException() :
			runtime_error("Expired shared pointer.")
		{}
	};

	//! Exception to be throw if the locked shared pointer contains a nullptr.
	class DLL_PUBLIC NullLockedPtrException : public std::runtime_error
	{
	public:
		NullLockedPtrException() :
			runtime_error("Locked shared pointer is null.")
		{}
	};

	/*!
	 * Locks a std::weak_ptr checking for expiration. It throws a
	 * ExpiredSharedPtrException if the pointer is expired.
	 *
	 * \param toLock Weak pointer to be locked.
	 * \returns Shared pointer from locking toLock.
	 */
	template <typename T>
	std::shared_ptr<T> lockWeakPtr(std::weak_ptr<T> toLock)
	{
		if (std::shared_ptr<T> locked = toLock.lock())
		{
			return locked;
		}
		else
		{
			throw ExpiredSharedPtrException();
		}
	}

	/*!
	 * Locks a std::weak_ptr checking for expiration and for nullptr.
	 * It throws a ExpiredSharedPtrException if the pointer is expired.
	 * It throws a NullLockedPtrException if the shapred pointer contains
	 * nullptr.
	 *
	 * \param toLock Weak pointer to be locked.
	 * \returns Shared pointer from locking toLock (not nullptr).
	 */
	template <typename T>
	std::shared_ptr<T> lockWeakPtrNotNull(std::weak_ptr<T> toLock)
	{
		std::shared_ptr<T> sharedPtr = lockWeakPtr<T>(toLock);

		if (sharedPtr.get() == nullptr)
		{
			throw NullLockedPtrException();
		}
		return sharedPtr;

	}

}

