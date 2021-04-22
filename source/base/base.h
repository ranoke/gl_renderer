#ifndef __BASE_H__
#define __BASE_H__

#include <memory>

//#include "utils/stb_leakcheck.h"

#define BIT(x) (1 << x)
#define zero_enum(type) (type)(0)

//template<typename T>
//using scope<T> = std::unique_ptr<T>;
//
//template<typename T, typename ... Args>
//constexpr scope<T> create_scope(Args&& ... args)
//{
//	return std::make_unique<T>(std::forward<Args>(args)...);
//}
//
//template<typename T>
//using ref<T> = std::shared_ptr<T>;
//
//template<typename T, typename ... Args>
//constexpr ref<T> make_ref(Args&& ... args)
//{
//	return std::make_shared<T>(std::forward<Args>(args)...);
//}

#endif // __BASE_H__