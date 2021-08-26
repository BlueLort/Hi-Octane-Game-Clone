#ifndef LOGGER_H_
#define LOGGER_H_

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define GXE_PRINT(...) gx::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define GXE_INFO(...) gx::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define GXE_DEBUG(...) gx::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define GXE_WARN(...) gx::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define GXE_ERROR(...) gx::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define GXE_FERROR(...) gx::Logger::GetEngineLogger()->critical(__VA_ARGS__);

#define GX_PRINT(...) gx::Logger::GetGameLogger()->trace(__VA_ARGS__)
#define GX_INFO(...) gx::Logger::GetGameLogger()->info(__VA_ARGS__)
#define GX_DEBUG(...) gx::Logger::GetGameLogger()->debug(__VA_ARGS__)
#define GX_WARN(...) gx::Logger::GetGameLogger()->warn(__VA_ARGS__)
#define GX_ERROR(...) gx::Logger::GetGameLogger()->error(__VA_ARGS__)
#define GX_FERROR(...) gx::Logger::GetGameLogger()->critical(__VA_ARGS__);

namespace gx {

	class Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return engine_logger_; };
		inline static std::shared_ptr<spdlog::logger>& GetGameLogger() { return game_logger_; };
	private:
		static std::shared_ptr<spdlog::logger> engine_logger_;
		static std::shared_ptr<spdlog::logger> game_logger_;
	};
}

#endif