#pragma once

#include "quill/Logger.h"
#include "quill/LogMacros.h"

#include <string>
#include <cstdlib> // for std::abort()

namespace vortex::io
{
	enum class VxLogLevel
	{
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};

	class VxLogger
	{
	public:
		static void init();
		static void setLogLevel(VxLogLevel);
		static quill::Logger* getLoggerAPI();
		// static void shutdown();
		static void flush();

	private:
		static quill::Logger* logger;
	};
}

#ifdef NDEBUG
#define VX_LOG_DEBUG(msg_str,...)
#else
#define VX_LOG_DEBUG(msg_str,...) LOG_DEBUG(vortex::io::VxLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#endif

#define VX_LOG_INFO(msg_str, ...) LOG_INFO(vortex::io::VxLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#define VX_LOG_WARNING(msg_str, ...) LOG_WARNING(vortex::io::VxLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#define VX_LOG_ERROR(msg_str, ...) LOG_ERROR(vortex::io::VxLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#define VX_LOG_CRITICAL(msg_str, ...) LOG_CRITICAL(vortex::io::VxLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)

#ifdef NDEBUG // If we are building for Release
#define VX_ASSERT(condition, msg, ...); // Do absolutely nothing!
#else         // If we are building for Debug (Hunting bugs)
#define VX_ASSERT(condition, msg, ...); \
        if (!(condition)) { \
            VX_LOG_CRITICAL(msg, ##__VA_ARGS__); \
            vortex::io::VxLogger::flush(); \
            std::abort(); \
        }
#endif