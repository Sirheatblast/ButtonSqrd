#pragma once

#include<string>
namespace BtnSqd {
	class ScriptManager {
	public:
		static bool CompileScripts();
		static bool CreateScript(std::string name = "newScript");
	private:
		static std::string GetTemplate(std::string name,std::string path);
	};
}