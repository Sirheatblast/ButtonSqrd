#pragma once

#include"ButtonSqrd/Render/Shader.h"
#include"ButtonSqrd/Render/Renderer.h"
#include"ButtonSqrd/Render/Buffers.h"

#include<glad/glad.h>
#include<glm/glm.hpp>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include"ButtonSqrd/Core/Logger.h"

namespace BtnSqd {
	class OpenGLShader:public Shader {
	public:
		OpenGLShader(const std::string vertexSource, const std::string fragmentSource);
		OpenGLShader(const std::string vertexSource,const std::string geoSource, const std::string fragmentSource);
		~OpenGLShader() override;

		void Use() override;
		void Detatch() override;

		void SetBool(const std::string& name, bool value)const override;
		void SetInt(const std::string& name, int value)const override;
		void SetUnsignedInt(const std::string& name,unsigned int value)const override;
		void SetFloat(const std::string& name, float value)const override;
		void SetVec2(const std::string& name, glm::vec2 value)const override;
		void SetVec3(const std::string& name, glm::vec3 value)const override;
		void SetVec4(const std::string& name, glm::vec4 value)const override;
		void SetMat3(const std::string& name, glm::mat4 value)const override;
		void SetMat4(const std::string& name, glm::mat4 value)const override;
		void SetUniform(const std::string& name, int value) const override;
		void SetVec3Array(const std::string& name, std::vector<glm::vec3> value) const override;
		void SetFloatArray(const std::string& name, std::vector<float> value) const override;
		void SetMat4Array(const std::string& name, std::vector<glm::mat4> value) const override;

		unsigned int GetId() override;
		void SetShaderName(std::string shaderNam) override {
			shaderName = shaderNam;
		}
		std::string GetShaderName() override;

	private:
		uint32_t renderId;
		std::string shaderName;
	};
}