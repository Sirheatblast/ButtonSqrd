#include "BtnConsole.h"

namespace BtnSqd {
	BtnConsole::BtnConsole() {
		sink.reset(new BtnSink());
		Logger::GetClientConsole()->sinks().push_back(sink);
		Logger::GetCoreConsole()->sinks().push_back(sink);
	}
	void BtnConsole::OnUpdate()	{
		ImGui::Begin("Console");

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)&&!ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
		}

		ImGui::SetCursorPosX(ImGui::GetWindowWidth()-150.0f);
		if (ImGui::Button("Clear")) {
			sink->clearBuffer();
		}
		ImGui::SameLine();
		if (ImGui::Button("Condense")) {
			condenseMessages = !condenseMessages;
		}

		ImVec2 consoleSize = ImGui::GetContentRegionAvail();
		lastMessage = "";

		ImGui::BeginChild("TextRegion", ImVec2(consoleSize.x,consoleSize.y ),true);
		for (auto& line : sink->GetBuffer()) {
			std::string message = line.loggerName;
			switch (line.level) {
			case spdlog::level::level_enum::info:
				message += " INFO: " + line.message;
				if (condenseMessages) {
					if (message != lastMessage) {
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), message.c_str());
					}
				}
				else {
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), message.c_str());
				}
				break;
			case spdlog::level::level_enum::trace:
				message += " TRACE: " + line.message;
				if (condenseMessages) {
					if (message != lastMessage) {
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), message.c_str());
					}
				}
				else{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), message.c_str());
				}
				break;
			case spdlog::level::level_enum::warn:
				message += " WARN: " + line.message;
				if (condenseMessages) {
					if (message != lastMessage) {
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), message.c_str());
					}
				}
				else {
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), message.c_str());
				}
				break;
			case spdlog::level::level_enum::err:
				message += " ERROR: " + line.message;
				if (condenseMessages) {
					if (message != lastMessage) {
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), message.c_str());
					}
				}
				else {
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), message.c_str());
				}
				break;
			}

			lastMessage = message;
		}
		ImGui::EndChild();

		ImGui::End();
	}
}
