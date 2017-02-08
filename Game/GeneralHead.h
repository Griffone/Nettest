#pragma once

#define VERSION "0"

#define PLATFORM

#define PLATFORM_WIN 1

#ifdef _WIN32
#undef PLATFORM
#define PLATFORM PLATFORM_WIN
#endif	// _WIN32

#define BUILD

#define TEST_BUILD 0

#define BUILD_TEST 1
#define BUILD_RELEASE 2

#ifdef _DEBUG
#undef BUILD
#define BUILD BUILD_TEST
#undef TEST_BUILD
#define TEST_BUILD 1
#else
#define BUILD BUILD_RELEASE
#endif // _DEBUG
