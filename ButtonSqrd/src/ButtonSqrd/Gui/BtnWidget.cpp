#include"BtnWidget.h"

namespace BtnSqd {
	void BtnWidget::AddChild(BtnWidget* widget) {
		if (widget == this) {
			return;
		}

		if (widget->HasParent()) {
			widget->GetParent()->RemoveChild(widget);
		}

		widget->parent = this;
		children.push_back(widget);
	}

	void BtnWidget::RemoveChild(BtnWidget* widget) {
		auto it = std::find(children.begin(), children.end(), widget);
		if (it != children.end()) {
			widget->parent = nullptr;
			children.erase(it);
		}
	}

	void BtnWidget::UpdateChildrenPos(BtnWidget* widget) {
		if (widget->parent) {
			widget->pos = widget->parent->GetPos() + widget->localPos;
		}
		for (const auto& child:widget->children) {
			UpdateChildrenPos(child);
		}
	}
}