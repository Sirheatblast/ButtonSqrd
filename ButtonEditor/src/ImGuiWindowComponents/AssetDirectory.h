#pragma once
#include<filesystem>
#include<imgui.h>
#include<ButtonSqrd.h>

#include"../EditorEvents.h"

namespace BtnSqd {
	class AssetDirectory {
	public:
		AssetDirectory();
		void OnUpdate();

	private:
		void GetCurrentFileDir();

		void AddNewThingsPopUp();
		void NewScriptCreateWindow();

		std::string newScriptName ="New Script";
		bool showScriptCreate = false;

		float iconImageSize = 75.0f;
		std::filesystem::path currentDir;

		std::unique_ptr<Texture> folderTexture;
		std::unique_ptr<Texture> fileTexture;
	};
}

