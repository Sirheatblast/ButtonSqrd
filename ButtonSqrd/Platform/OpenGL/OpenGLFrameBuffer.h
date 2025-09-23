#pragma once

#include"ButtonSqrd/Render/FrameBuffer.h"
#include"ButtonSqrd/Render/Texture.h"

#include<vector>
#include<glad/glad.h>
#include<glm/glm.hpp>

namespace BtnSqd {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(FrameBufferAttatchment frameAttatchment,unsigned int width, unsigned int height);
		~OpenGLFrameBuffer();

		void Bind() override;
		void WBind() override;
		void UnBind() override;
		void AttatchTexture(TextureSlot texSlot) override;
		void AttatchTexture(TextureSlot texSlot,TextureSettings texSettings) override;
		void AttatchTextureArray(TextureType texType, unsigned int arraySize) override;
		void AttatchTextureArray(TextureType texType, unsigned int arraySize, unsigned int nWidth, unsigned int nHeight) override;
		void AttatchRenderBuffer() override;

		void GenerateCubeMapArray(unsigned int mapTexture, unsigned int index, unsigned int arraySize) override;
		void GenerateCubeMap(TextureType texType,unsigned int mapTexture) override;

		void ClearTextureArrays() override;

		bool GetStatus() override;

		unsigned int GetId() override;
		std::vector<unsigned int>& GetTextureArrays() override;

		Texture* GetTexture() override;
		Texture* GetDepthTexture() override;
		std::pair<int, int> GetResolution() override;
		void SetResolution(int nWidth,int nHeight) override;

		std::tuple<unsigned int, unsigned int, unsigned int>GetPixelInformation(int positionX, int positionY) override;

	private:
		GLenum GetBufferAttatchment(FrameBufferAttatchment frameAttatchment);
		GLenum GetTextureType(TextureType texType);
		GLenum GetTextureArrayType(TextureType texType);

		Texture* currentTexture;
		Texture* renderBufferTexture;
		GLenum bufferAttactchment;
		unsigned int id, rbId, textureId;
		unsigned int width, height;

		std::vector<unsigned int> textureArrays;
	};
}