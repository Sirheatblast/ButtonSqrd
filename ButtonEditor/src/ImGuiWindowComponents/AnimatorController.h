#pragma once
#include<string>
#include<vector>
#include<imgui.h>
#include<imnodes.h>

#include"ButtonSqrd/Core/GameObject.h"
#include"ButtonSqrd/Core/ResourceManager.h"
#include"ButtonSqrd/Core/CoreComponents.h"
#include"ButtonSqrd/Render/BtnAnimation.h"

namespace BtnSqd {
	class AnimatorController {
	public:
		AnimatorController();
		~AnimatorController();
		void OnUpdate(GameObject& selectedObject);
    private:
		enum NodeType {
			AnimNode=0,
			TransitionNode,
		};

		struct Link {
			int id;
			int from;
			int to;
		};

		struct LinkData {
			int from=-1;
			int to=-1;
			int fromNodeId=-1;
			int toNodeId=-1;
		};
		struct Node {
			NodeType nType = NodeType::AnimNode;
			int id = -1;
			int inputId =-1;
			int outputId=-1;
			bool isDefault=false;
			std::string name;
			std::string state;
			AnimationPack* anim = nullptr;
			glm::vec2 pos=glm::vec2(0.0f);
			std::map<std::string,AnimationConnection*> connections;
		};

        void AnimConfig(const std::string& objName ,BtnSqd::GameObject& selectedObject);
		void DrawNode(Node node);
		void DrawTransitionNode(Node node);
		void DrawNodeEditor();
		void UpdateGridOffset();
		void BuildNodeTree(AnimatorComponent& animComp);
        void DrawAnimFlags(AnimatorComponent& animComp);
		void DrawNodeConfig(AnimatorComponent& animComp);
		void AddTransitionPopup(AnimatorComponent& anim);
		void NodeRightClickPopup(AnimatorComponent& anim);
		void CreateTransitionaryNode(const Node& fromNode, const Node& toNode,const std::string& state);

        bool addState = false;
        bool addStateP = false;

		std::string selectedState;
		int activeNodeId = -1;

		bool isPanning = false;
		glm::vec2 panStart;
		int idClock = 1;
		int nodeIdClock = 0;

		LinkData newLink = {};
		std::map<std::string, int>nameToId;
		std::vector<Link>links;
		std::vector<Node>nodes;
		int selObj = -1;
		glm::vec2 gridOffset = glm::vec2(0.0f, 0.0f);
		glm::vec2 gridOffsetStart = glm::vec2(0.0f, 0.0f);

		std::map<NodeType, std::function<void(Node)>> nodeHandles;
	};
}