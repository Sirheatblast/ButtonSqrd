#include "ResourceManager.h"
#include"ButtonSqrd/Core/SuperGameObject.h"

namespace BtnSqd {
	ResourceManager* ResourceManager::instance = new ResourceManager();
	bool ResourceManager::loadSuperGameObjects() {
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
	ResourceManager::ResourceManager(){
		if (instance != nullptr)
			return;
	}
	ResourceManager::~ResourceManager()	{
		delete instance;
	}
}
