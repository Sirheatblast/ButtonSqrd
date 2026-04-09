#include "OpenGLShader.h"
#include<glm/gtc/type_ptr.hpp>

namespace BtnSqd {
	OpenGLShader::OpenGLShader(const std::string vertexSource, const std::string fragmentSource) {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.open(vertexSource);
		fShaderFile.open(fragmentSource);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		std::string vertCode;
		std::string fragCode;

		vertCode = vShaderStream.str();
		fragCode = fShaderStream.str();

		const GLchar* source = (const GLchar*)vertCode.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			std::string info = infoLog.data();
			BTN_CORE_LOG_ERROR("Vertex shader failed to compile "+ info);
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragCode.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			std::string info = infoLog.data();
			BTN_CORE_LOG_ERROR("Fragment shader failed to compile "+ info);
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		renderId = glCreateProgram();
		GLuint program = renderId;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			std::string error = infoLog.data();
			BTN_CORE_LOG_ERROR("Shader program failed to link: " + error);
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::OpenGLShader(const std::string vertexSource, const std::string geoSource, const std::string fragmentSource) {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		std::ifstream vShaderFile;
		std::ifstream gShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.open(vertexSource);
		gShaderFile.open(geoSource);
		fShaderFile.open(fragmentSource);

		std::stringstream vShaderStream, fShaderStream,gShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		std::string vertCode;
		std::string geoCode;
		std::string fragCode;

		vertCode = vShaderStream.str();
		geoCode = gShaderStream.str();
		fragCode = fShaderStream.str();

		const GLchar* source = (const GLchar*)vertCode.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			std::string info = infoLog.data();
			BTN_CORE_LOG_ERROR("Vertex shader failed to compile " + info);
			return;
		}
		// Create an empty geo shader
		GLuint geoShader = glCreateShader(GL_GEOMETRY_SHADER);
		source = (const GLchar*)geoCode.c_str();
		glShaderSource(geoShader, 1, &source, 0);

		glCompileShader(geoShader);

		glGetShaderiv(geoShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(geoShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(geoShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(geoShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			std::string info = infoLog.data();
			BTN_CORE_LOG_ERROR("Geometry shader failed to compile " + info);
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragCode.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(geoShader);
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			std::string info = infoLog.data();
			BTN_CORE_LOG_ERROR("Fragment shader failed to compile " + info);
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		renderId = glCreateProgram();
		GLuint program = renderId;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, geoShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			std::string error = infoLog.data();
			BTN_CORE_LOG_ERROR("Shader program failed to link: " + error);
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, geoShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::OpenGLShader(const std::string& computeSource) {
		GLint computeShader = glCreateShader(GL_COMPUTE_SHADER);

		std::ifstream computeShaderFile;

		computeShaderFile.open(computeSource);

		std::stringstream computeShaderStream;
		computeShaderStream << computeShaderFile.rdbuf();

		std::string computeCode;
		computeCode = computeShaderStream.str();

		const GLchar* source = (const GLchar*)computeCode.c_str();
		glShaderSource(computeShader, 1, &source, 0);
		glCompileShader(computeShader);

		GLint isCompiled = 0;
		glGetShaderiv(computeShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)	{
			GLint maxLength = 0;
			glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(computeShader, maxLength, &maxLength, &infoLog[0]);
			glDeleteShader(computeShader);
			std::string info = infoLog.data();
			BTN_CORE_LOG_ERROR("Compute shader failed to compile " + info);
			return;
		}

		renderId = glCreateProgram();
		GLuint program = renderId;
		glAttachShader(program, computeShader);
		glLinkProgram(program);
		GLint isLinked = 0;

		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			glDeleteShader(computeShader);

			std::string error = infoLog.data();
			BTN_CORE_LOG_ERROR("Shader program failed to link: " + error);
			return;
		}

		glDetachShader(program, computeShader);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(renderId);
	}

	void OpenGLShader::Use() {
		glUseProgram(renderId);
	}
	void OpenGLShader::Detatch() {
		glUseProgram(0);
	}
	void OpenGLShader::SetBool(const std::string& name, bool value) const {
		glUseProgram(renderId);
		auto loc = glGetUniformLocation(renderId, name.c_str());
		glUniform1i(loc, value);
	}
	void OpenGLShader::SetInt(const std::string& name, int value) const {
		glUseProgram(renderId);
		glUniform1iv(glGetUniformLocation(renderId, name.c_str()), 1, &value);
	}
	void OpenGLShader::SetUnsignedInt(const std::string& name, unsigned int value) const {
		glUseProgram(renderId);
		glUniform1ui(glGetUniformLocation(renderId, name.c_str()), value);
	}
	void OpenGLShader::SetFloat(const std::string& name, float value) const {
		glUseProgram(renderId);
		glUniform1fv(glGetUniformLocation(renderId, name.c_str()), 1, &value);
	}
	void OpenGLShader::SetVec2(const std::string& name, glm::vec2 value) const {
		glUseProgram(renderId);
		glUniform2fv(glGetUniformLocation(renderId, name.c_str()), 1, &value[0]);
	}
	void OpenGLShader::SetVec3(const std::string& name, glm::vec3 value) const {
		glUseProgram(renderId);
		glUniform3fv(glGetUniformLocation(renderId, name.c_str()), 1, &value[0]);
	}
	void OpenGLShader::SetVec4(const std::string& name, glm::vec4 value) const {
		glUseProgram(renderId);
		glUniform4fv(glGetUniformLocation(renderId, name.c_str()), 1, &value[0]);
	}
	void OpenGLShader::SetMat3(const std::string& name, glm::mat4 value) const {
		glUseProgram(renderId);
		GLint location = glGetUniformLocation(renderId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetMat4(const std::string& name, glm::mat4 value) const {
		glUseProgram(renderId);
		GLint location = glGetUniformLocation(renderId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetUniform(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(renderId, name.c_str()), value);
	}
	void OpenGLShader::SetVec3Array(const std::string& name, std::vector<glm::vec3> value) const{
		GLint location = glGetUniformLocation(renderId, name.c_str());
		glUniform3fv(location, static_cast<GLsizei>(value.size()), &value.data()[0].x);
	}

	void OpenGLShader::SetFloatArray(const std::string& name,const std::vector<float>& value) const {
		GLint location = glGetUniformLocation(renderId, name.c_str());
		glUniform1fv(location, static_cast<GLsizei>(value.size()), &value.data()[0]);
	}
	unsigned int OpenGLShader::GetId() {
		return renderId;
	}
	std::string OpenGLShader::GetShaderName() {
		return shaderName;
	}
	void OpenGLShader::SetMat4Array(const std::string& name,const std::vector<glm::mat4>& value) const	{
		glUseProgram(renderId);
		GLint loc = glGetUniformLocation(renderId, (name + "[0]").c_str());
		if (loc != -1) {
			glUniformMatrix4fv(loc,static_cast<GLsizei>(value.size()),GL_FALSE,glm::value_ptr(value[0]));
		}
		glUseProgram(0);
	}
}