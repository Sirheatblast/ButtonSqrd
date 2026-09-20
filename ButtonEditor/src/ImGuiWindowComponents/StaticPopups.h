#pragma once
#include<ButtonSqrd/Core/CoreComponents.h>
#include<ButtonSqrd/Gui/Widgets/BtnButton.h>
#include<ButtonSqrd/Gui/Widgets/BtnTextBox.h>
#include<ButtonSqrd/Gui/Widgets/BtnImage.h>
#include<ButtonSqrd/Gui/Widgets/BtnSlider.h>
#include<imgui.h>

namespace BtnSqd {
	static void WidgetOptionsPopup(WidgetCanvasComponent& wCanvas) {
		if (ImGui::BeginPopup("##WidgetOptionsPopup")) {
			if (ImGui::Selectable("Delete##WidgetOptionPopupDelete")) {
				wCanvas.selectedWidget->Delete();
				auto widgetIter = std::find(wCanvas.Widgets.begin(), wCanvas.Widgets.end(), wCanvas.selectedWidget);
				if (widgetIter != wCanvas.Widgets.end()) {
					wCanvas.Widgets.erase(widgetIter);
				}
				wCanvas.selectedWidget = nullptr;
			}
			ImGui::EndPopup();
		}
	}

	static void CreateWidgetPopup(WidgetCanvasComponent& wCanvas) {
		if (ImGui::BeginPopup("##AddWidgetPopup")) {
			if (ImGui::Selectable("Text Box")) {
				wCanvas.Widgets.push_back(std::make_shared<BtnTextBox>());
			}
			if (ImGui::Selectable("Button")) {
				auto button = std::make_shared<BtnButton>();
				wCanvas.Widgets.push_back(button);
				auto text = std::make_shared<BtnTextBox>();
				wCanvas.Widgets.push_back(text);
				button->AddChild(text);
			}
			if (ImGui::Selectable("Image")) {
				wCanvas.Widgets.push_back(std::make_shared<BtnImage>());
			}
			if (ImGui::Selectable("Slider")) {
				wCanvas.Widgets.push_back(std::make_shared<BtnSlider>());
			}
			ImGui::EndPopup();
		}
	}
}