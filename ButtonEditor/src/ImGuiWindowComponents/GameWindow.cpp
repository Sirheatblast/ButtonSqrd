#include "GameWindow.h"

namespace BtnSqd {
	GameWindow::GameWindow(std::shared_ptr<BtnScene>& scene):currentScene(scene) {
		int width, height;
		width = BtnSqd::Application::GetApp()->GetWindow()->GetResolution().first;
		height = BtnSqd::Application::GetApp()->GetWindow()->GetResolution().second;

		gameViewport.reset(BtnSqd::FrameBuffer::Create(BtnSqd::FrameBufferAttatchment::ColorAttatchment, width, height));
		gameViewport->AttatchTexture(BtnSqd::TextureSlot::Albedo);
		gameViewport->AttatchRenderBuffer();
		gameViewport->UnBind();

		renderLayers.PushLayer(new GameLayer(currentScene, gameViewport));

	}

	void GameWindow::SendEvent(Event* e) {
		for (auto layer : renderLayers) {
			layer->OnEvent(e);
		}
	}

	void GameWindow::RenderWindow()	{
		for (auto layer : renderLayers) {
			layer->OnUpdate();
		}
	}
}
