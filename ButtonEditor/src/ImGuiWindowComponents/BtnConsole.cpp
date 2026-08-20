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

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 170.0f);
		if (ImGui::Button("Clear")) {
			sink->clearBuffer();
		}
		ImGui::SameLine();

		std::string label = (condenseMessages) ? "Condensed" : "Un-Condensed";
		label += "##BtnConsoleCondenseButton";

		if (ImGui::Button(label.c_str())) {
			condenseMessages = !condenseMessages;
			sink->SetShouldCondense(condenseMessages);
		}

		ImVec2 consoleSize = ImGui::GetContentRegionAvail();

		ImGui::BeginChild("TextRegion", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border, ImGuiWindowFlags_NoScrollbar);

		const auto& buffer = sink->GetBuffer();
		if (!buffer.empty()) {
			std::string fullTextBuffer;
			struct ProcessedLine {
				std::string text;
				ImVec4 color;
			};
			std::vector<ProcessedLine> linesToRender;
			linesToRender.reserve(buffer.size());

			for (size_t i = 0; i < buffer.size(); ++i) {
				const auto& line = buffer[i];
				std::string levelType = " INFO ";
				ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

				switch (line.level) {
				case spdlog::level::level_enum::info:
					levelType = " INFO ";
					color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
					break;
				case spdlog::level::level_enum::trace:
					levelType = " TRACE ";
					color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				case spdlog::level::level_enum::warn:
					levelType = " WARN ";
					color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
					break;
				case spdlog::level::level_enum::err:
					levelType = " ERROR ";
					color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				}

				std::string constructedLine = line.loggerName + levelType + line.message + "\n";
				fullTextBuffer += constructedLine;
				linesToRender.push_back({ constructedLine, color });
			}

			ImVec2 startCursorPos = ImGui::GetCursorPos();

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 0));

			std::vector<char> rawBuffer(fullTextBuffer.begin(), fullTextBuffer.end());
			rawBuffer.push_back('\0');

			ImGui::InputTextMultiline("##HiddenSelectableConsole", rawBuffer.data(), rawBuffer.size(),
									  ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
			ImGui::PopStyleColor(2);

			ImGui::BeginChild("##HiddenSelectableConsole");
			float scrollY = ImGui::GetScrollY();
			ImGui::EndChild();

			ImVec2 endCursorPos = ImGui::GetCursorPos();
			ImGuiStyle& style = ImGui::GetStyle();

			ImVec2 alignedPos = startCursorPos;
			alignedPos.x += style.FramePadding.x;

			alignedPos.y += style.FramePadding.y - scrollY;
			ImGui::SetCursorPos(alignedPos);

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 contentAvail = ImGui::GetContentRegionAvail();

			ImGui::PushClipRect(
				ImVec2(windowPos.x + startCursorPos.x, windowPos.y + startCursorPos.y),
				ImVec2(windowPos.x + startCursorPos.x + contentAvail.x, windowPos.y + startCursorPos.y + contentAvail.y),
				true
			);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			for (const auto& line : linesToRender) {
				ImGui::SetCursorPosX(alignedPos.x);
				ImGui::TextColored(line.color, "%s", line.text.c_str());
			}
			ImGui::PopStyleVar();

			ImGui::PopClipRect();

			ImGui::SetCursorPos(endCursorPos);
		}

		ImGui::EndChild();
		ImGui::End();
	}

}
