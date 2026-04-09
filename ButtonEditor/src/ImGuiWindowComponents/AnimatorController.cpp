#include"AnimatorController.h"
#include<algorithm>
#include<unordered_set>
#include<unordered_map>

namespace BtnSqd {
    AnimatorController::AnimatorController() {
        nodeHandles[NodeType::AnimNode] = [this](Node node) { this->DrawNode(node); };
        nodeHandles[NodeType::TransitionNode] = [this](Node node) { this->DrawTransitionNode(node); };
        ImNodes::CreateContext();
    }
    AnimatorController::~AnimatorController() {
        ImNodes::DestroyContext();
    }
    void AnimatorController::OnUpdate(GameObject& selectedObject)	{
		ImGui::Begin("Animator Controller",nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

		ImGui::BeginChild("##AnimatorOptions",ImVec2(250.0f,ImGui::GetWindowSize().y));
        ImGui::BeginGroup();

        std::string objName;
        if (selectedObject.IsValid()) {
            objName = selectedObject.GetComponent<TagComponenet>().tag;
        }
        else {
            objName = "Select GameObject";
        }
        if (ImGui::Button("Reset grid pos")) {
            gridOffset = glm::vec2(0.0f);
        }

        if (selectedObject.IsValid() && selectedObject.CheckObjectForComponent<AnimatorComponent>()) {
            auto& aniComp = selectedObject.GetComponent<AnimatorComponent>();
            if (selectedObject.GetId()!=selObj) {
                selObj = selectedObject.GetId();
                activeNodeId = -1;
                BuildNodeTree(aniComp);
            }
            AnimConfig(objName, selectedObject);
            DrawNodeConfig(aniComp);
            DrawAnimFlags(aniComp);
        }
        else {
            if (!nodes.empty()) {
                nodes.clear();
                links.clear();
                selObj = -1;
            }
            AnimConfig(objName, selectedObject);
        }
        ImGui::EndGroup();
        ImGui::EndChild();
        ImGui::SameLine();

        DrawNodeEditor();
        if (selectedObject.IsValid() && selectedObject.CheckObjectForComponent<AnimatorComponent>()) {
            AddTransitionPopup(selectedObject.GetComponent<AnimatorComponent>());
            NodeRightClickPopup(selectedObject.GetComponent<AnimatorComponent>());
        }
        ImGui::End();
    }

    void AnimatorController::AnimConfig(const std::string& objName, BtnSqd::GameObject& selectedObject) {
        ImGui::Text(objName.c_str());
        if (selectedObject.IsValid() && selectedObject.CheckObjectForComponent<AnimatorComponent>()) {
            AnimatorComponent& animComp = selectedObject.GetComponent<AnimatorComponent>();
            if (ImGui::BeginCombo("##AddAnimationToObject", "Press to add animation")) {
                for (auto [animName, anim] : ResourceManager::GetLoadedAnimations()) {
                    std::string animLable = animName + "##ANIMLABLE";
                    if (ImGui::Selectable(animLable.c_str())) {
                        animComp.AddAnimation(animName);
                        BuildNodeTree(animComp);
                    }
                }
                ImGui::EndCombo();
            }
        }
        else {
            ImGui::Text("Please add Animator Component\nto Continue");
        }
    }

    void AnimatorController::DrawNode(Node node) {

        if (node.isDefault) {
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(34, 139, 34, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(44, 159, 44, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(24, 119, 24, 255));
        }
        else {
            ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(70, 130, 180, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(90, 150, 200, 255));
            ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(50, 110, 160, 255));
        }
        if (node.anim->isLooping) {
            ImNodes::PushColorStyle(ImNodesCol_NodeOutline, IM_COL32(255, 255, 0, 255));
        }
        else {
            ImNodes::PushColorStyle(ImNodesCol_NodeOutline, IM_COL32(100, 100, 100, 255));
        }

        ImNodes::BeginNode(node.id);

        ImNodes::BeginNodeTitleBar();
        ImGui::Text(node.name.c_str());
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(node.inputId);
        ImGui::Text("Input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(node.outputId);
        ImGui::Text("Output");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        if (Input::IsMouseButtonPressed(MouseCode::Right)&&ImNodes::IsEditorHovered())
            ImNodes::SetNodeGridSpacePos(node.id, ImVec2(node.pos.x + gridOffset.x, node.pos.y + gridOffset.y));
    }

    void AnimatorController::DrawTransitionNode(Node node) {
        ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(139, 0, 0, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(165, 42, 42, 255));
        ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(120, 0, 0, 255));

        ImNodes::BeginNode(node.id);

        ImNodes::BeginNodeTitleBar();
        ImGui::Text("Transition Node");
        ImNodes::EndNodeTitleBar();

        ImNodes::BeginInputAttribute(node.inputId);
        ImGui::Text("Input");
        ImNodes::EndInputAttribute();

        ImNodes::BeginOutputAttribute(node.outputId);
        ImGui::Text("Output");
        ImNodes::EndOutputAttribute();
        std::string label = "##AnimTransitionFromState" + node.name + std::to_string(node.inputId) + "-" + std::to_string(node.outputId);

        ImGui::SetNextItemWidth(100.0f);
        std::string trigger = "When: " + node.state;
        ImGui::TextColored(ImVec4(255, 165, 0, 255),trigger.c_str());

        ImGui::Text("Ticks: ");
        ImGui::SameLine();
        label = "##AnimTransitionFromTick"+node.name+std::to_string(node.inputId)+"-"+std::to_string(node.outputId);
        ImGui::SetNextItemWidth(100.0f);
        ImGui::DragInt(label.c_str(),&node.connections[node.state]->ticks);

        ImNodes::EndNode();

        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();
        ImNodes::PopColorStyle();

        if (Input::IsMouseButtonPressed(MouseCode::Right) && ImNodes::IsEditorHovered())
            ImNodes::SetNodeGridSpacePos(node.id, ImVec2(node.pos.x + gridOffset.x, node.pos.y + gridOffset.y));
    }

    void AnimatorController::DrawNodeEditor() {
        ImNodes::BeginNodeEditor();

        UpdateGridOffset();


        for (auto& node:nodes) {
            nodeHandles[node.nType](node);

            auto nPos = ImNodes::GetNodeGridSpacePos(node.id);
            node.pos = glm::vec2(nPos.x - gridOffset.x, nPos.y - gridOffset.y);
            if(node.nType!=NodeType::TransitionNode)
                node.anim->nodePos = node.pos;
        }

        for (const auto& link : links) {
            ImNodes::Link(link.id, link.from, link.to);
        }

        ImNodes::EndNodeEditor();

        int hoveredId = -1;
        if (ImNodes::IsNodeHovered(&hoveredId)) {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                if (nodes[hoveredId].nType != NodeType::TransitionNode) {
                    activeNodeId = hoveredId;
                    auto nPos = ImNodes::GetNodeGridSpacePos(activeNodeId);
                    if (nodes[activeNodeId].anim) {
                        nodes[activeNodeId].anim->nodePos = glm::vec2(nPos.x, nPos.y);
                    }
                }                
            }
            else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                ImGui::OpenPopup("##NodeOptions");
                activeNodeId = hoveredId;
            }
        }

        int from, to;
        if (ImNodes::IsLinkCreated(&from, &to)) {
            ImGui::OpenPopup("##SelectStatePopup");
            newLink.from = from;
            newLink.to = to;
            for (const auto& node : nodes) {
                if (node.outputId == from) {
                    newLink.fromNodeId = node.id;
                    break;
                }
            }
            for (const auto& node : nodes) {
                if (node.inputId == to) {
                    newLink.toNodeId = node.id;
                    break;
                }
            }
        }
    }

    void AnimatorController::UpdateGridOffset() {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImNodes::IsEditorHovered()) {
            isPanning = true;
            panStart = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
            gridOffsetStart = gridOffset;
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
            isPanning = false;
        }

        if (isPanning) {
            glm::vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };
            glm::vec2 dragDelta = mousePos - panStart;
            gridOffset = gridOffsetStart + dragDelta;
        }
    }

    void AnimatorController::BuildNodeTree(AnimatorComponent& animComp) {
        nodes.clear();
        links.clear();
        nameToId.clear();
        idClock = 1;
        nodeIdClock = 0;

        for (auto& anim:animComp.animations) {
            if (!anim.has_value())
                continue;

            Node node;
            node.id = nodeIdClock++;
            node.inputId = idClock++;
            node.outputId = idClock++;
            node.name = anim.value().animation.GetName();
            node.pos = anim.value().nodePos;
            node.anim = &anim.value();
            node.isDefault = anim.value().isDefault;
            nameToId.insert({ node.name,animComp.nameToId[node.name]});
            nodes.push_back(node);
            ImNodes::SetNodeGridSpacePos(node.id, ImVec2(node.pos.x + gridOffset.x, node.pos.y + gridOffset.y));
        }

        for (const auto&animRef : animComp.animations) {
            if (!animRef.has_value())
                continue;
            auto& anim = animRef.value();
            for (const auto& [state,other] : anim.connections) {
                CreateTransitionaryNode(nodes[nameToId[anim.animation.GetName()]], nodes[nameToId[other.toAnim]],state);
            }
        }
    }

    void AnimatorController::DrawNodeConfig(AnimatorComponent& animComp) {

        ImGui::BeginChild("NodeConfig",ImVec2(225,150.0f),true);
        if (activeNodeId != -1&& nodes[activeNodeId].nType!=NodeType::TransitionNode) {
            Node& activeNode = nodes[activeNodeId];
            ImGui::Text(activeNode.name.c_str());
            ImGui::Text("Is Looping: ");
            ImGui::SameLine();
            if (ImGui::Checkbox("##animaitonControllerLoopCheck", &activeNode.anim->isLooping)) {
                auto it = animComp.idToIndex.find(static_cast<unsigned int>(activeNode.id));
                if (it != animComp.idToIndex.end()) {
                    animComp.animations[it->second].value().isLooping = activeNode.anim->isLooping;
                }
            }
            ImGui::Text("Is Default: ");
            ImGui::SameLine();
            if (ImGui::Checkbox("##animaitonControllerDefaultCheck", &activeNode.isDefault)) {
                for (auto& node:nodes) {
                    if (node.name != activeNode.name) {
                        node.isDefault = false;
                    }
                }
            }
            ImGui::Text("Playback Speed: ");
            ImGui::SameLine();
            if (ImGui::DragFloat("##animaitonControllerPlaySpeedDragFloat", &activeNode.anim->playbackSpeed, 0.01f, 0.0f, FLT_MAX)) {
                auto it = animComp.idToIndex.find(static_cast<unsigned int>(activeNode.id));
                if (it != animComp.idToIndex.end()) {
                    animComp.animations[it->second].value().playbackSpeed = activeNode.anim->playbackSpeed;
                }
            }
        }
        ImGui::EndChild();
    }

    void AnimatorController::AddTransitionPopup(AnimatorComponent& anim) {
        if (ImGui::BeginPopupModal("##SelectStatePopup",nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            std::string label = (selectedState.empty())?"Add state":selectedState ;
            if (ImGui::BeginCombo("##PopupSelectState", label.c_str())) {
                int idx = 0;
                for (const auto& s:anim.states) {
                    std::string sLab = s + "##AddStateState"+std::to_string(idx);
                    idx++;
                    if (ImGui::Selectable(sLab.c_str())) {
                        selectedState = s;
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Add State")) {
                addStateP = !addStateP;
            }
            if (addStateP) {
                static char bufferB[26] = "";
                ImGui::InputText("##EnterNewStatePopup", bufferB, IM_ARRAYSIZE(bufferB));
                ImGui::SameLine();
                if (ImGui::Button("AddState##AddStateAcceptPopup")) {
                    if (strlen(bufferB) > 0) {
                        anim.states.push_back(std::string(bufferB));
                        selectedState = bufferB;
                        bufferB[0] = '\0';
                        addStateP = false;
                    }
                }
            }
            if (ImGui::Button("Link")&&!selectedState.empty()) {  
                CreateTransitionaryNode(nodes[newLink.fromNodeId], nodes[newLink.toNodeId],selectedState);
                selectedState = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void AnimatorController::NodeRightClickPopup(AnimatorComponent& animComp) {
        if (ImGui::BeginPopup("##NodeOptions")) {
            if (ImGui::Selectable("Default##SetDefault")) {
                for (auto& node : nodes) {
                    if (node.isDefault && node.id != activeNodeId) {
                        node.isDefault = false;
                        node.anim->isDefault = false;
                        break;
                    }
                }
                nodes[activeNodeId].anim->isDefault = true;
                nodes[activeNodeId].isDefault = true;
                animComp.defaultAnimId = animComp.nameToId[nodes[activeNodeId].name];
            }
            if (ImGui::Selectable("Loop##SetLoop")) {
                nodes[activeNodeId].anim->isLooping = !nodes[activeNodeId].anim->isLooping;
            }
            if (ImGui::Selectable("Delete##DeleteNode")) {
                if (nodes[activeNodeId].nType != NodeType::TransitionNode) {
                    animComp.RemoveAnimation(nodes[activeNodeId].name);
                }
                else {
                    nodes[activeNodeId].anim->connections.erase(nodes[activeNodeId].state);
                }
                nodes.erase(nodes.begin() + activeNodeId);
                BuildNodeTree(animComp);
                activeNodeId = -1;
            }
            ImGui::EndPopup();
        }
    }

    void AnimatorController::CreateTransitionaryNode(const Node& fromNode, const Node& toNode,const std::string& state) {
        Node tNode;
        tNode.anim = fromNode.anim;
        tNode.id = nodeIdClock++;
        tNode.nType = NodeType::TransitionNode;
        tNode.inputId = idClock++;
        tNode.outputId = idClock++;
        tNode.name = "Transition Node";
        tNode.pos = (fromNode.pos+toNode.pos)*0.5f;
        ImNodes::SetNodeGridSpacePos(tNode.id, { tNode.pos.x,tNode.pos.y });

        newLink.fromNodeId = fromNode.id;
        newLink.from = fromNode.outputId;
        newLink.toNodeId = toNode.id;
        newLink.to = toNode.inputId;

        int linkId = idClock++;
        links.push_back({ linkId, newLink.from, tNode.inputId });
        linkId = idClock++;
        links.push_back({ linkId, tNode.outputId, newLink.to });

        if (state != "") {
            int defaultTick = 10;
            nodes[newLink.fromNodeId].anim->connections.insert({ state,{nodes[newLink.toNodeId].name,defaultTick} });
            tNode.connections[state] = &nodes[newLink.fromNodeId].anim->connections[state];
            tNode.state = state;
        }

        nodes.push_back(tNode);
    }

    void AnimatorController::DrawAnimFlags(AnimatorComponent& animComp) {
        ImGui::BeginChild("##AnimatorStateConfig",ImVec2(0,0),true);
        ImGui::Text("Current states: ");
        ImGui::Indent(20.0f);
        if (ImGui::Button("Add State")) {
            addState = !addState;
        }
        if (addState) {
            static char bufferB[26] = "";
            ImGui::InputText("##EnterNewState", bufferB, IM_ARRAYSIZE(bufferB));
            ImGui::SameLine();
            if (ImGui::Button("AddState##AddStateAccept")) {
                if (strlen(bufferB) > 0) {
                    animComp.states.push_back(std::string(bufferB));
                    bufferB[0] = '\0';
                    addState = false;
                }
            }
        }
        for (const auto& entry:animComp.states) {
            ImGui::Text(entry.c_str());
        }
        ImGui::Unindent(20.0f);
        ImGui::EndChild();
    }
}