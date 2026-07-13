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
		void SetHoverColor(glm::vec4 nCol) { hoverColor = nCol; }
		void SetClickColor(glm::vec4 nCol) { clickColor = nCol; }

		bool GetUseHoverColor() { return applyHoverColor; }
		bool GetUseClickColor() { return applyClickColor; }
		bool GetUseHoverTexture() { return useHoverTexture; }
		bool GetUseClickTexture() { return useClickTexture; }

		void SetUseHoverColor(bool useColor) { applyHoverColor = useColor; }
		void SetUseClickColor(bool useColor) { applyClickColor = useColor; }
		void SetUseHoverTexture(bool useTex) { useHoverTexture = useTex; }
		void SetUseClickTexture(bool useTex) { useClickTexture = useTex; }
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
		
		bool useHoverTexture;
		bool useClickTexture;
		bool applyHoverColor;
		bool applyClickColor;

		static inline std::vector<unsigned int> indices = { 0,2,1,2,0,3 };
	};
}