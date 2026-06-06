#pragma once
#include<filesystem>
#include<imgui.h>
#include<ButtonSqrd.h>
#include<functional>

#include"../EditorEvents.h"
#include"AssetResourceTree.h"

namespace BtnSqd {
	enum class AssetDirType {
		Home = 0,
		Model,
		Texture,
		Shader,
		Sound,
		SuperGame,
		Font
	};

	using LoadedAssetCallback = std::function<void()>;

	class AssetDirectory {
	public:
		AssetDirectory();
		void OnUpdate();


	private:
		void GetCurrentFileDir();

		void CreateDragPayload(const std::string& path, std::string& name, std::string dirType);

		void AddNewThingsPopUp();
		void NewScriptCreateWindow();

		void GenerateARTree();

		void CreateLoadedCallbacks();

		void ShowRawFiles();
		void ShowLoadedFiles();

		void AddLoadedFolder(std::string tag, std::string title);

		void DisplayLoadedModels();
		void DisplayLoadedTexures();
		void DisplayLoadedSkyBoxTextures();
		void DisplayLoadedParticleTextures();
		void DisplayLoadedSounds();
		void DisplayLoadedScenes();
		void DisplayLoadedScripts();
		void DisplayLoadedShaders();
		void DisplayLoadedFonts();
		void DisplayLoadedSuperGameObjects();

		std::string GetAssetName(std::string path);

		std::string newScriptName ="New Script";
		bool showScriptCreate = false;
		bool showAssetsLoaded = true;

		float iconImageSize = 75.0f;
		std::filesystem::path currentDir;

		std::unique_ptr<Texture> folderTexture;
		std::unique_ptr<Texture> fileTexture;
		AssetResourceTree arTree;
		std::map<AssetCategory, LoadedAssetCallback> lAssetCallbacks;
	};
}

