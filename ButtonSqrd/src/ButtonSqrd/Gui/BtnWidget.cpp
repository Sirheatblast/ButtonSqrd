#include"BtnWidget.h"

namespace BtnSqd {
	void BtnWidget::Delete() {
		auto par = parent.lock();
		for (auto child : children) {
			RemoveChild(child);
			if (par) {
				par->AddChild(child);
			}
		}
		//par->RemoveChild(weak_from_this());

	}

	void BtnWidget::AddChild(std::weak_ptr<BtnWidget> widget) {
		auto child = widget.lock();

		if (!child || child.get() == this) {
			return;
		}

		if (child->HasParent()) {
			child->GetParent()->RemoveChild(widget);
		}

		child->parent = this->weak_from_this();
		children.push_back(child);
	}

	void BtnWidget::RemoveChild(std::weak_ptr<BtnWidget> widget) {
		auto child = widget.lock();
		if (!child) {
			return;
		}

		auto it = std::find_if(children.begin(), children.end(), [&](const std::weak_ptr<BtnWidget>& w) {
			return w.lock() == widget.lock();
							   });

		if (it != children.end()) {
			child->parent.reset();
			children.erase(it);
		}
	}

	void BtnWidget::UpdateChildrenPos(std::weak_ptr<BtnWidget> widget) {
		auto strongChild = widget.lock();
		if (!strongChild) {
			return;
		}

		strongChild->children.erase(
			std::remove_if(strongChild->children.begin(), strongChild->children.end(),
			[](const std::weak_ptr<BtnWidget>& weakChild) {
				return weakChild.expired();
			}),
			strongChild->children.end()
		);

		

		if (strongChild->HasParent()) {
			if (auto parentPtr = strongChild->GetParent()) {
				strongChild->pos = parentPtr->GetPos() + strongChild->localPos;
			}
		}
		for (const auto& child : strongChild->children) {
			UpdateChildrenPos(child);
		}
	}
	bool BtnWidget::HasParent() const {
		return !parent.expired();
	}
	std::shared_ptr<BtnWidget> BtnWidget::GetParent() {
		return parent.lock();
	}
}