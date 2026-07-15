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
    const char* name;
    CreateScriptFunc create;
};

static DataType GetType(const char* rawType) {
    if (std::strcmp(rawType, "float") == 0) return DataType::Float;
    if (std::strcmp(rawType, "double") == 0) return DataType::Double;
    if (std::strcmp(rawType, "std::string") == 0) return DataType::String;
    if (std::strcmp(rawType, "bool") == 0) return DataType::Bool;
    if (std::strcmp(rawType, "BtnSqd::GameObject") == 0) return DataType::GameObject;
    if (std::strcmp(rawType, "BtnSqd::SuperGameObject") == 0) return DataType::SuperGameObject;
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
    ScriptRegistrar(const const char* name, CreateScriptFunc func) {
        GetScriptRegistryInternal().push_back({ name, func });
    }
};

struct AppInfo {
    BtnSqd::Application* app;
};

#define REGISTER_SCRIPT(CLASS_NAME) \
extern "C" __declspec(dllexport) BtnSqd::BtnScript* Create##CLASS_NAME(){return new CLASS_NAME();}\
static ScriptRegistrar registrar_##CLASS_NAME(#CLASS_NAME, Create##CLASS_NAME)

extern "C" __declspec(dllexport)
const RawScriptEntry* GetRegisteredScripts(size_t* count);
extern "C" __declspec(dllexport)
void SetApp(AppInfo* appInfo);
