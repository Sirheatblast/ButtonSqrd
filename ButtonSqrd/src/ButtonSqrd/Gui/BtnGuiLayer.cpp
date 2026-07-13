#include"BtnGuiLayer.h"
#include"ButtonSqrd.h"
#include"ButtonSqrd/Gui/Widgets/BtnTextBox.h"
#include"ButtonSqrd/Gui/Widgets/BtnButton.h"

namespace BtnSqd {
	BtnGuiLayer::BtnGuiLayer(std::shared_ptr<BtnScene>& currentScene, glm::vec2 viewPortSize, std::string name) :currentScene(currentScene), viewPortSize(viewPortSize), name(name) {
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
		RenderWidgets();
	}
	void BtnGuiLayer::OnEvent(Event* e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<OnEnableGuiEvent>([this](OnEnableGuiEvent* e)->bool {
			if (e->GetDest() == name) {
				SetEnabled(true);
			}
			return true;
											  });
		dispatcher.Dispatch<OnDisableGuiEvent>([this](OnDisableGuiEvent* e)->bool {
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

			if (glm::all(glm::lessThanEqual(mousePos, endPos)) &&
				glm::all(glm::greaterThanEqual(mousePos, startPos))) {
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
			widgetShader = ResourceManager::GetLoadedShaders()["WidgetShader"];
		}
		if (!textShader) {
			textShader = ResourceManager::GetLoadedShaders()["TextShader"];
		}

		if (frameBuffer) {
			frameBuffer->ABind();
		}

		while (!widgets.empty()) {
			auto widget = widgets.top();
			widgets.pop();
			DrawChildren(widget.get());
		}
		widgetShader->Detatch();

		if (frameBuffer) {
			frameBuffer->UnBind();
		}

	}

	void BtnGuiLayer::DrawWidget(BtnWidget* widget) {
		if (widget->GetUseScreenDim()) {
			float max = 100.0f;
			glm::vec2 windSize = viewPortSize;
			glm::vec2 wPos = widget->GetPercentPos();
			glm::vec2 wPercent = wPos / max;

			glm::vec2 newScreenPos = (wPercent * (windSize - widget->GetDimensions()));
			widget->SetPos(newScreenPos);
		}

		switch (widget->GetType()) {
		case BtnWidgetType::Text:
			RenderText(widget);
			break;
		case BtnWidgetType::Button:
			RenderButton(widget);
			break;
		default:
			RenderNormal(widget);
			break;
		}
	}

	void BtnGuiLayer::DrawChildren(BtnWidget* widget) {
		DrawWidget(widget);
		for (auto child : widget->GetChildren()) {
			if (!child) {
				widget->RemoveChild(child);
				continue;
			}
			DrawChildren(child);
		}
	}

	void BtnGuiLayer::RenderButton(BtnWidget* widget) {
		widgetShader->Use();
		widgetShader->SetMat4("VP", camera.projectionMatrix * camera.viewMatrix);
		float level = widget->GetLevel() / 10.0f;

		if (widget->HasParent()) {
			level += widget->GetParent()->GetLevel() + 0.01f;
		}

		glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(widget->GetPos(), level)); //fix this so that it could render widgets not from fixed world positions
		auto dimensions = widget->GetDimensions();

		BtnButton* button = static_cast<BtnButton*>(widget);
		glm::vec4 clearColor = button->GetColor();

		auto buttonMesh = widget->Draw(widgetShader);
		bool shouldMix = widget->GetMix();

		if (button->GetClicked()) {
			if (button->GetUseClickColor()) {
				clearColor = button->GetClickColor();
				shouldMix = true;
			}
			if (button->GetUseClickTexture() && button->GetClickTexture()) {
				buttonMesh.GetMaterial()->albedo = button->GetClickTexture();
			}
		}
		else if (button->GetHovered()) {
			if (button->GetUseHoverColor()) {
				clearColor = button->GetHoverColor();
				shouldMix = true;
			}
			if (button->GetUseHoverTexture() && button->GetHoverTexture()) {
				buttonMesh.GetMaterial()->albedo = button->GetHoverTexture();
			}
		}

		widgetShader->SetMat4("model", modelMat);
		widgetShader->SetVec4("clearColor", clearColor);
		widgetShader->SetVec2("widgetSize", widget->GetDimensions());
		widgetShader->SetBool("useAlbedoTexture", widget->GetHasTexture());
		widgetShader->SetBool("mixTex", shouldMix);
		RenderCommand::DrawMesh(buttonMesh);

		widgetShader->Detatch();
	}

	void BtnGuiLayer::RenderNormal(BtnWidget* widget) {
		widgetShader->Use();
		widgetShader->SetMat4("VP", camera.projectionMatrix * camera.viewMatrix);

		float level = widget->GetLevel() / 10.0f;

		if (widget->HasParent()) {
			level += widget->GetParent()->GetLevel() + 0.01f;
		}

		glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(widget->GetPos(), level)); //fix this so that it could render widgets not from fixed world positions
		auto dimensions = widget->GetDimensions();

		widgetShader->SetMat4("model", modelMat);
		widgetShader->SetVec4("clearColor", widget->GetColor());
		widgetShader->SetVec2("widgetSize", widget->GetDimensions());
		widgetShader->SetBool("useAlbedoTexture", widget->GetHasTexture());
		widgetShader->SetBool("mixTex", widget->GetMix());
		RenderCommand::DrawMesh(widget->Draw(widgetShader));
		widgetShader->Detatch();
	}

	void BtnGuiLayer::RenderText(BtnWidget* widget) {
		textShader->Use();

		textShader->SetMat4("VP", camera.projectionMatrix * camera.viewMatrix);

		float level = widget->GetLevel() / 10.0f;
		if (widget->HasParent()) {
			level += widget->GetParent()->GetLevel() + 0.01f;
		}

		glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(widget->GetPos(), level)); //fix this so that it could render widgets not from fixed world positions
		auto dimensions = widget->GetDimensions();
		textShader->SetMat4("model", modelMat);

		BtnTextBox* text = static_cast<BtnTextBox*>(widget);
		textShader->SetUniform("fontAtlas", 0);
		if (text->GetFont().GetFontTexture()) {
			text->GetFont().GetFontTexture()->Bind();
		}

		textShader->SetVec4("textColor", text->GetColor());
		textShader->SetVec4("backColor", text->GetBackgroundColor());
		textShader->SetFloat("pixelRange", 3.0f);
		RenderCommand::DrawMesh(text->Draw(textShader));
		textShader->Detatch();
	}

	void BtnGuiLayer::GenWidgetPQ() {
		for (const auto& widget : currentScene->GetWidgets()) {
			if (!widget->GetIsEnabled() || widget->HasParent()) {
				continue;
			}
			widget->UpdateChildrenPos(widget.get());
			widgets.push(widget);
		}
	}
	void BtnGuiLayer::SetUpCamera() {
		camera.viewMatrix = glm::mat4(1.0f);
		camera.projectionMatrix = glm::ortho(0.0f, viewPortSize.x, viewPortSize.y, 0.0f, camera.nearPlain, camera.farPlain);
	}

	void BtnGuiLayer::PullInput() {
		GenWidgetPQ();

		glm::vec2 mouse = Input::GetMousePosition();
		ViewPort viewPort = Application::GetApp()->GetCurrentViewPort();

		while (!widgets.empty()) {
			auto widget = widgets.top();
			widgets.pop();

			if (widget->GetUseScreenDim()) {
				float max = 100.0f;
				glm::vec2 windSize = viewPort.size;
				glm::vec2 wPos = widget->GetPercentPos();
				glm::vec2 wPercent = wPos / max;

				glm::vec2 newScreenPos = (wPercent * (windSize - widget->GetDimensions()));
				widget->SetPos(newScreenPos);
			}

			if (!widget->GetIsInteractive()) {
				continue;
			}

			glm::vec2 screenWidgetPos = viewPort.offset + widget->GetPos();
			glm::vec2 widgetHigh = screenWidgetPos + widget->GetDimensions();

			if (glm::all(glm::lessThanEqual(mouse, widgetHigh)) &&
				glm::all(glm::greaterThanEqual(mouse, screenWidgetPos))) {
				widget->SetHover(true);
			}
			else {
				widget->SetHover(false);
			}

			if (widget->GetHovered() && Input::IsMouseButtonPressed(MouseCode::Left)) {
				widget->SetClicked(true);
				widget->OnClick();
			}
			else {
				if (widget->GetClicked() && Input::IsMouseButtonUp(MouseCode::Left)) {
					widget->OnClickUp();
				}

				widget->SetClicked(false);
			}
		}
	}
}