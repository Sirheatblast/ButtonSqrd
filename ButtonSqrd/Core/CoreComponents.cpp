#include "CoreComponents.h"
#include"ButtonSqrd/Core/GameObject.h"
#include"ButtonSqrd/Core/SuperGameObject.h"
#include"Application.h"
namespace BtnSqd{

    struct AppInfo {
        Application* app;
    };

	bool BtnSqd::ScriptComponent::AddScript(GameObject* gameObject, const std::string& dllPath, std::string scriptName,BtnScene* scene) {
        ScriptData nScript;
        nScript.isEnabled = true;
        nScript.dllHandle = LoadLibraryA(dllPath.c_str());

        auto getScripts = (GetScriptsFunc)GetProcAddress(nScript.dllHandle, "GetRegisteredScripts");
        auto setApp = (void(*)(AppInfo*))GetProcAddress(nScript.dllHandle, "SetApp");

        if (!getScripts ||!setApp) {
            BTNLOG_ERROR("Failed to load Script DLL");
            return false;
        }
        
        AppInfo appInfo = { Application::GetAppRef() };
        setApp(&appInfo);

        size_t count = 0;
        const ScriptEntry* entries = getScripts(&count);

        for (size_t i = 0; i < count; i++) {
            if (scriptName == entries[i].name) {
                BtnScript* instance = entries[i].create();
                if (!instance) {
                    BTNLOG_ERROR("Failed to create {} script", scriptName);
                    continue;
                }
                nScript.script = instance;
                nScript.script->SetGameObject(gameObject);
                nScript.script->SetScene(scene);
                nScript.script->SetLogger(Logger::GetClientConsole());
                nScript.name = entries[i].name;
                nScript.script->OnCreate();
                scripts.emplace_back(nScript);

                return true;
            }
        }

		BTNLOG_ERROR("Script not found in DLL: {}", scriptName);
		FreeLibrary(nScript.dllHandle);
		return false;
	}
    bool ScriptComponent::AddScript(SuperGameObject* gameObject, const std::string& dllPath, std::string scriptName) {
        ScriptData nScript;
        nScript.isEnabled = true;
        nScript.dllHandle = LoadLibraryA(dllPath.c_str());

        auto getScripts = (GetScriptsFunc)GetProcAddress(nScript.dllHandle, "GetRegisteredScripts");
        auto setApp = (void(*)(AppInfo*))GetProcAddress(nScript.dllHandle, "SetApp");

        if (!getScripts || !setApp) {
            BTNLOG_ERROR("Failed to load Script DLL");
            return false;
        }

        AppInfo appInfo = { Application::GetAppRef() };
        setApp(&appInfo);

        size_t count = 0;
        const ScriptEntry* entries = getScripts(&count);

        for (size_t i = 0; i < count; i++) {
            if (scriptName == entries[i].name) {
                BtnScript* instance = entries[i].create();
                if (!instance) {
                    BTNLOG_ERROR("Failed to create {} script", scriptName);
                    continue;
                }
                nScript.script = instance;
                nScript.script->SetLogger(Logger::GetClientConsole());
                nScript.name = entries[i].name;
                nScript.script->OnCreate();
                scripts.emplace_back(nScript);

                return true;
            }
        }

        BTNLOG_ERROR("Script not found in DLL: {}", scriptName);
        FreeLibrary(nScript.dllHandle);
        return false;
    }
    physx::PxForceMode::Enum GetForceType(BtnForce force) {
        switch (force) {
        case BtnForce::Velocity:
            return physx::PxForceMode::eVELOCITY_CHANGE;
        case BtnForce::Impulse:
            return physx::PxForceMode::eIMPULSE;
        }
        return physx::PxForceMode::eFORCE;
    }

    bool PhysicsComponet::AddVelocity(glm::vec3 direction,BtnForce force) {
        if (isKinematic) {
            BTNLOG_WARN("Kinematic objects don't support this function");
            return false;
        }
        if (direction ==glm::vec3() || !rigidBody) {
            return false;
        }
        physx::PxVec3 nDir{ direction.x,direction.y,direction.z };


        static_cast<physx::PxRigidBody*>(rigidBody)->addForce(nDir, physx::PxForceMode::eIMPULSE);
        return true;
    }
    void PhysicsComponet::SetVelocity(glm::vec3 direction) {
        if (isKinematic) {
            BTNLOG_WARN("Kinematic objects don't support this function");
            return;
        }
        physx::PxVec3 nDir{ direction.x,direction.y,direction.z };
        static_cast<physx::PxRigidDynamic*>(rigidBody)->setLinearVelocity(nDir);
    }
    void PhysicsComponet::SetMaxSpeed(float maxSpeed) {
        if (isKinematic) {
            BTNLOG_WARN("Kinematic objects don't support this function");
            return;
        }
        static_cast<physx::PxRigidDynamic*>(rigidBody)->setMaxLinearVelocity(maxSpeed);
    }
    glm::vec3 PhysicsComponet::GetVelocity() {
        if (isKinematic) {
            BTNLOG_WARN("Kinematic objects don't support this function");
            return glm::vec3();
        }
        physx::PxVec3 vel = static_cast<physx::PxRigidDynamic*>(rigidBody)->getLinearVelocity();
        return glm::vec3(vel.x,vel.y,vel.z);
    }
    glm::vec3 CameraComponent::ScreenToWorldPos(glm::vec2 screenPos) {
        ViewPort viewP = Application::GetApp()->GetCurrentViewPort();
        screenPos -= viewP.offset;

        float depth = 0.0f;

        float x = (2.0f * screenPos.x) / viewP.size.x - 1.0f;
        float y = 1.0f - (2.0 * screenPos.y) / viewP.size.y;
        float z = 2.0f * depth - 1.0f;

        glm::vec4 clipCoords(x, y, z, 1.0f);
        glm::vec4 eyeCoords = glm::inverse(projectionMatrix) * clipCoords;
        eyeCoords.z = -1.0f;
        eyeCoords.w = 0.0f;

        glm::vec4 worldCoords = glm::inverse(viewMatrix) * eyeCoords;
        return glm::normalize(glm::vec3(worldCoords));
    }
}
