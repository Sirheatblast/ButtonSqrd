#include "BtnConsole.h"

namespace BtnSqd {
	BtnConsole::BtnConsole() {
		sink.reset(new BtnSink());
		Logger::GetClientConsole()->sinks().push_back(sink);
		Logger::GetCoreConsole()->sinks().push_back(sink);
	}
	void BtnConsole::OnUpdate() {
		ImGui::Begin("Console");

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemActive()) {
			Application::GetApp()->PushEvent(new OnSelectWindowEvent(ActiveWindow::MainGUI));
		}

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150.0f);
		if (ImGui::Button("Clear")) {
			sink->clearBuffer();
		}
		ImGui::SameLine();
		if (ImGui::Button("Condense")) {
			condenseMessages = !condenseMessages;
		}

		ImVec2 consoleSize = ImGui::GetContentRegionAvail();
		lastMessage = "";

		ImGui::BeginChild("TextRegion", ImVec2(consoleSize.x, consoleSize.y), true);
		for (size_t i = 0; i < sink->GetBuffer().size(); ++i) {
			const auto& line = sink->GetBuffer()[i];
			std::string uniqueId = "##ConsoleLogLine###" + std::to_string(i);
			std::string message = line.loggerName;
			switch (line.level) {
			case spdlog::level::level_enum::info: {
				message += " INFO: " + line.message;
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));

				std::vector<char> messageRaw(message.size() + 1);
				strcpy_s(messageRaw.data(), messageRaw.size(), message.c_str());
				
				if (condenseMessages) {
					if (message != lastMessage) {
						ImGui::InputText(uniqueId.c_str(), messageRaw.data(), messageRaw.size(), ImGuiInputTextFlags_ReadOnly);
					}
				}
				else {
					ImGui::InputText(uniqueId.c_str(), messageRaw.data(), messageRaw.size(), ImGuiInputTextFlags_ReadOnly);
				}

				if (ImGui::IsItemFocused() && ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C)) {
					ImGui::SetClipboardText(message.c_str());
				}

				ImGui::PopStyleColor(2);
				break;
			}
			case spdlog::level::level_enum::trace:
				message += " TRACE: " + line.message;
				if (condenseMessages) {
					if (message != lastMessage) {
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), message.c_str());
					}
				}
				else {
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
