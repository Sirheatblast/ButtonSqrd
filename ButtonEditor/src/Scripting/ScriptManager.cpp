#include "ScriptManager.h"

#include<string>
#include<cstdlib>
#include<fstream>
#include <filesystem>

#include"ButtonSqrd/Core/Logger.h"

namespace fs = std::filesystem;

static bool ShouldRecompile(const fs::path& dllPath, const fs::path& sourceDir) {
	if (!fs::exists(dllPath)) return true;

	auto dllTime = fs::last_write_time(dllPath);
	for (auto& p : fs::recursive_directory_iterator(sourceDir)) {
		if (p.path().extension() == ".cpp" || p.path().extension() == ".h") {
			if (fs::last_write_time(p.path()) > dllTime)
				return true;
		}
	}
	return false;
}

namespace BtnSqd {
	bool ScriptManager::CompileScripts() {
		const std::string MSBUILD_PATH = "\"C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe\"";
		BTNLOG_INFO("Start Compliation of Scripts");
		if (ShouldRecompile("EngineAssets/Scripts/bin/BtnScripts.dll", "Assets/Scripts")) {
			system("src\\Scripting\\Premake\\premake5.exe vs2026");

			std::string config = "Release";
			#ifdef BTN_DEBUG
				config = "Debug";
			#endif

			std::string cmd = MSBUILD_PATH + " EngineAssets\\Scripts\\ScriptBuild.sln /p:Configuration=" + config + " /p:Platform=x64 /p:WarningLevel=0";

			FILE* pipe = _popen(cmd.c_str(), "r");
			if (pipe) {
				char buffer[512];
				while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
					std::string line(buffer);

					while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) {
						line.pop_back();
					}

					if (line.empty() ||
						line.find("warning LNK4099") != std::string::npos ||
						line.find("Microsoft (R) Build Engine") != std::string::npos) {
						continue;
					}

					if (line.find("Build succeeded") != std::string::npos) {
						BTNLOG_INFO("SUCCESS: Scripts compiled and linked.");
					}
					else if (line.find("Build FAILED") != std::string::npos || line.find(": error") != std::string::npos) {
						BTNLOG_ERROR(line);
					}
					else if (line.find("Warning(s)") != std::string::npos) {
						BTNLOG_WARN(line);
					}
					else {
						BTNLOG_TRACE(line);
					}
				}
				_pclose(pipe);
			}
		}


		BTNLOG_INFO("Finished Compliation of Scripts");

		return true;
	}
	bool ScriptManager::CreateScript(std::string name) {
		std::string headerPath = "./Assets/Scripts/" + name + ".h";
		std::string cppPath = "./Assets/Scripts/" + name + ".cpp";

		std::ofstream headerFile(headerPath);
		headerFile << GetTemplate(name, "./EngineAssets/HeaderTemplate.txt");
		headerFile.close();

		std::ofstream cppFile(cppPath);
		cppFile << GetTemplate(name, "./EngineAssets/CppTemplate.txt");
		cppFile.close();

		system("src\\Scripting\\Premake\\premake5.exe vs2022");
		return false;
	}
	std::string ScriptManager::GetTemplate(std::string name, std::string path) {
		std::ifstream file(path);
		std::stringstream buffer;

		auto replaceAll = [](std::string str, const std::string& from, const std::string& to) {
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length();
			}
			return str;
			};

		buffer << file.rdbuf();
		std::string scriptTemplate = replaceAll(buffer.str(), "[NewScriptNameGoesHere]", name);

		return scriptTemplate;
	}
}