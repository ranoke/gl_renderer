#ifndef __BASE_H__
#define __BASE_H__


#include <memory>
#include <cassert>

//#include "utils/stb_leakcheck.h"

#define BIT(x) (1 << x)
#define zero_enum(type) (type)(0)

#ifndef R_DEBUG
  #ifdef R_PLATFORM_LINUX
    #include <signal.h>
    #define DEBUG_BREAK() { raise(SIGTRAP); }
  #elif R_PLATFORM_WINDOWS
    #define R_ASSERT() { __debugbreak(); }
  #else
    #error PLATFORM_IS_NOT_SUPPORTED
  #endif
#else
  #define R_ASSERT() {}
#endif

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr scope<T> create_scope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr ref<T> make_ref(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif // __BASE_H__