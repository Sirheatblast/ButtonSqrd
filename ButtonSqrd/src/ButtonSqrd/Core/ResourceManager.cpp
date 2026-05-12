#include "ResourceManager.h"
#include"ButtonSqrd/Core/SuperGameObject.h"
#include"ButtonSqrd/Gui/BtnFontLoader.h"

namespace BtnSqd {
	bool ResourceManager::LoadSkyBoxes() {
		std::string skyboxDirectory = "./Assets/Textures/Skyboxes/";
		for (auto& p : std::filesystem::directory_iterator(skyboxDirectory)) {
			std::string directory = skyboxDirectory + p.path().filename().string();
			if (!loadedShaders.contains(directory)) {
				TextureSettings settings;
				settings.texType = TextureType::CubeMap;

				loadedSkyBoxes[directory] = std::shared_ptr<Texture>(Texture::Create(directory, settings));
			}
		}
		return true;
	}

	ResourceManager* ResourceManager::instance = new ResourceManager();
	bool ResourceManager::LoadSuperGameObjects() {
		std::string superDirectory = "./Assets/SuperGameObjects/";
		for (auto& p : std::filesystem::directory_iterator(superDirectory)) {
			std::string directory = superDirectory + p.path().filename().string();
			if (!loadedShaders.contains(directory)) {
				std::shared_ptr<SuperGameObject> super = SuperGameObject::LoadSuperGameObject(directory);
				loadedSupers.insert({directory,super});
			}
		}
		return true;
	}
	bool ResourceManager::LoadSounds() {
		std::string soundDirectory = "./Assets/Sounds/";
		for (auto& p : std::filesystem::directory_iterator(soundDirectory)) {
			std::string soundName = p.path().stem().string();
			if (!loadedSounds.contains(soundName)) {
				std::shared_ptr<SoundBuffer> newSound;
				std::string soundAdress = p.path().string();
				newSound.reset(new SoundBuffer(soundAdress));
				if (newSound) {
					loadedSounds.insert({ soundName, newSound });
				}
			}
		}
		return true;
	}
	bool ResourceManager::LoadParticleTextures() {
		std::string paritcleDirectory = "./Assets/Textures/Particles/";
		for (auto& p : std::filesystem::directory_iterator(paritcleDirectory)) {
			std::string directory = paritcleDirectory + p.path().filename().string();
			TextureSettings settings;
			loadedParticleTextures[directory] = std::shared_ptr<Texture>(Texture::Create(directory, settings));
		}
		return true;
	}

	bool ResourceManager::LoadFonts() {
		std::string fontDirectory = "./Assets/Fonts/";
		for (auto& p:std::filesystem::recursive_directory_iterator(fontDirectory)) {
			std::string extension = p.path().extension().string();
			if (extension == ".ttf") {
				std::string fontEntry = p.path().string();
				bool didLoad;
				BtnFont font(fontEntry,didLoad);
				if (didLoad) {
					loadedFonts[p.path().filename().stem().string()] = font;
				}
				else {
					BTNLOG_ERROR("Unable to load font: {}", p.path().filename().stem().string());
				}
			}
		}
		return false;
	}

	ResourceManager::ResourceManager(){
		if (instance != nullptr)
			return;
	}
	ResourceManager::~ResourceManager()	{
		delete instance;
	}
}
