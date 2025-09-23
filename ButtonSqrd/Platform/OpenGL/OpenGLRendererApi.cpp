#include"OpenGLRendererApi.h"
#include"OpenGLTexture.h"
#include"ButtonSqrd/Core/Application.h"

namespace BtnSqd {
	void OpenGLRenderApi::Init() {
		glGenBuffers(1, &directionalUBO);
		glGenBuffers(1, &pointUBO);
		glGenBuffers(1, &spotUBO);
		glGenBuffers(1, &modelUBO);
		glGenBuffers(1, &materialUBO);

		glBindBuffer(GL_UNIFORM_BUFFER, directionalUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLightStruct) * MAX_DIRECTIONAL_LIGHTS, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, pointUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightStruct) * MAX_POINT_LIGHTS, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, spotUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightStruct) * MAX_SPOT_LIGHTS, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, modelUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ModelData), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void OpenGLRenderApi::ShowColliders(bool show) {
		showColliders = show;
	}
	void OpenGLRenderApi::DisableShadows() {
		enableShadows = false;
	}
	std::tuple<CameraComponent*,TransformComponent*> OpenGLRenderApi::GetActiveCamera()	{
		return std::make_tuple(activeCamera,cameraTransform);
	}
	void OpenGLRenderApi::SetClearColor(const glm::vec4& clearColor){
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}
	void OpenGLRenderApi::Clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRenderApi::ClearDepth()	{
		glClear( GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRenderApi::DrawIndexed(Mesh& mesh){

		mesh.Draw();
		glDrawElements(GL_TRIANGLES, mesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRenderApi::DrawIndexed(Model& model) {
		model.GetShader()->Use();
		model.GetShader()->SetMat4("VP", activeCamera->projectionMatrix * activeCamera->viewMatrix);
		model.GetShader()->SetMat4("projectionMat", activeCamera->projectionMatrix);
		model.GetShader()->SetVec3("camPos", cameraTransform->transform.GetPosition());

		for (Mesh mesh : *model.GetMeshes()) {
			mesh.Draw(model.GetShader().get());
			model.GetShader()->SetFloat("metallic", mesh.GetMaterial()->metallic);
			model.GetShader()->SetFloat("roughness", mesh.GetMaterial()->roughness);
			model.GetShader()->SetFloat("reflectance", mesh.GetMaterial()->reflectance);
			model.GetShader()->SetFloat("ao", 0.1f);
			glDrawElements(GL_TRIANGLES, mesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		model.GetShader()->Detatch();
	}
	void OpenGLRenderApi::DrawIndexed(GameObject& gameObject){
		std::vector<glm::vec3>lightPos;
		std::vector<glm::vec3>lightColors;
		std::vector<float> lightStrength;
		Model model = gameObject.GetComponent<ModelComponent>().currentModel;
		BtnTransform transform = gameObject.GetComponent<TransformComponent>().transform;

		model.GetShader()->Use();
		model.GetShader()->SetMat4("VP", activeCamera->projectionMatrix * activeCamera->viewMatrix);
		model.GetShader()->SetMat4("projectionMat", activeCamera->projectionMatrix);
		model.GetShader()->SetMat4("model", transform.GetMatrix());
		model.GetShader()->SetVec3Array("lightPositions", lightPos);
		model.GetShader()->SetVec3Array("lightColors", lightColors);
		model.GetShader()->SetFloatArray("lightStrength", lightStrength);
		model.GetShader()->SetVec3("camPos", cameraTransform->transform.GetPosition());

		for (Mesh mesh : *model.GetMeshes()) {
			mesh.Draw(model.GetShader().get());
			model.GetShader()->SetFloat("metallic", mesh.GetMaterial()->metallic);
			model.GetShader()->SetFloat("roughness", mesh.GetMaterial()->roughness);
			model.GetShader()->SetFloat("reflectance", mesh.GetMaterial()->reflectance);
			model.GetShader()->SetFloat("ao", 0.1f);
			glDrawElements(GL_TRIANGLES, mesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
		model.GetShader()->Detatch();
	}
	void OpenGLRenderApi::SetCurrentCamera(std::tuple<CameraComponent*, TransformComponent*> camera){
		auto& [camComp,transfComp] = camera;
		activeCamera = camComp;
		cameraTransform = transfComp;
	}

	void OpenGLRenderApi::DisableDepth() {
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
	}
	void OpenGLRenderApi::EnableDepth() {
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
	void OpenGLRenderApi::EnableBackFraceCulling() {
		glEnable(GL_CULL_FACE);
	}
	void OpenGLRenderApi::DisableBackFraceCulling() {
		glDisable(GL_CULL_FACE);
	}
	void OpenGLRenderApi::SetActiveScene(std::shared_ptr<BtnScene> scene) {
		if (activeScene) {
			if (activeScene != scene) {
				activeScene = scene;
				GetActiveCamera();
			}
		}
		else {
			activeScene = scene;
			GetActiveCamera();
		}
	}
	void OpenGLRenderApi::SubmitToScene(Model& model) {

	}
	void OpenGLRenderApi::SubmitToScene(const std::shared_ptr<VertexArray>& vertexArray) {
		//TODO create a new model from vertexArray
	}
	void OpenGLRenderApi::DrawScene(){
		if (lightFrameBuffer) {
			if (dirLightFrameBuffer->GetTextureArrays().size() > 0) {
				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D_ARRAY, dirLightFrameBuffer->GetTextureArrays()[0]);
			}
			if (lightFrameBuffer->GetTextureArrays().size() > 0) {
				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, lightFrameBuffer->GetTextureArrays()[0]);
				glActiveTexture(GL_TEXTURE7);
				glBindTexture(GL_TEXTURE_2D_ARRAY, lightFrameBuffer->GetTextureArrays()[1]);
			}
		}

		reflectionFrameBuffer->GetTexture()->Bind();
		reflectionFrameBuffer->GetDepthTexture()->Bind();

		// Use UBOs for light data
		bool hasSetUBO = (directionalLights.size() +pointLights.size() + spotLights.size() == loadedLights) ? true : false;

		glBindBuffer(GL_UNIFORM_BUFFER, directionalUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightStruct) * directionalLights.size(), directionalLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, pointUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLightStruct) * pointLights.size(),pointLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, spotUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SpotLightStruct) * spotLights.size(), spotLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		for (auto& [gameId,gameModels, gameTransform] : activeScene->GetRegister().GetAllOfID<ModelComponent,TransformComponent>()) {
			Model model = gameModels.currentModel;
			std::shared_ptr<Shader> shader = model.GetShader();
			if (model.GetMeshes()->size()==0) {
				continue;
			}
			bool hasBones = false;

			shader->Use();
			shader->SetBool("useSSR", useSSR);
			shader->SetBool("enableShadows", enableShadows);
			if (activeScene->GetgameObjects()[gameId]->CheckObjectForComponent<ArmatureComponent>()) {
				hasBones = true;
				shader->SetBool("hasBones", hasBones);
				HandleBones(activeScene->GetgameObjects()[gameId],shader);
			}
			else {
				shader->SetBool("hasBones", hasBones);
			}

			if (!hasSetUBO) {
				unsigned int directionalLightIndex = glGetUniformBlockIndex(shader->GetId(), "directionalLightInfo");
				unsigned int pointLightIndex = glGetUniformBlockIndex(shader->GetId(), "pointLightInfo");
				unsigned int spotLightIndex = glGetUniformBlockIndex(shader->GetId(), "spotLightInfo");
				unsigned int modelDataIndex = glGetUniformBlockIndex(shader->GetId(), "ModelData");
				unsigned int materialDataIndex = glGetUniformBlockIndex(shader->GetId(), "MaterialData");
			
				glUniformBlockBinding(shader->GetId(), directionalLightIndex, 0);
				glUniformBlockBinding(shader->GetId(), pointLightIndex, 1);
				glUniformBlockBinding(shader->GetId(), spotLightIndex, 2);
				glUniformBlockBinding(shader->GetId(), modelDataIndex, 3);
				glUniformBlockBinding(shader->GetId(), materialDataIndex, 4);
				hasSetUBO = true;
			}

			glBindBufferBase(GL_UNIFORM_BUFFER, 0, directionalUBO);
			glBindBufferBase(GL_UNIFORM_BUFFER, 1, pointUBO);
			glBindBufferBase(GL_UNIFORM_BUFFER, 2, spotUBO);

			if (firstTime) {
				shader->SetUniform("ourTexture", 0);
				shader->SetUniform("ourNormalMap", 1);
				shader->SetUniform("shadowMap", 3);
				shader->SetUniform("envCubemap", 4);
				shader->SetUniform("directionalShadowMaps", 5);
				shader->SetUniform("pointShadowMaps", 6);
				shader->SetUniform("spotShadowMaps", 7);
				shader->SetUniform("reflectionMap", 8);
				shader->SetUniform("depthMap", 9);
				firstTime = false;
			}

			ModelData modelData;
			modelData.VP = activeCamera->projectionMatrix * activeCamera->viewMatrix;
			modelData.inverseVP = glm::inverse(activeCamera->projectionMatrix * activeCamera->viewMatrix);
			modelData.model = gameTransform.transform.GetMatrix();

			modelData.numDirectionalLights = static_cast<int>(directionalLights.size());
			modelData.numPointLights = static_cast<int>(pointLights.size());
			modelData.numSpotLights = static_cast<int>(spotLights.size());

			modelData.camPos = cameraTransform->transform.GetPosition();
			modelData.nearPlane = nearPlane;
			modelData.farPlane = farPlane;

			glBindBuffer(GL_UNIFORM_BUFFER, modelUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ModelData), &modelData);
			glBindBufferBase(GL_UNIFORM_BUFFER, 3, modelUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);


			for (Mesh mesh : *model.GetMeshes()) {
				Material* material = mesh.GetMaterial();

				MaterialData materialData;
				materialData.metallic = material->metallic;
				materialData.roughness = material->roughness;
				materialData.reflectance = material->reflectance;
				materialData.ao = amibentOclusion;
				materialData.clearColor = material->clearColor;

				mesh.Draw(shader.get());

				glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialData), &materialData);
				glBindBufferBase(GL_UNIFORM_BUFFER, 4, materialUBO);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				glDrawElements(GL_TRIANGLES, mesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
				mesh.GetVertexArray()->Unbind();
			}
		}

		if (showColliders) {
			DrawColliders();
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);		
	}
	void OpenGLRenderApi::DrawScene(std::shared_ptr<Shader> shader) {
		if (!activeCamera || !activeScene) {
			return;
		}
		shader->Use();

		shader->SetMat4("VP", activeCamera->projectionMatrix * activeCamera->viewMatrix);
		shader->SetVec3("camPos", cameraTransform->transform.GetPosition());
		shader->SetFloat("farPlane", farPlane);

		unsigned int i=0;
		for (auto& [gameObjId, gameModel, gameTransform] : activeScene->GetRegister().GetAllOfID<ModelComponent,TransformComponent>()) {
			Model model = gameModel.currentModel;
			shader->SetUnsignedInt("objectId", gameObjId);
			shader->SetUnsignedInt("drawId", i+1);
			//HandleBones(activeScene->GetgameObjects()[gameObjId], shader); --causing issues with bool in shader

			shader->SetMat4("model", gameTransform.transform.GetMatrix());
			for (Mesh& mesh : *model.GetMeshes()) {
				Material* material = mesh.GetMaterial();

				MaterialData materialData;
				materialData.metallic = material->metallic;
				materialData.roughness = material->roughness;
				materialData.reflectance = material->reflectance;
				materialData.ao = amibentOclusion;
				materialData.clearColor = material->clearColor;

				mesh.Draw(shader.get());

				glBindBuffer(GL_UNIFORM_BUFFER, materialUBO);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialData), &materialData);
				glBindBufferBase(GL_UNIFORM_BUFFER, 4, materialUBO);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				glDrawElements(GL_TRIANGLES, mesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
				mesh.GetVertexArray()->Unbind();
			}
			i++;
		}
	}

	void OpenGLRenderApi::DrawSkyBox() {
		if (activeScene->GetSkyboxTexture()) {

			std::shared_ptr<Texture>& skyTex = activeScene->GetSkyboxTexture();

			if (skyTex) {
				if(skyTex->GetPath()!=skyboxPath) {
					skyboxPath = skyTex->GetPath();
					BTNLOG_TRACE("Loaded new skybox {}", skyTex->GetPath());
					skybox.GetMeshes()->data()[0].SetTexture(ResourceManager::GetLoadedSkyboxes()[skyTex->GetPath()], TextureSlot::Texture4);
				}
			}

			skybox.GetMeshes()->data()[0].GetMaterial()->albedo->Bind(TextureSlot::Texture4);

			auto camera = *activeCamera;
			skybox.GetShader()->SetMat4("projection", camera.projectionMatrix);
			skybox.GetShader()->SetMat4("view", glm::mat4(glm::mat3(camera.viewMatrix)));
			DisableDepth();
			DisableBackFraceCulling();
			DrawIndexed(skybox);
			EnableBackFraceCulling();
			EnableDepth();
		}
	}
	void OpenGLRenderApi::InitSkybox(){
		skybox = Model("./Assets/Models/Cube.fbx", Shader::Create("./Assets/Shaders/SkyboxShader.vrt", "./Assets/Shaders/SkyboxShader.frag"));
	}
	void OpenGLRenderApi::InitShadowBuffer() {
		lightFrameBuffer.reset(FrameBuffer::Create(FrameBufferAttatchment::DepthAttatchment, 512, 512));
		lightFrameBuffer->UnBind();
		dirLightFrameBuffer.reset(FrameBuffer::Create(FrameBufferAttatchment::DepthAttatchment, 2048, 2048));
		dirLightFrameBuffer->UnBind();

		if (lightFrameBuffer->GetStatus() || dirLightFrameBuffer->GetStatus()) {
			BTNLOG_ERROR("FRAMEBUFFER IS NOT COMPLETE");
		}

		depthShader.reset(Shader::Create("./Assets/Shaders/DepthShader.vrt","./Assets/Shaders/DepthShader.frag"));
		pointDepthShader.reset(Shader::Create("./Assets/Shaders/PointDepthShader.vrt", "./Assets/Shaders/PointDepthShader.geo", "./Assets/Shaders/PointDepthShader.frag"));
	}
	void OpenGLRenderApi::EnableShadows() {
		enableShadows = true;
		dirLightFrameBuffer->ClearTextureArrays();
		lightFrameBuffer->ClearTextureArrays();

		GenerateShadows();
	}
	void OpenGLRenderApi::GenerateShadows()	{
		if (!activeCamera) {
			return;
		}
		glm::mat4 lightProjection, lightView;		
		glm::vec3 camPos = cameraTransform->transform.GetPosition();
		glm::vec3 camFront = cameraTransform->transform.front;
		float directionalViewPortSize = 50.0f;

		dirLightFrameBuffer->Bind();
		//directional shadows
		dirLightFrameBuffer->AttatchTextureArray(TextureType::Texture_2D,(unsigned int)directionalLights.size());
		for (unsigned int i = 0; i < directionalLights.size(); i++) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, dirLightFrameBuffer->GetTextureArrays()[0]);

			lightProjection = glm::ortho(-directionalViewPortSize, directionalViewPortSize, -directionalViewPortSize, directionalViewPortSize, nearPlane,100.0f);
			glm::vec3 dir = -directionalLights[i].direction;

			glm::vec3 directionalLightPos = camPos + dir * 25.0f;
			glm::vec3 directionalTargetPos = camPos + dir;
			lightView = glm::lookAt(directionalLightPos, directionalTargetPos, glm::vec3(0.0f, 1.0f, 0.0f));

			directionalLights[i].lightPositionMat = lightProjection * lightView;
			depthShader->SetMat4("lightSpaceMatrix", directionalLights[i].lightPositionMat);

			glClear(GL_DEPTH_BUFFER_BIT);
			DrawScene(depthShader);
		}
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		dirLightFrameBuffer->UnBind();
		lightFrameBuffer->Bind();
		//point shadows
		glCullFace(GL_FRONT);
		lightFrameBuffer->AttatchTextureArray(TextureType::CubeMap, (unsigned int)pointLights.size());
		for (unsigned int i = 0; i < pointLights.size(); i++) {
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, lightFrameBuffer->GetTextureArrays()[0]);

			lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
			glm::vec3 lightPos = pointLights[i].position;
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));  // +X
			shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0))); // -X
			shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));   // +Y
			shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1))); // -Y
			shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));  // +Z
			shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))); // -Z

			pointDepthShader->SetMat4Array("shadowMatrices", shadowTransforms);

			pointDepthShader->SetVec3("lightPos", pointLights[i].position);
			pointDepthShader->SetInt("lightIndex",i);
			glClear(GL_DEPTH_BUFFER_BIT);
			DrawScene(pointDepthShader);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

		//spotlight shadows
		lightFrameBuffer->AttatchTextureArray(TextureType::Texture_2D, (unsigned int)spotLights.size());
		for (unsigned int i = 0; i < spotLights.size(); i++) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, lightFrameBuffer->GetTextureArrays()[1]);

			lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
			lightView = glm::lookAt(spotLights[i].position, spotLights[i].direction+spotLights[i].position, glm::vec3(1.0f, 0.0f, 0.0f));

			spotLights[i].lightPositionMat = lightProjection * lightView;
			depthShader->SetMat4("lightSpaceMatrix", spotLights[i].lightPositionMat);

			glClear(GL_DEPTH_BUFFER_BIT);
			DrawScene(depthShader);
		}
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		lightFrameBuffer->UnBind();
		glCullFace(GL_BACK);
	}
	
	void OpenGLRenderApi::InitReflecitonsBuffer() {
		auto [wWidth, wHeight] = Application::GetApp()->GetWindow()->GetResolution();
		reflectionFrameBuffer.reset(FrameBuffer::Create(FrameBufferAttatchment::ColorAttatchment, wWidth, wHeight));
		reflectionFrameBuffer->AttatchTexture(TextureSlot::Texture8);
		reflectionFrameBuffer->AttatchRenderBuffer();
		reflectionFrameBuffer->UnBind();

		reflectionShader.reset(Shader::Create("./Assets/Shaders/BasicShader.vrt", "./Assets/Shaders/BasicShader.frag"));
	}
	void OpenGLRenderApi::EnableReflections() {
		useSSR = true;
		GenerateReflections();
	}
	void OpenGLRenderApi::GenerateReflections()	{
		if (!activeCamera) {
			return;
		}

		reflectionFrameBuffer->Bind();
		Clear();
		DrawSkyBox();
		DrawScene(reflectionShader);
		reflectionFrameBuffer->UnBind();
	}

	std::map<ColliderType, std::function<Mesh(ColliderComponent, TransformComponent)>> OpenGLRenderApi::colliderHandles = {
		{ColliderType::BoxCollider,OpenGLRenderApi::GetCubicCollider},
		{ColliderType::PlaneCollider,OpenGLRenderApi::GetCubicCollider},
		{ColliderType::CircleCollider,OpenGLRenderApi::GetSphereCollider},
		{ColliderType::CapsuleCollider,OpenGLRenderApi::GetCapsuleCollider},
	};


	void OpenGLRenderApi::DrawColliders() {
		if (!lineShader) {
			lineShader.reset(Shader::Create("./Assets/Shaders/LineShader.vrt","./Assets/Shaders/LineShader.frag"));
		}
		for (auto [collider,transform]:activeScene->GetRegister().GetAllOf<ColliderComponent,TransformComponent>()) {
			glm::mat4 colliderMat = glm::scale(transform.transform.GetTransformMat()*transform.transform.GetRotationMat(),collider.scale);

			glm::vec3 dColor;
			if (collider.isTrigger&&!collider.isSelectalbe) {
				dColor = glm::vec3(0.0f, 0.0f, 1.0f);
			}
			else if(collider.isSelectalbe) {
				dColor = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			else {
				dColor = glm::vec3(1.0f, 1.0f, 0.0f);
			}

			lineShader->Use();
			lineShader->SetVec3("debugColor", dColor);
			lineShader->SetMat4("viewProj", activeCamera->projectionMatrix * activeCamera->viewMatrix*colliderMat);
			auto meshFunc = colliderHandles[collider.colliderType];
			if (!meshFunc) {
				continue;
			}
			Mesh colliderMesh= meshFunc(collider,transform);
			colliderMesh.GetVertexArray()->Bind();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.5f);
			glDrawElements(GL_TRIANGLES, colliderMesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(0);
		}
	}
	void OpenGLRenderApi::HandleBones(std::shared_ptr<GameObject>gameObj, std::shared_ptr<Shader>shader)	{
		shader->Use();
		if (gameObj->CheckObjectForComponent<ArmatureComponent>()) {
			BtnArmature& armature = gameObj->GetComponent<ArmatureComponent>().armature;
			glm::mat4 transformMat = gameObj->GetComponent<TransformComponent>().transform.GetMatrix();

			std::vector<glm::mat4> boneMats;
			ComputeBoneMats(armature, boneMats);

			if (boneMats.empty()) {
				return;
			}
			shader->SetMat4Array("boneMats",boneMats);
		}
	}
	void OpenGLRenderApi::ComputeBoneMats(BtnSqd::BtnArmature& armature, std::vector<glm::mat4>& boneMats) {
		const std::vector<BtnSqd::AniBone>& bones = armature.GetBones();
		boneMats.resize(bones.size());

		for (size_t i = 0; i < bones.size(); ++i) {
			boneMats[i] = bones[i].finalTransform;
		}
	}
	Mesh OpenGLRenderApi::GetCubicCollider(ColliderComponent collider, TransformComponent transform) {
		glm::vec3 halfExtents = collider.scale;
		glm::mat4 modelMatrix = transform.transform.GetMatrix();

		std::vector<Vertices> verts = {
			{ glm::vec3(-1, -1, -1), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(1, -1, -1), glm::vec2(1.0f, 0.0f) },
			{ glm::vec3(1,  1, -1), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(-1,  1, -1), glm::vec2(0.0f, 1.0f) },
			{ glm::vec3(-1, -1,  1), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(1, -1,  1), glm::vec2(1.0f, 0.0f) },
			{ glm::vec3(1,  1,  1), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(-1,  1,  1), glm::vec2(0.0f, 1.0f) }

		};
		std::vector<unsigned int> indices = {
			0, 3, 1,  3, 2, 1,
			6, 4, 5, 7, 4, 6,
			3, 0, 7,  4, 7, 0,
			5, 1, 6,  2, 6, 1,
			5, 0, 1, 0, 5, 4,
			2, 3, 6,  7, 6, 3

		};
		return Mesh(verts,indices,Material());
	}
	Mesh OpenGLRenderApi::GetSphereCollider(ColliderComponent collider, TransformComponent transform) {
		std::vector<Vertices> verts;
		std::vector<unsigned int> indices;

		int stacks = 8;
		int slices = 16;
		float radius = collider.radius;


		for (int i = 0; i <= stacks; ++i) {
			float theta = i * glm::pi<float>() / stacks;
			float y = radius * cos(theta);
			float r = radius * sin(theta);

			for (int j = 0; j <= slices; ++j) {
				float phi = j * 2.0f * glm::pi<float>() / slices;
				float x = r * cos(phi);
				float z = r * sin(phi);
				verts.push_back({ glm::vec3(x, y, z),glm::vec2() });
			}
		}
		for (int i = 0; i < stacks; ++i) {
			for (int j = 0; j < slices; ++j) {
				int first = i * (slices + 1) + j;
				int second = first + slices + 1;

				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}
		return Mesh(verts, indices, Material());
	}
	Mesh OpenGLRenderApi::GetCapsuleCollider(ColliderComponent collider, TransformComponent transform) {
		int stacks = 8;
		int slices = 16;
		float radius = collider.radius;
		float height = collider.halfHeight*2.0f;
		std::vector<Vertices> verts;
		std::vector<unsigned int> indices;

		for (int i = 0; i <= stacks; i++) {
			float theta = glm::half_pi<float>() * i / stacks;
			float y = radius * cos(theta);
			float r = radius * sin(theta);

			for (int j = 0; j <= slices; j++) {
				float phi = 2.0f * glm::pi<float>() * j / slices;
				float x = r * cos(phi);
				float z = r * sin(phi);
				verts.push_back({glm::vec3(x,y+height-radius,z),glm::vec2()});
			}
		}

		for (int i = 0; i <= 1; ++i) {
			float y = (i == 0) ? -height + radius : height - radius;

			for (int j = 0; j <= slices; ++j) {
				float phi = 2.0f * glm::pi<float>() * j / slices;
				float x = radius * cos(phi);
				float z = radius * sin(phi);
				verts.push_back({ glm::vec3(x, y, z),glm::vec2() });
			}
		}

		for (int i = 0; i <= stacks; ++i) {
			float theta = glm::half_pi<float>() * i / stacks;
			float y = -radius * cos(theta);
			float r = radius * sin(theta);

			for (int j = 0; j <= slices; ++j) {
				float phi = 2.0f * glm::pi<float>() * j / slices;
				float x = r * cos(phi);
				float z = r * sin(phi);
				verts.push_back({ glm::vec3(x,y - height+ radius,z),glm::vec2() });
			}
		}
		int totalStacks = 2 * stacks*stacks;
		for (int stack = 0; stack < totalStacks; ++stack) {
			for (int slice = 0; slice < slices; ++slice) {
				int current = stack * slices + slice;
				int next = current + slices;

				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);

				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}



		return Mesh(verts,indices,Material());
	}
	void OpenGLRenderApi::DisableReflection() {
		useSSR = false;
	}
	void OpenGLRenderApi::ResizeScreen(int width, int height) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}
	void OpenGLRenderApi::ProcessLights() {
		directionalLights.clear();
		pointLights.clear();
		spotLights.clear();

		for (auto& lightWithTransform : activeScene->GetRegister().GetAllOf<LightComponent, TransformComponent>()) {
			auto& [light, lightTransform] = lightWithTransform;
			switch (light.lightType) {
			case LightType::DirectionalLight:
				DirectionalLightStruct dLight;
				dLight.color = light.lightColor;
				dLight.strength = light.strength;
				dLight.lightPositionMat = light.lightMat;
				dLight.direction = -lightTransform.transform.front;
				directionalLights.push_back(dLight);
				break;
			case LightType::PointLight:
				PointLightStruct pLight;
				pLight.color = light.lightColor;
				pLight.strength = light.strength;
				pLight.position = lightTransform.transform.GetTransformMat()[3];
				pointLights.push_back(pLight);

				break;
			case LightType::SpotLight:
				SpotLightStruct sLight;
				sLight.color = light.lightColor;
				sLight.strength = light.strength;
				sLight.outerCutOff = glm::radians(light.outerCutOff);
				sLight.cutOff = glm::radians(light.cutOff);
				sLight.position = lightTransform.transform.GetTransformMat()[3];
				sLight.direction = -lightTransform.transform.front;
				spotLights.push_back(sLight);
				break;
			}
		}
	}
}