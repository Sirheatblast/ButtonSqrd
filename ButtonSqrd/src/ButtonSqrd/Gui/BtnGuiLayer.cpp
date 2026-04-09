#include"BtnGuiLayer.h"
#include"ButtonSqrd.h"

namespace BtnSqd {
	BtnGuiLayer::BtnGuiLayer(std::shared_ptr<BtnScene> currentScene,glm::vec2 viewPortSize, std::string name):currentScene(currentScene), viewPortSize(viewPortSize), name(name) {
		camera.fov = 80.0f;
		camera.nearPlain = -1.0f;
		camera.farPlain = 1.0f;
		camera.isMainCamera = false;

		SetUpCamera();

	}
	BtnGuiLayer::~BtnGuiLayer() {

	}
	void BtnGuiLayer::OnAttach() {
		
	}
	void BtnGuiLayer::OnDetach() {

	}
	void BtnGuiLayer::OnUpdate() {
		GenWidgetPQ();
		PullInput();
		RenderWidgets();
	}
	void BtnGuiLayer::OnEvent(Event* e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<OnEnableGuiEvent>([this](OnEnableGuiEvent* e)->bool {
			if (e->GetDest()==name) {
				SetEnabled(true);
			}
			return true; 
		});
		dispatcher.Dispatch<OnDisableGuiEvent>([this](OnDisableGuiEvent* e)->bool{
			if (e->GetDest() == name) {
				SetEnabled(false);
			}
			return true;
		});
	}
	void BtnGuiLayer::SetViewPortSize(glm::vec2 nViewPortSize) {
		viewPortSize = nViewPortSize;
		SetUpCamera();
	}
	void BtnGuiLayer::SetFrameBuffer(std::shared_ptr<FrameBuffer> currentViewport) {
		frameBuffer = currentViewport;
	}
	std::shared_ptr<BtnWidget> BtnGuiLayer::PickWidget(glm::vec2 screenPos) {
		glm::vec2 mousePos = screenPos - viewPortOffset;
		std::shared_ptr<BtnWidget> selected;
		for (const auto& widget : currentScene->GetWidgets()) {
			glm::vec2 startPos = widget->GetPos();
			glm::vec2 endPos = startPos + widget->GetDimensions();

			if (mousePos.x>startPos.x&&mousePos.x<endPos.x &&
				mousePos.y>startPos.y && mousePos.y < endPos.y) {

				selected = widget;
				widget->SetHover(true);
			}
			else {
				widget->SetHover(false);
			}
		}
		return selected;
	}
		void BtnGuiLayer::RenderWidgets() {
		if (!widgetShader) {
			widgetShader = ResourceManager::GetLoadedShaders()["BasicShader"];
		}

		if (frameBuffer) {
			frameBuffer->ABind();
		}

		widgetShader->Use();
		widgetShader->SetMat4("VP", camera.projectionMatrix*camera.viewMatrix);

		while (!widgets.empty()) {
			auto widget = widgets.top();
			widgets.pop();
			
			if (widget->GetUseScreenDim()) {
				float max = 100.0f;
				glm::vec2 windSize = viewPortSize;
				glm::vec2 wPos = widget->GetPercentPos();
				glm::vec2 wPercent = wPos / max;

				glm::vec2 newScreenPos = (wPercent * (windSize - widget->GetDimensions()));
				widget->SetPos(newScreenPos);
			}

			glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(widget->GetPos(), 0.5f));
			auto dimensions = widget->GetDimensions();

			widgetShader->SetMat4("model", modelMat);
			widgetShader->SetVec3("clearColor", widget->GetColor());
			RenderCommand::DrawMesh(widget->Draw());
		}
		widgetShader->Detatch();

		if (frameBuffer) {
			frameBuffer->UnBind();
		}
	}
	void BtnGuiLayer::GenWidgetPQ() {
		for (const auto& Widget : currentScene->GetWidgets()) {
			widgets.push(Widget);
		}
	}
	void BtnGuiLayer::SetUpCamera() {
		camera.viewMatrix = glm::mat4(1.0f);
		camera.projectionMatrix = glm::ortho(0.0f,viewPortSize.x,viewPortSize.y,0.0f,camera.nearPlain,camera.farPlain);
	}
	void BtnGuiLayer::PullInput() {

	}
}