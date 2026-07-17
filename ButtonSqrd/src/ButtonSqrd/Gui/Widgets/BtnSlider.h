#pragma once
#include<ButtonSqrd/Gui/BtnWidget.h>
#include<ButtonSqrd/Gui/BtnSmartRect.h>
namespace BtnSqd {
	enum SliderType {
		Dot=0,
		Bar,
	};

	class BtnSlider :BtnWidget{
	public:
		Mesh& Draw(std::shared_ptr<Shader> shader) override;
	private:
		float currentPercentange = 0.0f;
		float minRange = 0.0f;
		float maxRange = 1.0f;
		SliderType sType=SliderType::Dot;

		glm::vec2 lastDimensions;
		BtnSmartRect rect;
		
		static inline std::vector<unsigned int> indices = { 0,2,1,2,0,3 };
	};
}