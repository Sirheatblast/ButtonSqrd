#pragma once

#include<glm/glm.hpp>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>


namespace BtnSqd {
	class Shader {
	public:
		virtual ~Shader()=default;

		virtual void Use() =0;
		virtual void Detatch() =0;

		virtual void SetBool(const std::string& name, bool value)const =0;
		virtual void SetInt(const std::string& name, int value)const =0;
		virtual void SetUnsignedInt(const std::string& name, unsigned int value)const =0;
		virtual void SetFloat(const std::string& name, float value)const =0;
		virtual void SetVec2(const std::string& name, glm::vec2 value)const =0;
		virtual void SetVec3(const std::string& name, glm::vec3 value)const =0;
		virtual void SetVec4(const std::string& name, glm::vec4 value)const =0;
		virtual void SetMat3(const std::string& name, glm::mat4 value)const =0;
		virtual void SetMat4(const std::string& name, glm::mat4 value)const =0;
		virtual void SetUniform(const std::string& name, int value)const = 0;
		virtual void SetVec3Array(const std::string& name, std::vector<glm::vec3> value)const = 0;
		virtual void SetFloatArray(const std::string& name,const std::vector<float>& value) const = 0;
		virtual void SetMat4Array(const std::string& name,const std::vector<glm::mat4>& value)const = 0;

		virtual void SetShaderName(std::string shaderNam) = 0;

		virtual std::string GetShaderName() = 0;
		virtual unsigned int GetId() = 0;

		static Shader* Create(const std::string vertexSource, const std::string fragmentSource);
		static Shader* Create(const std::string vertexSource,const std::string geoSource, const std::string fragmentSource);
		static Shader* Create(const std::string& computeSource);
	};
}