#pragma once
#include"ButtonSqrd/Render/RendererApi.h"
#include"ButtonSqrd/Render/Model.h"
#include"ButtonSqrd/Render/BtnArmature.h"
#include"ButtonSqrd/Render/FrameBuffer.h"
#include"ButtonSqrd/Core/GameObject.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Render/ParticleQueue.h"

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<memory>

#define GL_POINT_SPRITE 0x8861

namespace BtnSqd {
	class OpenGLRenderApi :public RendererApi {
	public:
		OpenGLRenderApi() :directionalUBO(0), pointUBO(0), spotUBO(0) {
			particQueue.reset(new ParticleQueue(MAX_PARTICLES));
		}

		std::tuple<CameraComponent*, TransformComponent*> GetActiveCamera();

		void SetClearColor(const glm::vec4& clearColor) override;
		void Clear() override;
		void ClearDepth() override;
		void ResizeScreen(int width, int height) override;
		void DrawIndexed(GameObject& gameObject) override;
		void DrawIndexed(Model& model) override;
		void DrawIndexed(Mesh& mesh) override;
		void DrawIndexed(std::vector<Mesh> meshes) override;

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
		void Update()override;

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
			float transparency; //4
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
			int hasBones;
		};

		enum CurveFlags : uint32_t {
			CURVE_FADE = 1 << 0,
			CURVE_SIZE = 1 << 1,
			CURVE_COLOR = 1 << 2,
			CURVE_VEL = 1 << 3,
			PARTICLE_FADE = 1 << 4,
			PARTICLE_TCOL = 1 << 5,
			USE_ORIENTATION = 1 << 6,
		};


		void GenerateShadows();
		void GenerateReflections();
		void DrawColliders();
		void DrawBones();
		void HandleBones(std::shared_ptr<GameObject> gameObj, std::shared_ptr<Shader>shader);
		void ComputeBoneMats(BtnSqd::BtnArmature& armature, std::vector<glm::mat4>& boneMats);
		void UpdateAnimations(ArmNode& node, AnimatorComponent& aniComp, BtnArmature& armature);
		void BlendAnimations(BtnSqd::AnimatorComponent& aniComp, BtnSqd::BtnArmature& armature);

		std::vector<ParticleData> OnEmitParticle(ParticleEmitterComponent& Emitter, BtnTransform& transform);
		void ParticleEmitSelected(BtnSqd::ParticleEmitterComponent& Emitter, std::vector<BtnSqd::ParticleData>& p, BtnSqd::BtnTransform& transform);
		void ParticleEmitRandom(BtnSqd::ParticleEmitterComponent& Emitter, std::vector<BtnSqd::ParticleData>& p, unsigned int& pIdx, BtnSqd::BtnTransform& transform);
		void ParticleEmitLinear(BtnSqd::ParticleEmitterComponent& Emitter, std::vector<BtnSqd::ParticleData>& p, unsigned int& pIdx, BtnSqd::BtnTransform& transform);
		unsigned int CreateCurveTexture(const std::vector<glm::vec2>& curve);
		void CreateParticle(std::vector<BtnSqd::ParticleData>& p, int i, BtnSqd::ParticleEmitterComponent& Emitter, unsigned int& pIdx, BtnSqd::BtnTransform& transform);
		void ProcessParticles();
		void DrawParticles();
		void InitParticles();
		void CreateParticleTextureVAO();
		void RemakeParticleTextureArray();
		void RemakeParticleCurveArray();

		static std::map<ColliderType, std::function<Mesh(ColliderComponent, TransformComponent)>> colliderHandles;

		static Mesh GetCubicCollider(ColliderComponent collider, TransformComponent transform);
		static Mesh GetSphereCollider(ColliderComponent collider, TransformComponent transform);
		static Mesh GetCapsuleCollider(ColliderComponent collider, TransformComponent transform);

		std::shared_ptr<BtnScene> activeScene;
		std::shared_ptr<FrameBuffer> lightFrameBuffer = nullptr;
		std::shared_ptr<FrameBuffer> dirLightFrameBuffer = nullptr;
		std::shared_ptr<FrameBuffer> reflectionFrameBuffer = nullptr;
		std::shared_ptr<FrameBuffer> particleTextureFrameBuffer = nullptr;
		std::shared_ptr<Shader> depthShader;
		std::shared_ptr<Shader> pointDepthShader;
		std::shared_ptr<Shader> reflectionShader;
		std::shared_ptr<Shader> lineShader;
		std::shared_ptr<Shader> activeShader;
		std::shared_ptr<Shader> particleShader;
		std::shared_ptr<Shader> particleComputeShader;
		std::shared_ptr<Shader> copyTexShader;

		std::shared_ptr<ShaderStorageBuffer>ssbo;
		unsigned int pVao, quadVAO;

		unsigned int directionalUBO, pointUBO, spotUBO, lightUBO, directionalShadowArray, pointShadowlArray, spotShadowArray;
		unsigned int modelUBO, materialUBO;
		unsigned int boneUBO = 0;
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
		const int MAX_BONES = 128;
		const int MAX_PARTICLES = 50000;

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
		std::vector<ParticleData>particData;
		std::map<std::string, int>particleTextureKeys;
		std::map<std::string, int>particleCurveKeys;
		std::shared_ptr<ParticleQueue> particQueue;
		std::shared_ptr<Texture>particleCurveTexArray;


		int particleTextureCount = 0;
		void Init() override;
};
}