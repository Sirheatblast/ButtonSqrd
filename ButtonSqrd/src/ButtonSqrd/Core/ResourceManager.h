#pragma once
#include"ButtonSqrd/Render/Texture.h"
#include"ButtonSqrd/Render/Model.h"
#include"ButtonSqrd/Render/BtnArmature.h"
#include"ButtonSqrd/Render/BtnAnimation.h"

#include"ButtonSqrd/Audio/SoundBuffer.h"

#include<map>
#include<memory>
#include<filesystem>

namespace BtnSqd {
	class SuperGameObject;
	class ResourceManager {
	public:
		static void AddTexture(std::shared_ptr<Texture> newTexture){
			instance->loadedTextures.insert({newTexture->GetPath(),newTexture});
		}

		static void AddParticleTexture(std::shared_ptr<Texture> newTexture) {
			instance->loadedParticleTextures.insert({ newTexture->GetPath(),newTexture });
		}

		static void AddSkybox(std::shared_ptr<Texture> newSkybox) {
			instance->loadedSkyBoxes.insert({ newSkybox->GetPath(),newSkybox });
		}

		static void AddModel(std::string name,Model newModel){
			instance->loadedModels.insert({ name,newModel });
		}

		static void AddSuperGameObject(std::string name,std::shared_ptr<SuperGameObject> newSuper) {
			instance->loadedSupers.insert({name,newSuper});
		}

		static void AddArmature(std::string name, BtnArmature newArmature) {
			instance->loadedArmatures.insert({name,newArmature});
		}

		static void AddAnimation(std::string name, BtnAnimation newAnimation) {
			instance->loadedAnimations.insert({ name,newAnimation });
		}
		static void AddSound(std::string name, std::shared_ptr<SoundBuffer> newSound) {
			instance->loadedSounds.insert({ name,newSound });
		}

		static bool LoadAllShaders() { return instance->LoadShaders(); }
		static bool LoadAllModels() { return instance->LoadModels(); }
		static bool LoadAllSkyboxes() { return instance->LoadSkyBoxes(); }
		static bool LoadAllSuperGameObjects() { return instance->LoadSuperGameObjects(); }
		static bool LoadAllSounds() { return instance->LoadSounds(); }
		static bool LoadAllParticleTextures() { return instance->LoadParticleTextures(); }

		static std::map<std::string, std::shared_ptr<Texture>>& GetLoadedTextures() { return instance->loadedTextures; }
		static std::map<std::string, std::shared_ptr<Texture>>& GetLoadedParticleTextures() { return instance->loadedParticleTextures; }
		static std::map<std::string, std::shared_ptr<Texture>>& GetLoadedSkyboxes() { return instance->loadedSkyBoxes; }
		static std::map<std::string, std::shared_ptr<Shader>>& GetLoadedShaders() { return instance->loadedShaders; }
		static std::map<std::string, Model> GetLoadedModels() { return instance->loadedModels; }
		static std::map<std::string, BtnAnimation> GetLoadedAnimations() { return instance->loadedAnimations; }
		static std::map<std::string, BtnArmature>& GetLoadedArmatures() { return instance->loadedArmatures; }
		static std::map < std::string, std::shared_ptr<SuperGameObject>>& GetLoadedSuperGameObjects() { return instance->loadedSupers; }
		static std::map < std::string, std::shared_ptr<SoundBuffer>>& GetLoadedSounds() { return instance->loadedSounds; }

	private:
		bool LoadSkyBoxes();

		//This function goes after Load Shaders
		bool LoadModels() {
			std::string modelDirectory = "./Assets/Models/";
			for (auto& p : std::filesystem::directory_iterator(modelDirectory)) {
				std::string directory =modelDirectory+ p.path().filename().string();
				if (!loadedShaders.contains(directory)) {
					Model newModel(directory,loadedShaders["PbrShader"].get());
					loadedModels.insert({ directory,newModel });
				}
			}
			return true;
		}

		bool LoadShaders() {
			std::string shaderDirectory = "./Assets/Shaders/";
			for (auto& p : std::filesystem::directory_iterator(shaderDirectory)) {
				std::string shaderName = p.path().stem().string();
				if (!loadedShaders.contains(shaderName)) {
					std::shared_ptr<Shader> newShader;
					std::string shaderAdress = shaderDirectory + shaderName;
					if (std::filesystem::directory_entry(shaderAdress + ".geo").exists()) {
						newShader.reset(Shader::Create(shaderAdress + ".vrt", shaderAdress + ".geo", shaderAdress + ".frag"));
					}
					else if (std::filesystem::directory_entry(shaderAdress + ".cmpt").exists()) {
						newShader.reset(Shader::Create(shaderAdress + ".cmpt"));
					}
					else {
						newShader.reset(Shader::Create(shaderAdress + ".vrt", shaderAdress + ".frag"));
					}
					if (newShader) {
						newShader->SetShaderName(shaderName);
						loadedShaders.insert({ shaderName, newShader });
					}
				}
			}
			return true;
		}

		bool LoadSuperGameObjects();
		bool LoadSounds();
		bool LoadParticleTextures();

		ResourceManager();
		~ResourceManager();
		static ResourceManager* instance;

		std::map<std::string,std::shared_ptr<Texture>> loadedTextures;
		std::map<std::string, std::shared_ptr<Texture>> loadedParticleTextures;
		std::map<std::string,std::shared_ptr<Texture>> loadedSkyBoxes;
		std::map<std::string,Model>loadedModels;
		std::map<std::string, BtnArmature>loadedArmatures;
		std::map<std::string, std::shared_ptr<Shader>>loadedShaders;
		std::map <std::string, std::shared_ptr<SuperGameObject>>loadedSupers;
		std::map<std::string, BtnAnimation>loadedAnimations;
		std::map<std::string, std::shared_ptr<SoundBuffer>> loadedSounds;
	};
}

