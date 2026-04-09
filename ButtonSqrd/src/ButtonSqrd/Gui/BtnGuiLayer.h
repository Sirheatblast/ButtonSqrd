#pragma once
#include"ButtonSqrd/Core/Layer.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Core/Scene.h"
#include"ButtonSqrd/Render/RendererCommand.h"
#include"ButtonSqrd/Render/Shader.h"
#include"ButtonSqrd/Render/FrameBuffer.h"
#include"BtnWidget.h"

#include"ButtonSqrd/Event/GuiEvent.h"

#include<queue>
#include<vector>
#include<memory>
#include<glm/glm.hpp>
namespace BtnSqd {
	class BtnGuiLayer : public Layer {
	public:
		BtnGuiLayer(std::shared_ptr<BtnScene> currentScene,glm::vec2 viewPortSize,std::string name = "def");
		~BtnGuiLayer() override;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event* e) override;

		void SetViewPortSize(glm::vec2 nViewPortSize);
		inline void SetViewPortOffset(glm::vec2 nOffset) { viewPortOffset = nOffset; }
		void SetFrameBuffer(std::shared_ptr<FrameBuffer> currentViewport);

		std::shared_ptr<BtnWidget> PickWidget(glm::vec2 screenPos);


	private:
		struct CompareWidget {
			bool operator()(const std::shared_ptr<BtnWidget>& a, const std::shared_ptr<BtnWidget>& b) const {
				return a->GetLevel() < b->GetLevel();
			}
		};

		void RenderWidgets();
		void GenWidgetPQ();
		void SetUpCamera();
		void PullInput();

		std::shared_ptr<BtnScene> currentScene;
		std::shared_ptr<FrameBuffer> frameBuffer;
		std::shared_ptr<Shader> widgetShader;
		std::priority_queue<std::shared_ptr<BtnWidget>,std::vector<std::shared_ptr<BtnWidget>>, CompareWidget> widgets;
		CameraComponent camera;
		glm::vec2 viewPortSize;
		glm::vec2 viewPortOffset = glm::vec2(0.0f);
		std::string name;
	};
}