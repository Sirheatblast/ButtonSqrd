#include "BtnConsole.h"

namespace BtnSqd {
	BtnConsole::BtnConsole() {
		sink.reset(new BtnSink());
		Logger::GetClientConsole()->sinks().push_back(sink);
		Logger::GetCoreConsole()->sinks().push_back(sink);
	}
	void BtnConsole::LogToConsole(LogData lData) {
		lData.message += lData.levelType + lData.line.message;
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Text, lData.color);

		std::vector<char> messageRaw(lData.message.size() + 1);
		strcpy_s(messageRaw.data(), messageRaw.size(), lData.message.c_str());

		if (condenseMessages) {
			if (lData.message != lastMessage) {
				ImGui::InputText(lData.uniqueId.c_str(), messageRaw.data(), messageRaw.size(), ImGuiInputTextFlags_ReadOnly);
			}
		}
		else {
			ImGui::InputText(lData.uniqueId.c_str(), messageRaw.data(), messageRaw.size(), ImGuiInputTextFlags_ReadOnly);
		}
		lastMessage = lData.message;
		ImGui::PopStyleColor(2);
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
			LogData lData(line);
			lData.message = message;
			lData.uniqueId = uniqueId;

			switch (line.level) {
			case spdlog::level::level_enum::info: {
				lData.levelType = " INFO ";
				lData.color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
				break;
			}
			case spdlog::level::level_enum::trace: {
				lData.levelType = " TRACE ";
				lData.color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}
			case spdlog::level::level_enum::warn: {
				lData.levelType = " WARN ";
				lData.color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
				break;
			}
			case spdlog::level::level_enum::err: {
				lData.levelType = " ERROR ";
				lData.color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			}
			}
			LogToConsole(lData);
		}
		ImGui::EndChild();

		ImGui::End();
	}
}
