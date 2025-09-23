#pragma once
#include"ButtonSqrd/Render/RendererApi.h"
#include"ButtonSqrd/Render/Model.h"
#include"ButtonSqrd/Render/BtnArmature.h"
#include"ButtonSqrd/Render/FrameBuffer.h"
#include"ButtonSqrd/Core/GameObject.h"
#include"ButtonSqrd/Core/CoreComponents.h"

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<memory>
namespace BtnSqd {
	class OpenGLRenderApi :public RendererApi {
	public:
		OpenGLRenderApi():directionalUBO(0),pointUBO(0),spotUBO(0){}

		std::tuple<CameraComponent*, TransformComponent*> GetActiveCamera();

		void SetClearColor(const glm::vec4& clearColor) override;
		void Clear() override;
		void ClearDepth() override;
		void ResizeScreen(int width, int height) override;
		void DrawIndexed(GameObject& gameObject) override;
		void DrawIndexed(Model& model) override;
		void DrawIndexed(Mesh& mesh) override;
		void SetCurrentCamera(std::tuple<CameraComponent*, TransformComponent*> camera) override;
		void DisableDepth() override;
		void EnableDepth() override;
		void EnableBackFraceCulling() override;
		void DisableBackFraceCulling() override;
		void SetActiveScene(std::shared_ptr<BtnScene> scene) override;
		void SubmitToScene(Model& model) override;
		void SubmitToScene(const std::shared_ptr<VertexArray>& vertexArray) override;

		void DrawScene() override;
		void DrawScene(std::shared_ptr<Shader> shader) override;
		void DrawSkyBox() override;

		void InitSkybox() override;
		void ProcessLights() override;

		void InitShadowBuffer() override;
		void EnableShadows() override;

		void InitReflecitonsBuffer() override;
		void EnableReflections() override;

		void DisableReflection() override;
		void DisableShadows() override;

		void ShowColliders(bool show) override;
	private:
		struct MaterialData {
			float metallic; //4
			float roughness; //4
			float reflectance; //4
			float ao; //4
			glm::vec3 clearColor; //12
			float padding; //4
		};

		struct ModelData {
			glm::mat4 VP;               // 64 bytes
			glm::mat4 inverseVP;        // 64 bytes
			glm::mat4 model;            // 64 bytes
			int numDirectionalLights;   // 4 bytes
			int numPointLights;         // 4 bytes
			int numSpotLights;          // 4 bytes
			int padding1;              // 4 bytes
			glm::vec3 camPos;           // 12 bytes
			float nearPlane;            // 4 bytes
			float farPlane;             // 4 bytes
			float padding2;            // 4 bytes
		};

		void GenerateShadows();
		void GenerateReflections();
		void DrawColliders();
		void HandleBones(std::shared_ptr<GameObject> gameObj,std::shared_ptr<Shader>shader);
		void ComputeBoneMats(BtnSqd::BtnArmature& armature,std::vector<glm::mat4>& boneMats);

		static std::map<ColliderType, std::function<Mesh(ColliderComponent, TransformComponent)>> colliderHandles;

		static Mesh GetCubicCollider(ColliderComponent collider,TransformComponent transform);
		static Mesh GetSphereCollider(ColliderComponent collider,TransformComponent transform);
		static Mesh GetCapsuleCollider(ColliderComponent collider,TransformComponent transform);

		std::shared_ptr<BtnScene> activeScene;
		std::shared_ptr<FrameBuffer> lightFrameBuffer = nullptr;
		std::shared_ptr<FrameBuffer> dirLightFrameBuffer = nullptr;
		std::shared_ptr<FrameBuffer> reflectionFrameBuffer = nullptr;
		std::shared_ptr<Shader> depthShader;
		std::shared_ptr<Shader> pointDepthShader;
		std::shared_ptr<Shader> reflectionShader;
		std::shared_ptr<Shader> lineShader;
		std::shared_ptr<Shader> activeShader;

		unsigned int directionalUBO,pointUBO,spotUBO,lightUBO,directionalShadowArray,pointShadowlArray,spotShadowArray;
		unsigned int modelUBO, materialUBO;
		unsigned int maxLights = 50;
		unsigned int loadedLights = 0;
		//far and near planes for shadowmaps only, main rendering near and farplane is found in the camera component
		float farPlane = 100.0f;
		float nearPlane = 0.1f;
		float amibentOclusion = 0.5f;

		const int MAX_MATERIALS = 256;
		const int MAX_DIRECTIONAL_LIGHTS = 8;
		const int MAX_POINT_LIGHTS = 16;
		const int MAX_SPOT_LIGHTS = 16;

		bool firstTime = true;
		bool useSSR = true;
		bool enableShadows = true;
		bool showColliders = false;

		Model skybox;
		std::string skyboxPath;

		CameraComponent* activeCamera;
		TransformComponent* cameraTransform;

		std::vector<DirectionalLightStruct> directionalLights;
		std::vector<PointLightStruct> pointLights;
		std::vector<SpotLightStruct> spotLights;

		void Init() override;
};
}