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

	class BtnWidget {
	public:
		virtual ~BtnWidget()= default;
		virtual Mesh& Draw() = 0;

		inline unsigned int GetLevel() const { return level; }
		inline unsigned int GetId()const { return id; }
		inline glm::vec2 GetPos()const { return pos; }
		inline glm::vec2 GetPercentPos()const { return percentPos; }
		inline std::string GetName()const { return name; }
		inline glm::vec2 GetDimensions()const { return {width,height}; }
		inline glm::vec4 GetColor()const { return color; }
		inline float GetBorder()const { return border; }
		inline BtnWidgetType GetType()const { return wType; }
		inline bool GetUseScreenDim()const { return useScreenDim; }
		inline bool GetHovered()const { return isHovered; }
		
		inline void SetId(unsigned int nId) { id = nId; }
		inline void SetPos(glm::vec2 nPos) { pos = nPos; }
		inline void SetPosPercent(glm::vec2 nPercent) { percentPos = nPercent; }
		inline void SetName(std::string newName) { name = newName; }
		inline void SetColor(glm::vec4 nColor) { color = nColor; }
		inline void SetBorder(float nBorder) { border = nBorder; }
		inline void SetUseScreenDim(bool useScreen) { useScreenDim = useScreen; }
		inline void SetHover(bool hover) { isHovered = hover; }
		void SetDimensions(glm::vec2 dimensions) {
			width = dimensions.x;
			height = dimensions.y;
		}

	protected:
		unsigned int id =0;
		unsigned int level=0;
		float width=100;
		float height = 100;
		float border = 1.0f;

		glm::vec2 pos = glm::vec2(0.0f);
		glm::vec2 percentPos = glm::vec2(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		bool isEnabled = true;
		bool isHovered = false;
		bool useScreenDim = false;
		std::string name = "widget";
		BtnWidgetType wType= BtnWidgetType::None;
	};
}