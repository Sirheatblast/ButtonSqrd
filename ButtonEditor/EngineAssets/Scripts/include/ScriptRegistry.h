#pragma once

#include <string>
#include <vector>
#include<ButtonSqrd.h>

using CreateScriptFunc = BtnSqd::BtnScript* (*)();

#define Editable(type,name) RegisterEditable(#name,GetType(#type),&name)

struct RawScriptEntry {
    const char* name;
    CreateScriptFunc create;
};

struct ApplicationInfo {
    BtnSqd::Window* window;
};

struct ScriptEntryInternal {
    std::string name;
    CreateScriptFunc create;
};

static DataType GetType(const char* rawType) {
    if (rawType=="float") {
        return DataType::Float;
    }
    if (rawType=="double") {
        return DataType::Double;
    }
    if (rawType =="std::string") {
        return DataType::String;
    }
    if (rawType =="bool") {
        return DataType::Bool;
    }
    if (rawType=="BtnSqd::GameObject") {
        return DataType::GameObject;
    }
    if (rawType == "BtnSqd::SuperGameObject") {
        return DataType::SuperGameObject;
    }
    return DataType::Int;
}

inline std::vector<ScriptEntryInternal>& GetScriptRegistryInternal() {
    static std::vector<ScriptEntryInternal> reg;
    return reg;
}

inline std::vector<RawScriptEntry>& GetScriptRegistryView() {
    static std::vector<RawScriptEntry> view;
    return view;
}

struct ScriptRegistrar {
    ScriptRegistrar(const std::string& name, CreateScriptFunc func) {
        GetScriptRegistryInternal().push_back({ name, func });
    }
};

struct AppInfo {
    BtnSqd::Application* app;
};

#define REGISTER_SCRIPT(CLASS_NAME) \
    extern "C" BtnSqd::BtnScript* Create##CLASS_NAME(){return new CLASS_NAME();}\
    static ScriptRegistrar registrar_##CLASS_NAME(#CLASS_NAME, Create##CLASS_NAME)

extern "C" __declspec(dllexport)
const RawScriptEntry* GetRegisteredScripts(size_t* count);
extern "C" __declspec(dllexport)
void SetApp(AppInfo* appInfo);
