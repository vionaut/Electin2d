#pragma once

#include "quill/Logger.h"
#include "quill/LogMacros.h"

#include <string>
#include <cstdlib> // for std::abort()

namespace el
{
	enum class ElLogLevel
	{
		Debug,
		Info,
		Warning,
		Error,
		Critical
	};

	class ElLogger
	{
	public:
		static void init();
		static void setLogLevel(ElLogLevel);
		static quill::Logger *getLoggerAPI();
		// static void shutdown();
		static void flush();

	private:
		static quill::Logger *logger;
	};
}

#ifdef NDEBUG
#define EL_LOG_DEBUG(msg_str, ...)
#else
#define EL_LOG_DEBUG(msg_str, ...) LOG_DEBUG(el::ElLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#endif

#define EL_LOG_INFO(msg_str, ...) LOG_INFO(el::ElLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#define EL_LOG_WARNING(msg_str, ...) LOG_WARNING(el::ElLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#define EL_LOG_ERROR(msg_str, ...) LOG_ERROR(el::ElLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)
#define EL_LOG_CRITICAL(msg_str, ...) LOG_CRITICAL(el::ElLogger::getLoggerAPI(), msg_str, ##__VA_ARGS__)

#ifdef NDEBUG						 // If we are building for Release
#define EL_ASSERT(condition, msg, ...) ((void)0)
#else							     // If we are building for Debug (Hunting bugs)
#define EL_ASSERT(condition, msg, ...)               \
	do {                                             \
		if (!(condition))                            \
		{                                            \
			EL_LOG_CRITICAL(msg, ##__VA_ARGS__);     \
			el::ElLogger::flush();                   \
			std::abort();                            \
		}                                            \
	} while (0)
#endif