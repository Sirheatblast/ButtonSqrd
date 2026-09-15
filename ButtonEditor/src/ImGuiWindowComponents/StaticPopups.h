#pragma once
#include<ButtonSqrd/Core/CoreComponents.h>
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
}