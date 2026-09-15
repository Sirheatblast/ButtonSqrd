#pragma once
#include"ButtonSqrd/Core/Application.h"
#include"ButtonSqrd/Core/Layer.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Core/Scene.h"
#include"ButtonSqrd/Render/RendererCommand.h"
#include"ButtonSqrd/Render/Shader.h"
#include"ButtonSqrd/Render/FrameBuffer.h"
#include"BtnWidget.h"
#include"ButtonSqrd/Gui/Widgets/BtnSlider.h"

#include"ButtonSqrd/Event/GuiEvent.h"

#include<queue>
#include<vector>
#include<memory>
#include<glm/glm.hpp>
namespace BtnSqd {
	class BtnGuiLayer : public Layer {
	public:
		BtnGuiLayer(std::shared_ptr<BtnScene>& currentScene, glm::vec2 viewPortSize, std::string name = "def");
		~BtnGuiLayer() override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event* e) override;
		void PullInput();

		void SetViewPortSize(glm::vec2 nViewPortSize);
		inline void SetViewPortOffset(glm::vec2 nOffset) { viewPortOffset = nOffset; }
		void SetFrameBuffer(std::shared_ptr<FrameBuffer> currentViewport);

		std::shared_ptr<BtnWidget> PickWidget(glm::vec2 screenPos);

		void SetCamera(CameraComponent* cam) {
			perspectiveCam = cam;
		}

		void UpdateCamera();

	private:
		struct CompareWidget {
			bool operator()(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>& tA, std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>& tB) const {
				auto& [a, ta, tab] = tA;
				auto& [b, tb, tbb] = tB;
				return a->GetLevel() > b->GetLevel();
			}
		};

		void RenderWidgets();
		void DrawWidget(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);
		void DrawChildren(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);
		void GenWidgetPQ();

		glm::mat4 CalculateModelMatrix(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);

		void RenderButton(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);
		void RenderSlider(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);
		void RenderNormal(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);
		void RenderText(std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>widgetPackage);

		void ProcessWidgetState(std::shared_ptr<BtnWidget> widget, glm::vec2 mouse,glm::vec2 canvasScreen, ViewPort viewPort);
		void ProcessSliderState(std::shared_ptr<BtnWidget> widget, glm::vec2 mouse,glm::vec2 canvasScreen, ViewPort viewPort);

		void ProcessSliderInput(std::shared_ptr<BtnSlider> slider, glm::vec2 deltaMouse, glm::vec2 sliderPos);

		std::shared_ptr<BtnScene>& currentScene;
		std::shared_ptr<FrameBuffer> frameBuffer;

		std::shared_ptr<Shader> widgetShader;
		std::shared_ptr<Shader> textShader;

		std::priority_queue<std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>,
			std::vector<std::tuple<std::shared_ptr<BtnWidget>, glm::mat4, glm::vec2>>, CompareWidget> widgets;

		CameraComponent* perspectiveCam = nullptr;

		CameraComponent camera;

		glm::vec2 viewPortSize;
		glm::vec2 viewPortOffset = glm::vec2(0.0f);
		std::string name;
	};
}