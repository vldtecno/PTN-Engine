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
#pragma warning(disable : 4275)
#endif

#include "PTN_Engine/PTN_Exception.h"
#include "PTN_Engine/Utilities/Explicit.h"
#include <memory>

namespace ptne
{

//! Exception to be thrown if attempting to lock an expired pointer.
class DLL_PUBLIC ExpiredSharedPtrException : public PTN_Exception
{
public:
	ExpiredSharedPtrException()
	: PTN_Exception("Expired shared pointer.")
	{
	}
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
} // namespace
