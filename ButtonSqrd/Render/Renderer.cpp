#include"Renderer.h"

namespace BtnSqd {
	void Renderer::BeginScene(std::shared_ptr<BtnScene> scene){
		RenderCommand::SetActiveScene(scene);
	}
	void Renderer::EndScene() {
		
	}

	void Renderer::SubmitToScene(Model& model) {
		RenderCommand::SubmitToScene(model);
	}

	void Renderer::DrawSkyBox() {
		RenderCommand::DrawSkyBox();
	}
	void Renderer::DrawScene() {
		RenderCommand::DrawScene();
	}
	void Renderer::DrawScene(std::shared_ptr<Shader> shader) {
		RenderCommand::DrawScene(shader);
	}

	void Renderer::DrawMesh(Mesh mesh) {
		RenderCommand::DrawMesh(mesh);
	}
	void Renderer::DisableDepth(){
		RenderCommand::DisableDepth();
	}
	void Renderer::EnableDepth(){
		RenderCommand::EnableDepth();
	}
}