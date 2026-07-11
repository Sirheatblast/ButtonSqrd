#pragma once
#include<ButtonSqrd/Gui/BtnWidget.h>
#include<ButtonSqrd/Gui/BtnSmartRect.h>
#include<ButtonSqrd/Core/ResourceManager.h>

namespace BtnSqd {
	class BtnButton:public BtnWidget {
	public:
		BtnButton();
		Mesh& Draw(std::shared_ptr<Shader> shader) override;
		void OnUpdate()override;
		void OnClick()override;
		void OnClickUp()override;

		void SetMainTexture(std::string path);
		void SetHoverTexture(std::string path);
		void SetClickTexture(std::string path);
		std::shared_ptr<Texture> GetTexture() { return buttonTexture; }
		std::shared_ptr<Texture> GetHoverTexture() { return hoverTexture; }
		std::shared_ptr<Texture> GetClickTexture() { return clickTexture; }

		glm::vec4 GetHoverColor() { return hoverColor; }
		glm::vec4 GetClickColor() { return clickColor; }
	private:
		std::vector<Vertices> GenerateVerts();
		void UpdateMesh();

		std::shared_ptr<Mesh>buttonMesh;

		std::shared_ptr<Texture>buttonTexture;
		std::shared_ptr<Texture>hoverTexture;
		std::shared_ptr<Texture>clickTexture;

		glm::vec4 hoverColor;
		glm::vec4 clickColor;

		glm::vec2 lastDimensions;
		BtnSmartRect rect;
		bool hasHoverTexture;
		bool hasClickTexture;

		static inline std::vector<unsigned int> indices = { 0,2,1,2,0,3 };
	};
}