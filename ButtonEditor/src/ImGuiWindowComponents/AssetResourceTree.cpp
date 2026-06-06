#include "AssetResourceTree.h"

namespace BtnSqd {
	AssetResourceTree::AssetResourceTree() {
		AssetLevel home;
		home.id = 0;
		home.name = "Home";
		directory.push_back(home);
	}
	void AssetResourceTree::AddChild(std::string childName, AssetCategory childCategory) {
		AssetLevel nLevel;
		nLevel.name = childName;
		nLevel.parentId = level;
		nLevel.id = directory.size();
		nLevel.category = childCategory;

		directory[level].childrenData.insert({nLevel.name,nLevel.id});
		directory.push_back(nLevel);
	}
	void AssetResourceTree::MoveUp() {
		if (directory[level].parentId >= 0) {
			level = directory[level].parentId;
		}
	}
	void AssetResourceTree::MoveDown(std::string childName) {
		if (directory[level].childrenData.contains(childName)) {
			level = directory[level].childrenData[childName];
		}
	}
}