#pragma once
#include"VertexArray.h"
#include"Camera.h"
#include"Model.h"
#include"FrameBuffer.h"
#include"ButtonSqrd/Core/Scene.h"
#include"ButtonSqrd/Core/GameObject.h"
#include"ButtonSqrd/Core/CoreComponents.h"

#include<glm/glm.hpp>
#include<memory>
#include<vector>

namespace BtnSqd {
	class RendererApi {
	public:
		enum class API {
			NONE = 0, OPENGL = 1
		};

		virtual void Init()=0;

		virtual void SetClearColor(const glm::vec4& clearColor) = 0;
		virtual void SetCurrentCamera(std::tuple<CameraComponent*, TransformComponent*> camera) = 0;
		virtual void Clear() = 0;
		virtual void ClearDepth() = 0;
		virtual void ResizeScreen(int width, int height) = 0;

		virtual void DrawIndexed(GameObject& gameObject) = 0;
		virtual void DrawIndexed(Model& model) = 0;
		virtual void DrawIndexed(Mesh& mesh) = 0;
		virtual void DrawIndexed(std::vector<Mesh>meshes) = 0;

		virtual void DrawScene() = 0;
		virtual void DrawScene(std::shared_ptr<Shader> shader) = 0;
		virtual void DrawSkyBox() = 0;

		virtual void ShowColliders(bool show) = 0;

		virtual void InitSkybox() = 0;

		virtual void InitShadowBuffer() = 0;
		virtual void InitReflecitonsBuffer() = 0;

		virtual void EnableShadows() = 0;
		virtual void DisableShadows() = 0;

		virtual void EnableReflections() = 0;
		virtual void DisableReflection() = 0;
		virtual void ProcessLights() = 0;

		virtual void Update() = 0;

		virtual void DisableDepth() = 0;
		virtual void EnableDepth() = 0;

		virtual void EnableBackFraceCulling() = 0;
		virtual void DisableBackFraceCulling() = 0;

		virtual void SetActiveScene(std::shared_ptr<BtnScene> scene) = 0;
		virtual void SubmitToScene(Model& model) = 0;
		virtual void SubmitToScene(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetApi() { return api; }
	private:
		static API api;
	};
}