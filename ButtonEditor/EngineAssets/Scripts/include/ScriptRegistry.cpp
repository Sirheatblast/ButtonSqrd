#include "ScriptRegistry.h"

extern "C" __declspec(dllexport)
const RawScriptEntry* GetRegisteredScripts(size_t* count) {
    auto& internal = GetScriptRegistryInternal();
    auto& view = GetScriptRegistryView();

    view.clear();
    for (auto& e : internal)
        view.push_back({ e.name, e.create });

    *count = view.size();
    return view.data();
}

void SetApp(AppInfo* appInfo) {
    BtnSqd::Application::GetAppRef() = appInfo->app;
}
