#include "Logger.h"
namespace BtnSqd {

	spdlog::logger* Logger::s_clientLgr;
	spdlog::logger* Logger::s_coreLgr;

	spdlog::logger* Logger::GetCoreConsole() { return s_coreLgr; }
	spdlog::logger* Logger::GetClientConsole() { return s_clientLgr; }

	Logger::~Logger() {
		delete s_clientLgr;
		s_clientLgr = nullptr;
		delete s_coreLgr;
		s_coreLgr = nullptr;
	}

	void Logger::Init() {
		spdlog::set_pattern("%^[%T] %l %n:%v%$");

		s_coreLgr = spdlog::stdout_color_mt("ButtonSqrd").get();
		s_coreLgr->set_level(spdlog::level::trace);

		s_clientLgr = spdlog::stdout_color_mt("Application").get();
		s_clientLgr->set_level(spdlog::level::trace);
	}
}