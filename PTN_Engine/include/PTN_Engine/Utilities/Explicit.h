/*
 * 2017 July 1
 *
 * The author disclaims copyright to the source code in this file.
 */

#pragma once


#if defined(WIN32)
	#ifdef _EXPORTING
		#define DLL_PUBLIC __declspec(dllexport)
	#else
		#define DLL_PUBLIC __declspec(dllimport)
	#endif
	#define DLL_HIDDEN
#elif defined(__GNUC__)
	//GCC
	#ifdef _EXPORTING
		#define DLL_PUBLIC __attribute__((visibility("default")))
		#define DLL_HIDDEN __attribute__((visibility("hidden")))
	#else
		#define DLL_PUBLIC
		#define DLL_HIDDEN
	#endif
#else
	#define DLL_PUBLIC
	#define DLL_HIDDEN
#endif
