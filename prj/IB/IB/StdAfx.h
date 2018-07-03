/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved. This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

#ifdef PosixClient_DLLEXP
#define DLLEXP __declspec( dllexport )
#else
#define DLLEXP __declspec( dllimport )
#endif

#ifdef _MSC_VER

#define assert ASSERT
#define snprintf _snprintf


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#endif

