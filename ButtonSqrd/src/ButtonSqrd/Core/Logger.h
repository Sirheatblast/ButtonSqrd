#pragma once
#include<memory>
#include<stdexcept>

#include"core.h"

#include<spdlog/spdlog.h>
#include<spdlog/sinks/stdout_color_sinks.h>
#include<spdlog/fmt/fmt.h>

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

template <>
struct fmt::formatter<glm::quat> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::quat& q, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "quat({}, {}, {}, {})", q.w, q.x, q.y, q.z);
	}
};

template <>
struct fmt::formatter<glm::vec4> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec4& v, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "vec4({}, {}, {}, {})", v.x, v.y,v.z,v.w);
	}
};

template <>
struct fmt::formatter<glm::vec3> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec3& v, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "vec3({}, {}, {})", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<glm::vec2> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::vec2& v, FormatContext& ctx) {
		return fmt::format_to(ctx.out(), "vec2({}, {})", v.x, v.y);
	}
};

template <>
struct fmt::formatter<glm::mat4> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const glm::mat4& m, FormatContext& ctx) {
		return fmt::format_to(
			ctx.out(),
			"\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]",
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
		);
	}
};

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

