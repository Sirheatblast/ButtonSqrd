#pragma once
#include"ButtonSqrd/Core/Logger.h"

#include<string>
#include<deque>
#include<imgui.h>

#include"ButtonSqrd/Core/Application.h"
#include"../EditorEvents.h"

namespace BtnSqd {
	class BtnConsole {
	public:
		BtnConsole();

		void OnUpdate();
	private:
		struct MessageInfo {
			std::string message;
			std::string loggerName;
			spdlog::level::level_enum level;
		};
		class BtnSink:public spdlog::sinks::sink {
		public:
			void log(const spdlog::details::log_msg& msg) override {
				std::string formatted = fmt::to_string(msg.payload);
				buffer.push_back({ formatted,std::string(msg.logger_name.data()),msg.level});
				if (buffer.size() > maxSize) buffer.pop_front();
			}
			void flush() override {}

			void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override {
				formatter_ = std::move(sink_formatter);
			}

			void set_pattern(const std::string& pattern) override {
				set_formatter(std::unique_ptr<spdlog::formatter>(new spdlog::pattern_formatter{ pattern }));
			}

			const std::deque<MessageInfo>& GetBuffer() const {
				return buffer;
			}

			void clearBuffer() {
				buffer.clear();
			}
		private:
			
			std::deque<MessageInfo> buffer;
			size_t maxSize = 500;
			std::mutex mutex;
			std::unique_ptr<spdlog::formatter> formatter_; 
		};

		std::shared_ptr<BtnSink>sink;
		std::string lastMessage ="";
		bool condenseMessages = true;
	};
}