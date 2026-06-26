#pragma once
#include<glm/glm.hpp>
#include<string>

#include"ButtonSqrd/Render/Mesh.h"

namespace BtnSqd {
	enum class BtnWidgetType {
		Button,
		Slider,
		Text,
		BtnImage,
		None
	};

	class BtnWidget:public std::enable_shared_from_this<BtnWidget> {
	public:
		virtual ~BtnWidget()= default;
		virtual Mesh& Draw(std::shared_ptr<Shader> shader) = 0;

		inline unsigned int GetLevel() const { return level; }
		inline unsigned int GetId()const { return id; }
		inline glm::vec2 GetPos()const { return pos; }
		inline glm::vec2 GetLocalPos()const { return localPos; }
		inline glm::vec2 GetPercentPos()const { return percentPos; }
		inline std::string GetName()const { return name; }
		inline glm::vec2 GetDimensions()const { return {width,height}; }
		inline glm::vec4 GetColor()const { return color; }
		inline float GetBorder()const { return border; }
		inline BtnWidgetType GetType()const { return wType; }
		inline bool GetUseScreenDim()const { return useScreenDim; }
		inline bool GetHovered()const { return isHovered; }
		inline bool GetHasTexture()const { return hasTexture; }
		
		inline void SetId(unsigned int nId) { id = nId; }
		inline void SetLevel(unsigned int nLevel) { level = nLevel; }
		inline void SetPos(glm::vec2 nPos) { pos = nPos; }
		inline void SetLocalPos(glm::vec2 nlPos) { localPos = nlPos; }
		inline void SetPosPercent(glm::vec2 nPercent) { percentPos = nPercent; }
		inline void SetName(std::string newName) { name = newName; }
		inline void SetColor(glm::vec4 nColor) { color = nColor; }
		inline void SetBorder(float nBorder) { border = nBorder; }
		inline void SetUseScreenDim(bool useScreen) { useScreenDim = useScreen; }
		inline void SetHover(bool hover) { isHovered = hover; }
		inline bool GetMix() { return mix; }
		inline void SetMix(bool nMix) { mix = nMix; }
		inline void SetHasTexture(bool hasTex) { hasTexture = hasTex; }
		void SetDimensions(glm::vec2 dimensions) {
			width = dimensions.x;
			height = dimensions.y;
		}

		void AddChild(BtnWidget* widget);
		void RemoveChild(BtnWidget* widget);
		void UpdateChildrenPos(BtnWidget* widget);

		bool HasParent()const { return parent != nullptr; }
		BtnWidget* GetParent() { return parent; }

		bool HasChildren()const { return !children.empty(); }
		bool GetShowChildren() { return showChildren; }
		void SetShowChildren(bool show) { showChildren = show; }
		std::vector<BtnWidget*> GetChildren() { return children; }

	protected:
		unsigned int id =0;
		unsigned int level=0;
		float width=100;
		float height = 100;
		float border = 1.0f;

		glm::vec2 pos = glm::vec2(0.0f);
		glm::vec2 localPos = glm::vec2(0.0f);
		glm::vec2 percentPos = glm::vec2(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		bool isEnabled = true;
		bool isHovered = false;
		bool useScreenDim = false;
		bool mix = false;
		bool hasTexture = false;
		bool showChildren = false;
		std::string name = "widget";
		BtnWidgetType wType= BtnWidgetType::None;

		BtnWidget* parent = nullptr;
		std::vector<BtnWidget*>children;
	};
}