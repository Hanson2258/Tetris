#pragma once
#include "crtdbg.h"			// for detecting memory leaks

// This header file is intended to assist with debugging memory leaks.
// Include it in header or cpp files that use the new keyword to dynamically
// allocate memory.  It will map instances of new in your code to be mapped 
// to the debug version of new so that the source file and line number of
// the call are recorded (so that they can be output when a memory leak occurs)

// Calling _CrtMemDumpAllObjectsSince(NULL); at the end of your main() function
// should output memory leaks along with line numbers to the console.

// Details on this were taken from this article:
// https://learn.microsoft.com/en-us/troubleshoot/developer/visualstudio/cpp/libraries/crtdbg-map-alloc-macro-not-work


// if we're creating a debug build
#ifdef _DEBUG												

// define a debug version of the new operator
#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__) 

// anywhere new is used in code, replace it with the debug version
#define new MYDEBUG_NEW		

#endif	// _DEBUG