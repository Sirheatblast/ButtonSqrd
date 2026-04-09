#include"OpenGLRendererApi.h"
#include"OpenGLTexture.h"
#include"ButtonSqrd/Core/Application.h"

#include<random>
namespace BtnSqd {
	void OpenGLRenderApi::Init() {
		glGenBuffers(1, &directionalUBO);
		glGenBuffers(1, &pointUBO);
		glGenBuffers(1, &spotUBO);
		glGenBuffers(1, &modelUBO);
		glGenBuffers(1, &materialUBO);
		glGenBuffers(1, &boneUBO);

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

		glBindBuffer(GL_UNIFORM_BUFFER, boneUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * MAX_BONES, nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		InitParticles();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
	void OpenGLRenderApi::DrawIndexed(std::vector<Mesh> meshes) {
		for (auto& mesh : meshes) {
			mesh.Draw();
			glDrawElements(GL_TRIANGLES, mesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}
	void OpenGLRenderApi::ShowColliders(bool show) {
		showColliders = show;
	}
	void OpenGLRenderApi::DisableShadows() {
		enableShadows = false;
	}
	std::tuple<CameraComponent*, TransformComponent*> OpenGLRenderApi::GetActiveCamera() {
		return std::make_tuple(activeCamera, cameraTransform);
	}
	void OpenGLRenderApi::SetClearColor(const glm::vec4& clearColor) {
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}
	void OpenGLRenderApi::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRenderApi::ClearDepth() {
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRenderApi::DrawIndexed(Mesh& mesh) {
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
	void OpenGLRenderApi::DrawIndexed(GameObject& gameObject) {
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
	void OpenGLRenderApi::SetCurrentCamera(std::tuple<CameraComponent*, TransformComponent*> camera) {
		auto& [camComp, transfComp] = camera;
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
	void OpenGLRenderApi::DrawScene() {
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
		bool hasSetUBO = (directionalLights.size() + pointLights.size() + spotLights.size() == loadedLights) ? true : false;

		glBindBuffer(GL_UNIFORM_BUFFER, directionalUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLightStruct) * directionalLights.size(), directionalLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, pointUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLightStruct) * pointLights.size(), pointLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, spotUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SpotLightStruct) * spotLights.size(), spotLights.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		for (auto& [gameId, gameModels, gameTransform] : activeScene->GetRegister().GetAllOfID<ModelComponent, TransformComponent>()) {
			Model model = gameModels.currentModel;
			std::shared_ptr<Shader> shader = model.GetShader();
			if (model.GetMeshes()->size() == 0) {
				continue;
			}
			bool hasBones = false;

			shader->Use();
			shader->SetBool("useSSR", useSSR);
			shader->SetBool("enableShadows", enableShadows);
			if (activeScene->GetgameObjects()[gameId]->CheckObjectForComponent<ArmatureComponent>() && !activeScene->GetgameObjects()[gameId]->GetComponent<ArmatureComponent>().armature.GetBones().empty()) {
				hasBones = true;
				HandleBones(activeScene->GetgameObjects()[gameId], shader);
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
			modelData.hasBones = hasBones;

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
				materialData.transparency = material->transparency;
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

		ProcessParticles();
		DrawParticles();

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

		unsigned int i = 0;
		for (auto& [gameObjId, gameModel, gameTransform] : activeScene->GetRegister().GetAllOfID<ModelComponent, TransformComponent>()) {
			Model model = gameModel.currentModel;
			shader->SetUnsignedInt("objectId", gameObjId);
			shader->SetUnsignedInt("drawId", i + 1);
			shader->SetInt("hasBones", activeScene->GetgameObjects()[gameObjId]->CheckObjectForComponent<ArmatureComponent>());
			HandleBones(activeScene->GetgameObjects()[gameObjId], shader);

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
				if (skyTex->GetPath() != skyboxPath) {
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
	void OpenGLRenderApi::InitSkybox() {
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

		depthShader.reset(Shader::Create("./Assets/Shaders/DepthShader.vrt", "./Assets/Shaders/DepthShader.frag"));
		pointDepthShader.reset(Shader::Create("./Assets/Shaders/PointDepthShader.vrt", "./Assets/Shaders/PointDepthShader.geo", "./Assets/Shaders/PointDepthShader.frag"));
	}
	void OpenGLRenderApi::EnableShadows() {
		enableShadows = true;
		dirLightFrameBuffer->ClearTextureArrays();
		lightFrameBuffer->ClearTextureArrays();

		GenerateShadows();
	}
	void OpenGLRenderApi::Update() {
		particQueue->Update();
	}
	void OpenGLRenderApi::GenerateShadows() {
		if (!activeCamera) {
			return;
		}
		glm::mat4 lightProjection, lightView;
		glm::vec3 camPos = cameraTransform->transform.GetPosition();
		glm::vec3 camFront = cameraTransform->transform.front;
		float directionalViewPortSize = 50.0f;

		dirLightFrameBuffer->Bind();
		//directional shadows
		dirLightFrameBuffer->AttatchTextureArray(TextureType::Texture_2D, (unsigned int)directionalLights.size());
		for (unsigned int i = 0; i < directionalLights.size(); i++) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, dirLightFrameBuffer->GetTextureArrays()[0]);

			lightProjection = glm::ortho(-directionalViewPortSize, directionalViewPortSize, -directionalViewPortSize, directionalViewPortSize, nearPlane, 100.0f);
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
			pointDepthShader->SetInt("lightIndex", i);
			glClear(GL_DEPTH_BUFFER_BIT);
			DrawScene(pointDepthShader);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);

		//spotlight shadows
		lightFrameBuffer->AttatchTextureArray(TextureType::Texture_2D, (unsigned int)spotLights.size());
		for (unsigned int i = 0; i < spotLights.size(); i++) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, lightFrameBuffer->GetTextureArrays()[1]);

			lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
			lightView = glm::lookAt(spotLights[i].position, spotLights[i].direction + spotLights[i].position, glm::vec3(1.0f, 0.0f, 0.0f));

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
	void OpenGLRenderApi::GenerateReflections() {
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
			lineShader.reset(Shader::Create("./Assets/Shaders/LineShader.vrt", "./Assets/Shaders/LineShader.frag"));
		}
		for (auto [collider, transform] : activeScene->GetRegister().GetAllOf<ColliderComponent, TransformComponent>()) {
			glm::mat4 colliderMat = glm::scale(transform.transform.GetTransformMat() * transform.transform.GetRotationMat(), collider.scale);

			glm::vec3 dColor;
			if (collider.isTrigger && !collider.isSelectalbe) {
				dColor = glm::vec3(0.0f, 0.0f, 1.0f);
			}
			else if (collider.isSelectalbe) {
				dColor = glm::vec3(0.0f, 1.0f, 0.0f);
			}
			else {
				dColor = glm::vec3(1.0f, 1.0f, 0.0f);
			}

			lineShader->Use();
			lineShader->SetVec3("debugColor", dColor);
			lineShader->SetMat4("viewProj", activeCamera->projectionMatrix * activeCamera->viewMatrix * colliderMat);
			auto meshFunc = colliderHandles[collider.colliderType];
			if (!meshFunc) {
				continue;
			}
			Mesh colliderMesh = meshFunc(collider, transform);
			colliderMesh.GetVertexArray()->Bind();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.5f);
			glDrawElements(GL_TRIANGLES, colliderMesh.GetVertexArray()->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(0);
		}
	}
	void OpenGLRenderApi::DrawBones() {

	}
	void OpenGLRenderApi::HandleBones(std::shared_ptr<GameObject>gameObj, std::shared_ptr<Shader>shader) {
		shader->Use();
		if (gameObj->CheckObjectForComponent<ArmatureComponent>()) {
			BtnArmature& armature = gameObj->GetComponent<ArmatureComponent>().armature;

			if (gameObj->CheckObjectForComponent<AnimatorComponent>()) {
				auto& aniComp = gameObj->GetComponent<AnimatorComponent>();
				if (aniComp.currentAnimation.isPlaying) {
					aniComp.currentTime += BtnTime::GetDeltaTime() * aniComp.currentAnimation.animation.GetTicksPerSecond() * aniComp.currentAnimation.playbackSpeed;
					if (aniComp.currentAnimation.isLooping) {
						if (aniComp.currentTime > aniComp.currentAnimation.animation.GetDuration()) {
							aniComp.currentTime = fmod(aniComp.currentTime, aniComp.currentAnimation.animation.GetDuration());
						}
					}
					else {
						if (aniComp.currentTime > aniComp.currentAnimation.animation.GetDuration()) {
							aniComp.currentTime = aniComp.currentAnimation.animation.GetDuration();
						}
					}
					UpdateAnimations(armature.GetRootNode(), aniComp, armature);
				}
			}

			std::vector<glm::mat4> boneMats;
			ComputeBoneMats(armature, boneMats);
			boneMats.resize(MAX_BONES);

			glBindBuffer(GL_UNIFORM_BUFFER, boneUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, MAX_BONES * sizeof(glm::mat4), boneMats.data());
			glBindBufferBase(GL_UNIFORM_BUFFER, 5, boneUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}
	void OpenGLRenderApi::ComputeBoneMats(BtnSqd::BtnArmature& armature, std::vector<glm::mat4>& boneMats) {
		const std::vector<AniBone>& bones = armature.GetBones();
		BtnArmature::UpdateBoneTransforms(armature.GetRootNode(), glm::mat4(1.0f), armature);
		boneMats.resize(bones.size());
		for (unsigned int i = 0; i < armature.GetBones().size(); i++) {
			boneMats[i] = bones[i].finalTransform;
		}
	}

	float getMaxKeyFrameTime(const BtnAnimation& animation) {
		float maxTime = 0.0f;

		for (const auto& [boneName, keyframes] : animation.GetBoneKeyFrames()) {
			for (const KeyFrame& kf : keyframes) {
				if (kf.time > maxTime) {
					maxTime = kf.time;
				}
			}
		}

		return maxTime;
	}

	int GetCurrentFrame(float animationTime, std::vector<KeyFrame>keyFrames) {
		for (int i = 0; i < keyFrames.size() - 1; i++) {
			if (animationTime < keyFrames[i + 1].time)
				return i;
		}
		return keyFrames.size() - 2;
	}

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}

	glm::vec3 InterpolatePosition(float currentTime, const std::vector<KeyFrame>& keyFrames) {
		if (keyFrames.size() == 1) {
			return keyFrames[0].position;
		}

		int currentIndex = GetCurrentFrame(currentTime, keyFrames);
		int nextIndex = currentIndex + 1;
		float scaleFactor = GetScaleFactor(keyFrames[currentIndex].time, keyFrames[nextIndex].time, currentTime);

		return glm::mix(keyFrames[currentIndex].position, keyFrames[nextIndex].position, scaleFactor);
	}


	glm::quat InterpolateRotation(float currentTime, std::vector<KeyFrame>keyFrames) {
		if (keyFrames.size() == 1) {
			return glm::mat4_cast(keyFrames[0].rotation);
		}

		int currentIndex = GetCurrentFrame(currentTime, keyFrames);
		int nextIndex = currentIndex + 1;
		float scaleFactor = GetScaleFactor(keyFrames[currentIndex].time, keyFrames[nextIndex].time, currentTime);

		glm::quat finalRotation = glm::slerp(keyFrames[currentIndex].rotation, keyFrames[nextIndex].rotation, scaleFactor);
		finalRotation = glm::normalize(finalRotation);
		return finalRotation;
	}

	glm::vec3 InterpolateScale(float currentTime, std::vector<KeyFrame>keyFrames) {
		if (keyFrames.size() == 1) {
			return keyFrames[0].scale;
		}

		int currentIndex = GetCurrentFrame(currentTime, keyFrames);
		int nextIndex = currentIndex + 1;
		float scaleFactor = GetScaleFactor(keyFrames[currentIndex].time, keyFrames[nextIndex].time, currentTime);

		glm::vec3 finalScale = glm::mix(keyFrames[currentIndex].scale, keyFrames[nextIndex].scale, scaleFactor);
		return finalScale;
	}

	void OpenGLRenderApi::UpdateAnimations(ArmNode& node, AnimatorComponent& aniComp, BtnArmature& armature) {
		if (!aniComp.blendAnim || aniComp.currentAnimation == aniComp.nextAnimation) {
			auto keyFrameMap = aniComp.currentAnimation.animation.GetBoneKeyFrames();
			auto& bones = armature.GetBones();

			for (auto& bone : bones) {
				auto it = keyFrameMap.find(bone.name);
				if (it == keyFrameMap.end() || it->second.empty()) continue;

				const auto& keyFrames = it->second;

				glm::vec3 pos = InterpolatePosition(aniComp.currentTime, keyFrames);
				glm::quat rot = InterpolateRotation(aniComp.currentTime, keyFrames);
				glm::vec3 scale = InterpolateScale(aniComp.currentTime, keyFrames);

				glm::mat4 tMat = glm::translate(glm::mat4(1.0f), pos);
				glm::mat4 rMat = glm::mat4_cast(rot);
				glm::mat4 sMat = glm::scale(glm::mat4(1.0f), scale);

				bone.localTrasform = tMat * rMat * sMat;
			}
			return;
		}
		BlendAnimations(aniComp, armature);
	}
	void OpenGLRenderApi::BlendAnimations(BtnSqd::AnimatorComponent& aniComp, BtnSqd::BtnArmature& armature) {
		auto keyFrameMapA = aniComp.currentAnimation.animation.GetBoneKeyFrames();
		auto keyFrameMapB = aniComp.nextAnimation.animation.GetBoneKeyFrames();
		auto& bones = armature.GetBones();

		aniComp.blendTime += BtnTime::GetDeltaTime() * aniComp.maxBlendTicks;

		float blendFactor = 1.0f;
		if (aniComp.maxBlendTicks > 0.0f) {
			blendFactor = glm::clamp(aniComp.blendTime / aniComp.maxBlendTicks, 0.0f, 1.0f);
		}

		for (auto& bone : bones) {
			glm::vec3 posA(0.0f), posB(0.0f);
			glm::vec3 scaleA(1.0f), scaleB(1.0f);
			glm::quat rotA(1.0f, 0.0f, 0.0f, 0.0f), rotB(1.0f, 0.0f, 0.0f, 0.0f);

			auto itA = keyFrameMapA.find(bone.name);
			if (itA != keyFrameMapA.end() && !itA->second.empty()) {
				const auto& kfA = itA->second;
				posA = InterpolatePosition(aniComp.blendTime, kfA);
				rotA = InterpolateRotation(aniComp.blendTime, kfA);
				scaleA = InterpolateScale(aniComp.blendTime, kfA);
			}

			auto itB = keyFrameMapB.find(bone.name);
			if (itB != keyFrameMapB.end() && !itB->second.empty()) {
				const auto& kfB = itB->second;

				posB = InterpolatePosition(aniComp.blendTime, kfB);
				rotB = InterpolateRotation(aniComp.blendTime, kfB);
				scaleB = InterpolateScale(aniComp.blendTime, kfB);
			}

			glm::vec3 pos = glm::mix(posA, posB, blendFactor);
			glm::quat rot = glm::normalize(glm::slerp(rotA, rotB, blendFactor));
			glm::vec3 scale = glm::mix(scaleA, scaleB, blendFactor);

			glm::mat4 tMat = glm::translate(glm::mat4(1.0f), pos);
			glm::mat4 rMat = glm::mat4_cast(rot);
			glm::mat4 sMat = glm::scale(glm::mat4(1.0f), scale);

			bone.localTrasform = tMat * rMat * sMat;
		}
		if (blendFactor >= 1.0f) {
			aniComp.currentAnimation.isPlaying = false;
			aniComp.currentAnimation = aniComp.nextAnimation;
			aniComp.currentAnimation.isPlaying = true;
		}
	}

	inline float RandomFloat(float min, float max) {
		static thread_local std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);
	}

	inline int RandomInt(int min, int max) {
		static thread_local std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> dist(min, max);
		return dist(rng);
	}

	glm::vec3 CalcInitVel(const glm::vec3& initVel, float min, float max) {
		glm::vec3 baseVel = initVel;
		glm::vec3 dir = glm::normalize(baseVel);

		float yaw = RandomFloat(min, max);
		float pitch = RandomFloat(min, max);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		if (glm::abs(glm::dot(dir, up)) > 0.99f) {
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		}

		glm::vec3 right = glm::normalize(glm::cross(dir, up));
		up = glm::normalize(glm::cross(right, dir));
		glm::vec3 rotatedDir = glm::normalize(dir + yaw * right + pitch * up);

		glm::vec3 finalVel = rotatedDir * glm::length(baseVel);
		return finalVel;
	}

	std::vector<ParticleData> OpenGLRenderApi::OnEmitParticle(ParticleEmitterComponent& Emitter, BtnTransform& transform) {
		if (Emitter.particles.empty())
			return std::vector<ParticleData>();
		std::vector<ParticleData> p;
		p.resize(static_cast<size_t>(Emitter.maxParticles));

		unsigned int pIdx = 0;
		switch (Emitter.eMode) {
		case ParticleEmitionMode::Linear:
			ParticleEmitLinear(Emitter, p, pIdx, transform);
			break;

		case ParticleEmitionMode::Random:
			ParticleEmitRandom(Emitter, p, pIdx, transform);
			break;

		case ParticleEmitionMode::Selected:
			ParticleEmitSelected(Emitter, p, transform);
			break;
		}

		return p;
	}

	void OpenGLRenderApi::ParticleEmitSelected(BtnSqd::ParticleEmitterComponent& Emitter, std::vector<BtnSqd::ParticleData>& p, BtnSqd::BtnTransform& transform) {
		for (int i = 0; i < Emitter.maxParticles; i++) {
			CreateParticle(p, i, Emitter, Emitter.selectedParticleID, transform);
		}
	}

	void OpenGLRenderApi::ParticleEmitRandom(BtnSqd::ParticleEmitterComponent& Emitter, std::vector<BtnSqd::ParticleData>& p, unsigned int& pIdx, BtnSqd::BtnTransform& transform) {
		for (int i = 0; i < Emitter.maxParticles; i++) {
			CreateParticle(p, i, Emitter, pIdx, transform);
			pIdx = RandomInt(0, Emitter.particles.size() - 1);
		}
	}

	void OpenGLRenderApi::ParticleEmitLinear(BtnSqd::ParticleEmitterComponent& Emitter, std::vector<BtnSqd::ParticleData>& p, unsigned int& pIdx, BtnSqd::BtnTransform& transform) {
		for (int i = 0; i < Emitter.maxParticles; i++) {
			CreateParticle(p, i, Emitter, pIdx, transform);

			pIdx++;
			if (pIdx >= Emitter.particles.size()) {
				pIdx = 0;
			}
		}
	}
	float EvaluateCurveLinear(const std::vector<glm::vec2>& curve, float t, int resolution) {
		if (curve.size() < 2)
			return curve.empty() ? 0.0f : curve.front().y;
		t = glm::clamp(t, 0.0f, 1.0f);

		const size_t segmentCount = curve.size() - 1;
		float scaledT = t * segmentCount;
		size_t seg = (size_t)scaledT;

		if (seg >= segmentCount)
			return curve.back().y;

		float localT = scaledT - seg;

		const glm::vec2 a = curve[seg];
		const glm::vec2 b = curve[seg + 1];

		return a.y + localT * (b.y - a.y);
	}

	float Normalize(float v, float minV, float maxV) {
		if (std::abs(maxV - minV) < 1e-6f) return 0.0f;

		if (minV >= 0) {
			float result = (v - minV) / (maxV - minV);
			return std::clamp(result, 0.0f, 1.0f);
		}
		else {
			float result = 2.0f * ((v - minV) / (maxV - minV)) - 1.0f;
			return std::clamp(result, -1.0f, 1.0f);
		}
	}

	std::vector<float> BakeCurveLUT(const std::vector<glm::vec2>& curve, int resolution, float min = 0.0f, float max = 100.0f) {
		std::vector<float> lut(resolution);

		for (int i = 0; i < resolution; i++) {
			float t = i / float(resolution - 1);
			lut[i] = Normalize(EvaluateCurveLinear(curve, t, resolution), min, max);
		}

		return lut;
	}

	//Remove this function
	unsigned int OpenGLRenderApi::CreateCurveTexture(const std::vector<glm::vec2>& curve) {
		unsigned int lutResolution = 256;
		if (curve.empty())
			return 0;

		std::vector<float> lut = BakeCurveLUT(curve, lutResolution);

		std::shared_ptr<Texture> lineTex;
		TextureSettings texSett;
		texSett.texType = TextureType::Texture_1D;
		texSett.wrappingMode = TextureWrappingInfo::Clamp;
		texSett.texDataType = TextureDataType::Color;

		lineTex.reset(Texture::Create(256, 0, texSett, lut.data()));
		return lineTex->GetId();
	}

	void OpenGLRenderApi::CreateParticle(std::vector<BtnSqd::ParticleData>& p, int i, BtnSqd::ParticleEmitterComponent& Emitter, unsigned int& pIdx, BtnSqd::BtnTransform& transform) {
		if (Emitter.particles[pIdx].genCurveTextures) {
			Emitter.particles[pIdx].genCurveTextures = false;
			RemakeParticleCurveArray();
		}

		if (Emitter.maxLifeTime < Emitter.particles[pIdx].lifetime)
			Emitter.maxLifeTime = Emitter.particles[pIdx].lifetime;

		p[i].color = Emitter.particles[pIdx].clearColor;
		p[i].tcolor = Emitter.particles[pIdx].transitionColor;
		p[i].position = glm::vec4(transform.GetPosition(), 1.0f);
		if (Emitter.particles[pIdx].useEmitterDirection) {
			const auto quat = transform.GetRotationQ();
			p[i].orientationQuat = glm::vec4(quat.x, quat.y, quat.z, quat.w);
		}
		p[i].velocity = (glm::length(Emitter.particles[pIdx].initialVelocity) > 0.0f) ? glm::vec4(CalcInitVel(Emitter.particles[pIdx].initialVelocity, Emitter.particles[pIdx].minSpread, Emitter.particles[pIdx].maxSpread), 0.0f) : glm::vec4(0.0f);

		float spawnDelay = RandomFloat(0.0f, Emitter.particles[pIdx].spawnDelay);
		p[i].pData = glm::vec4(Emitter.particles[pIdx].lifetime, Emitter.particles[pIdx].lifetime, Emitter.particles[pIdx].size, spawnDelay);

		unsigned int texId = 0;
		if (particleTextureKeys.contains(Emitter.particles[pIdx].texKey)) {
			texId = particleTextureKeys[Emitter.particles[pIdx].texKey];
		}

		uint32_t flags = 0;

		if (Emitter.particles[pIdx].useFadeCurve)   flags |= CURVE_FADE;
		if (Emitter.particles[pIdx].useSizeCurve)   flags |= CURVE_SIZE;
		if (Emitter.particles[pIdx].useColorCurve)  flags |= CURVE_COLOR;
		if (Emitter.particles[pIdx].useVelCurve)	   flags |= CURVE_VEL;
		if (Emitter.particles[pIdx].fade)		   flags |= PARTICLE_FADE;
		if (Emitter.particles[pIdx].enableColorT)   flags |= PARTICLE_TCOL;
		if (Emitter.particles[pIdx].useEmitterDirection) flags |= USE_ORIENTATION;

		unsigned int curveLayer = pIdx + Emitter.pEmitterOffset;
		p[i].pIData = glm::uvec4(Emitter.particles[pIdx].useTexture, texId, curveLayer, flags);
	}

	void OpenGLRenderApi::ProcessParticles() {
		if (ResourceManager::GetLoadedParticleTextures().size() != particleTextureKeys.size()) {
			particleTextureCount = particleTextureKeys.size();
			RemakeParticleTextureArray();
		}

		for (auto& [particleEmtrRef, transformRef] : activeScene->GetRegister().GetAllOfRef<ParticleEmitterComponent, TransformComponent>()) {
			auto& particleEmtr = particleEmtrRef.get();
			auto& transform = transformRef.get();
			if (particleEmtr.isEmiting && particleEmtr.isRepeating) {
				if (particleEmtr.repeatTimer <= 0) {
					particData = OnEmitParticle(particleEmtr, transform.transform);
					unsigned int pOffset = 0;
					particQueue->DispatchBlock(particleEmtr.maxParticles, particleEmtr.maxLifeTime, pOffset);
					ssbo->UpdateData(particData.data(), sizeof(ParticleData) * particData.size(), pOffset * sizeof(ParticleData));
					particleEmtr.repeatTimer = particleEmtr.repeatMaxTime;
				}
				else {
					particleEmtr.repeatTimer -= BtnTime::GetDeltaTime();
				}
			}
			else if (particleEmtr.isEmiting) {
				particData = OnEmitParticle(particleEmtr, transform.transform);
				unsigned int pOffset = 0;
				particQueue->DispatchBlock(particleEmtr.maxParticles, particleEmtr.maxLifeTime, pOffset);
				ssbo->UpdateData(particData.data(), sizeof(ParticleData) * particData.size(), pOffset * sizeof(ParticleData));
				particleEmtr.isEmiting = false;
			}
		}
		if (!particData.empty()) {
			ssbo->Bind();
			particleComputeShader->Use();
			particleComputeShader->SetFloat("deltaTime", BtnTime::GetDeltaTime());

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D_ARRAY, particleCurveTexArray->GetId());
			particleComputeShader->SetUniform("uCurveArray", 2);

			uint32_t groups = (MAX_PARTICLES + 255) / 256;
			glDispatchCompute(groups, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
		}
	}

	void OpenGLRenderApi::DrawParticles() {
		if (particData.empty()) {
			return;
		}
		glDepthMask(GL_FALSE);
		particleShader->Use();
		particleShader->SetMat4("VP", activeCamera->projectionMatrix * activeCamera->viewMatrix);
		ssbo->Bind();

		if (!particleTextureFrameBuffer->GetTextureArrays().empty()) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, particleTextureFrameBuffer->GetTextureArrays()[0]);
			particleShader->SetUniform("uParticleTextures", 0);
		}

		glBindVertexArray(pVao);
		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
		glBindVertexArray(0);

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
		particleShader->Detatch();
		glDepthMask(GL_TRUE);

	}

	void OpenGLRenderApi::InitParticles() {
		ssbo.reset(ShaderStorageBuffer::Create(0));
		ssbo->AttachData(nullptr, sizeof(ParticleData) * MAX_PARTICLES);

		particleShader.reset(Shader::Create("./Assets/Shaders/ParticleShader.vrt", "./Assets/Shaders/ParticleShader.frag"));
		particleComputeShader.reset(Shader::Create("./Assets/Shaders/ParticleShader.cmpt"));
		copyTexShader.reset(Shader::Create("./Assets/Shaders/TexCopy.vrt", "./Assets/Shaders/TexCopy.frag"));

		CreateParticleTextureVAO();
		RemakeParticleTextureArray();

		glGenVertexArrays(1, &pVao);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SPRITE);
	}

	void OpenGLRenderApi::CreateParticleTextureVAO() {
		float quadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f
		};

		GLuint quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);

		glBindVertexArray(quadVAO);

		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindVertexArray(0);
	}

	void OpenGLRenderApi::RemakeParticleTextureArray() {
		unsigned int width = 128;
		unsigned int height = 128;
		unsigned int numLayers = (unsigned int)ResourceManager::GetLoadedParticleTextures().size();
		if (numLayers == 0)
			return;


		particleTextureFrameBuffer.reset(FrameBuffer::Create(FrameBufferAttatchment::ColorAttatchment, width, height));
		particleTextureFrameBuffer->Bind();
		particleTextureFrameBuffer->AttatchTextureArray(TextureType::Texture_2D, numLayers);

		GLuint texArrayID = particleTextureFrameBuffer->GetTextureArrays()[0];

		glBindVertexArray(quadVAO);
		glViewport(0, 0, width, height);

		int layer = 0;

		particleTextureKeys.clear();
		for (const auto& [texKey, texture] : ResourceManager::GetLoadedParticleTextures()) {
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texArrayID, 0, layer);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			copyTexShader->Use();

			glActiveTexture(GL_TEXTURE0);
			texture->Bind();
			copyTexShader->SetUniform("uSourceTex", 0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			particleTextureKeys.insert({ texKey,layer });
			layer++;
		}

		particleTextureFrameBuffer->UnBind();
	}

	void OpenGLRenderApi::RemakeParticleCurveArray() {
		auto Emitters = activeScene->GetRegister().GetAllOfRef<ParticleEmitterComponent>();
		unsigned int numLayers = 0;
		for (const auto& [Emitter] : Emitters) {
			numLayers += Emitter.get().particles.size();
		}

		if (numLayers == 0) {
			return;
		}

		const unsigned int lutRes = 256;
		const unsigned int numAttributes = 6;

		TextureSettings texSett;
		texSett.texDataType = TextureDataType::SingleColor;
		texSett.maxLayers = numLayers;
		texSett.texFormat = TexFormat::SINGLE;
		texSett.texType = TextureType::Texture_2D_Array;
		texSett.wrappingMode = TextureWrappingInfo::Clamp;

		particleCurveTexArray.reset(Texture::Create(lutRes, numAttributes, texSett));
		particleCurveTexArray->Bind();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned int pIdx = 0;
		unsigned int eOffset = 0;
		for (auto& [Emitter] : Emitters) {
			for (int i = 0; i < Emitter.get().particles.size(); i++) {
				const BtnParticle& partic = Emitter.get().particles[i];

				std::vector<float> layerData;
				layerData.reserve(lutRes * numAttributes);

				auto fade = BakeCurveLUT(partic.fadeCurvePoints, lutRes);
				auto size = BakeCurveLUT(partic.sizeCurvePoints, lutRes);
				auto color = BakeCurveLUT(partic.colorCurvePoints, lutRes);
				auto velX = BakeCurveLUT(partic.velCurvePoints[0], lutRes, -100.0f);
				auto velY = BakeCurveLUT(partic.velCurvePoints[1], lutRes, -100.0f);
				auto velZ = BakeCurveLUT(partic.velCurvePoints[2], lutRes, -100.0f);

				layerData.insert(layerData.end(), fade.begin(), fade.end());
				layerData.insert(layerData.end(), size.begin(), size.end());
				layerData.insert(layerData.end(), color.begin(), color.end());
				layerData.insert(layerData.end(), velX.begin(), velX.end());
				layerData.insert(layerData.end(), velY.begin(), velY.end());
				layerData.insert(layerData.end(), velZ.begin(), velZ.end());

				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, pIdx, lutRes, numAttributes, 1, GL_RED, GL_FLOAT, layerData.data());
				pIdx++;
			}
			Emitter.get().pEmitterOffset = eOffset;
			eOffset += pIdx;
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
		return Mesh(verts, indices, Material());
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
		float height = collider.halfHeight * 2.0f;
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
				verts.push_back({ glm::vec3(x,y + height - radius,z),glm::vec2() });
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
				verts.push_back({ glm::vec3(x,y - height + radius,z),glm::vec2() });
			}
		}
		int totalStacks = 2 * stacks * stacks;
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



		return Mesh(verts, indices, Material());
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