#pragma once
#include"RendererApi.h"

namespace BtnSqd {
	class RenderCommand {
	public:
		static inline void Init() { renApi->Init(); }

		static inline void SetClearColor(const glm::vec4& clearColor) { renApi->SetClearColor(clearColor); }
		static inline void Clear() { renApi->Clear(); }
		static inline void ClearDepth() { renApi->ClearDepth(); }
		static inline void DrawIndexed(GameObject& gameObject) { renApi->DrawIndexed(gameObject); }
		static inline void SetCurrentCamera(std::tuple<CameraComponent*, TransformComponent*> camera) { renApi->SetCurrentCamera(camera); }

		static inline void DisableDepth() { renApi->DisableDepth(); }
		static inline void EnableDepth() { renApi->EnableDepth(); }
		static inline void EnableBackFaceCulling() { renApi->EnableBackFraceCulling(); }
		static inline void DisableBackFaceCulling() { renApi->DisableBackFraceCulling(); }

		static inline void SetActiveScene(std::shared_ptr<BtnScene> scene) { renApi->SetActiveScene(scene); }

		static inline void SubmitToScene(Model& model) { renApi->SubmitToScene(model); }
		static inline void SubmitToScene(const std::shared_ptr<VertexArray>& vertexArray) { renApi->SubmitToScene(vertexArray); }

		static inline void DrawScene() { renApi->DrawScene(); }
		static inline void DrawScene(std::shared_ptr<Shader> shader) { renApi->DrawScene(shader); }
		static inline void DrawSkyBox() { renApi->DrawSkyBox(); }
		static inline void DrawMesh(Mesh& mesh) { renApi->DrawIndexed(mesh); }
		static inline void DrawMeshes(std::vector<Mesh> meshes) { renApi->DrawIndexed(meshes); }

		static inline void EnableShadows() { renApi->EnableShadows(); }
		static inline void DisableShadows() { renApi->DisableShadows(); }
		static inline void InitShadowBuffer() { renApi->InitShadowBuffer(); }

		static inline void ShowColliders(bool show) { renApi->ShowColliders(show); }

		static inline void EnableReflections() { renApi->EnableReflections(); }
		static inline void DisableReflections() { renApi->DisableReflection(); }
		static inline void InitReflectionsBuffer() { renApi->InitReflecitonsBuffer(); }

		static inline void InitSkybox() { renApi->InitSkybox(); }

		static inline void ProcessLights() { renApi->ProcessLights(); }
		static inline void UpdateRenderer() { renApi->Update(); }

		static inline void ResizeScreen(int width, int height) { renApi->ResizeScreen(width, height); }

	private:
		static RendererApi* renApi;
	};
}