#pragma once
#include"ButtonSqrd/Core/core.h"
#include"RendererCommand.h"
#include"Camera.h"

#include<glm/glm.hpp>
#include<memory>

namespace BtnSqd {
	class Renderer {
	public:
		Renderer() = default;

		virtual ~Renderer() = default;
		
		static void BeginScene(std::shared_ptr<BtnScene> scene);
		static void EndScene();

		static void SubmitToScene(Model& model);

		static void DrawSkyBox();
		
		static void DrawScene();
		static void DrawScene(std::shared_ptr<Shader> shader);

		static void DrawMesh(Mesh mesh);

		static void DisableDepth();
		static void EnableDepth();

		static inline RendererApi::API GetApi() { return RendererApi::GetApi(); }
	};
}