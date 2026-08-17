#include "el/core/io/logger/el_logger.hpp"

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/FileSink.h"
#include "quill/std/Array.h"

#include <string>
#include <utility>
#include <chrono>
#include <format>
#include <filesystem>

quill::Logger *el::ElLogger::logger = nullptr;

void el::ElLogger::init()
{
	// quill backend
	quill::BackendOptions backend_options;
	quill::Backend::start(backend_options);

	// quill frontend
	auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("c_sink");

	// formatting the log file name
	auto now = std::chrono::system_clock::now();
	auto duration_seconds = std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(now)};
	std::string filename = "logs/el_" + std::format("{:%Y_%m_%d_%H_%M_%S}", duration_seconds) + ".log";

	quill::FileEventNotifier f_notifier;
	f_notifier.before_open = [](std::filesystem::path const &filename)
	{
		if (filename.has_parent_path())
		{
			std::filesystem::create_directories(filename.parent_path());
		}
	};
	auto file_sink = quill::Frontend::create_or_get_sink<quill::FileSink>(
		filename, []()
		{
			quill::FileSinkConfig cfg;
			cfg.set_open_mode('w');
			return cfg; }(), f_notifier);

	logger = quill::Frontend::create_or_get_logger("root", {std::move(console_sink), std::move(file_sink)});
}

void el::ElLogger::setLogLevel(el::ElLogLevel level)
{
	if (!logger)
		return;

	switch (level)
	{
	case ElLogLevel::Debug:
		logger->set_log_level(quill::v12::LogLevel::Debug);
		break;
	case ElLogLevel::Info:
		logger->set_log_level(quill::v12::LogLevel::Info);
		break;
	case ElLogLevel::Warning:
		logger->set_log_level(quill::v12::LogLevel::Warning);
		break;
	case ElLogLevel::Error:
		logger->set_log_level(quill::v12::LogLevel::Error);
		break;
	case ElLogLevel::Critical:
		logger->set_log_level(quill::v12::LogLevel::Critical);
		break;
	}
}

quill::Logger *el::ElLogger::getLoggerAPI()
{
	return ElLogger::logger;
}

void el::ElLogger::flush()
{
	if (ElLogger::logger)
		logger->flush_log();
}