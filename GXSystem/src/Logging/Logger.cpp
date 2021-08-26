#include "Config/pch.h"
#include "Logger.h"

namespace gx {
	std::shared_ptr<spdlog::logger> Logger::engine_logger_ = nullptr;
	std::shared_ptr<spdlog::logger> Logger::game_logger_ = nullptr;

	void Logger::Init() {
		spdlog::set_pattern("%^[%T] |%n| %v%$");
		engine_logger_ = spdlog::stdout_color_mt("GX System");
		game_logger_ = spdlog::stdout_color_mt("Game");
		engine_logger_->set_level(spdlog::level::trace);
		game_logger_->set_level(spdlog::level::trace);

	}

}