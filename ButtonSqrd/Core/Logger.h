#pragma once
#include<memory>
#include<stdexcept>

#include"core.h"

#include<spdlog/spdlog.h>
#include<spdlog/sinks/stdout_color_sinks.h>

namespace BtnSqd {
	class Logger {
	public:
		~Logger();

		static void Init();

		static spdlog::logger* GetCoreConsole();
		static spdlog::logger* GetClientConsole();
	private:
		static spdlog::logger* s_coreLgr;
		static spdlog::logger* s_clientLgr;
	};

#ifdef _DEBUG
	#define BTN_CORE_LOG_TRACE(...) BtnSqd::Logger::GetCoreConsole()->trace(__VA_ARGS__);
	#define BTN_CORE_LOG_INFO(...)  BtnSqd::Logger::GetCoreConsole()->info(__VA_ARGS__);
	#define BTN_CORE_LOG_WARN(...)  BtnSqd::Logger::GetCoreConsole()->warn(__VA_ARGS__);
	#define BTN_CORE_LOG_ERROR(...) BtnSqd::Logger::GetCoreConsole()->error(__VA_ARGS__); throw std::runtime_error(__VA_ARGS__);

	#define BTNLOG_TRACE(...) BtnSqd::Logger::GetClientConsole()->trace(__VA_ARGS__);
	#define BTNLOG_INFO(...)  BtnSqd::Logger::GetClientConsole()->info(__VA_ARGS__);
	#define BTNLOG_WARN(...)  BtnSqd::Logger::GetClientConsole()->warn(__VA_ARGS__);
	#define BTNLOG_ERROR(...) BtnSqd::Logger::GetClientConsole()->error(__VA_ARGS__);

#else
	#define BTN_CORE_LOG_TRACE(...) BtnSqd::Logger::GetCoreConsole()->trace(__VA_ARGS__);
	#define BTN_CORE_LOG_INFO(...) BtnSqd::Logger::GetCoreConsole()->info(__VA_ARGS__);
	#define BTN_CORE_LOG_WARN(...) BtnSqd::Logger::GetCoreConsole()->warn(__VA_ARGS__);
	#define BTN_CORE_LOG_ERROR(...) BtnSqd::Logger::GetCoreConsole()->error(__VA_ARGS__); throw std::runtime_error(__VA_ARGS__);

	#define BTNLOG_TRACE(...) BtnSqd::Logger::GetClientConsole()->trace(__VA_ARGS__);
	#define BTNLOG_INFO(...)BtnSqd::Logger::GetClientConsole()->info(__VA_ARGS__);
	#define BTNLOG_WARN(...)BtnSqd::Logger::GetClientConsole()->warn(__VA_ARGS__);
	#define BTNLOG_ERROR(...)BtnSqd::Logger::GetClientConsole()->error(__VA_ARGS__);
#endif
}

