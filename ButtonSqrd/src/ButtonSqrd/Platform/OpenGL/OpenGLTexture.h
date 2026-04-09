#pragma once

#include"ButtonSqrd/Render/Texture.h"

#include<glad/glad.h>
#include<ButtonSqrd/Core/Logger.h>
#include<vector>
#include<string>

namespace BtnSqd {
	class OpenGLTexture :public Texture {
	public:
		OpenGLTexture(std::string path, TextureSettings texSettings);
		OpenGLTexture(unsigned int width,unsigned int height, TextureSettings texSettings,void* data = nullptr);

		~OpenGLTexture();

		OpenGLTexture(OpenGLTexture& glTex);
		void Bind() override;
		void Bind(TextureSlot texSlot) override;
		void UnBind() override;
		void SetType(std::string tType) override;

		std::string GetPath() override;
		unsigned int GetId() override;

		void Resize(unsigned int nWidth, unsigned int nHeight) override;
		std::tuple<unsigned int, unsigned int> GetResolution() override;
	private:
		void LoadCubeMap();
		void LoadNormal();

		GLenum GetTextureType();
		GLenum GetTextureWrapData();
		GLenum GetTextureMaxFilterSettings();
		GLenum GetTextureMinFilterSettings();
		GLenum GetTextureDataType();
		GLenum GetTextreSlot();

		GLenum GetTextureInternal();
		GLenum GetTextureFormat();
		GLenum GetTextureDataInternalType();

		unsigned int textureId;
		int width, height, nrComponents, nrChannels;
		std::string path,textureType;

		TextureSettings texSettings;
	};
}