#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace logger
{

	inline void init()
	{

		spdlog::set_pattern("%^[%T] %n:%$ %v");
		spdlog::set_level(spdlog::level::trace);
		spdlog::flush_on(spdlog::level::trace);

	}

	template<typename ... Args>
	inline void trace(Args... args) 
	{
		spdlog::trace(args ...);
	}

	template<typename ... Args>
	inline void info(Args... args) 
	{
		spdlog::info(args ...);
	}

	template<typename ... Args>
	inline void warn(Args... args) 
	{
		spdlog::warn(args ...);
	}

	template<typename ... Args>
	inline void error(Args... args) 
	{
		spdlog::error(args ...);
	}
	

} // namespace log


#endif // __LOGGER_H_