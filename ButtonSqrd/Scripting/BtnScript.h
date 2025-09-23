#pragma once
#include <string>
#include"ButtonSqrd/Core/Logger.h"

enum DataType:int {
    Int = 0,
    String,
    Bool,
    Float,
    Double,
    GameObject,
    SuperGameObject
};

struct EditableData {
    const char* name;
    DataType type;
    void* data;
};


class EditableRegister {
public:
    void AddEditable(EditableData data) {
        scriptEditables.emplace_back(data);
    }
    std::vector<EditableData>& GetEditables() { return scriptEditables; }
private:
    std::vector<EditableData> scriptEditables;
};

namespace BtnSqd {
    class GameObject;
    class BtnScene;

    class BtnScript {
    public:
        void SetGameObject(GameObject* gObj) {
            gameObject = gObj;
        }

        void SetLogger(spdlog::logger* nLogger) {
            logger = nLogger;
        }

        void SetScene(BtnScene* currentScene) {
            scene = currentScene;
        }

        virtual ~BtnScript() {}

        virtual void OnCreate() {}
        virtual void OnStart() {}
        virtual void OnAwake() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnPhyUpdate(float deltaTime) {}
        virtual void OnDestroy() {}

        virtual void OnColliderEnter(GameObject* other){}
        virtual void OnColliderLeave(GameObject* other){}
        
        EditableData* GetEditables(uint32_t& count) {
            auto& vars = editableRegister.GetEditables();
            count = static_cast<uint32_t>(vars.size());
            return vars.data();
        }
    protected:

        void RegisterEditable(const char* name, DataType type, void* data) {
            editableRegister.AddEditable({name, type, data});
        }


        EditableRegister editableRegister;
        GameObject* gameObject = nullptr;
        BtnScene* scene;
        spdlog::logger* logger = nullptr;
    };

}
