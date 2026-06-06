#pragma once

#include<string>
#include<vector>
#include<map>

namespace BtnSqd {
	enum AssetCategory {
		None=0,
		ModelCat,
		TextureCat,
		ShaderCat,
		ScriptCat,
		SuperGameObjectCat,
		FontCat,
		SoundCat,
		SceneCat,

		ParticleTexCat,
		SkyBoxTexCat,
	};
	struct AssetLevel {
		std::string name = "NULL";
		std::map<std::string,int> childrenData;
		int id=-1;
		int parentId = -1;
		AssetCategory category = None;
	};

	class AssetResourceTree {
	public:
		AssetResourceTree();

		AssetLevel GetLevel()const { return directory[level]; }
		AssetLevel GetLevel(int selected) { return directory[selected]; }

		void AddChild(std::string childName,AssetCategory childCategory);
		void MoveUp();
		void MoveDown(std::string childName);
	private:
		int level=0;
		std::vector<AssetLevel>directory;
	};
}