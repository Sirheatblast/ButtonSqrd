#pragma once

#include"Texture.h"
#include<string>
#include<vector>
#include<memory>

namespace BtnSqd {
	enum class FrameBufferAttatchment {
		ColorAttatchment = 0,
		DepthAttatchment,
		StencilAttatchment,
		DepthStencilAttatchment
	};

	class FrameBuffer {
	public:
		//Binds the framebuffer normally
		virtual void Bind()=0;
		//Binds the framebuffer in a way that allows you to write to it
		virtual void WBind() = 0;
		virtual void UnBind()=0;


		virtual void AttatchTexture(TextureSlot texSlot)=0;
		virtual void AttatchTexture(TextureSlot texSlot,TextureSettings)=0;
		virtual void AttatchTextureArray(TextureType texType, unsigned int arraySize) = 0;
		virtual void AttatchTextureArray(TextureType texType, unsigned int arraySize,unsigned int nWidth,unsigned int nHeight) = 0;
		virtual void AttatchRenderBuffer()=0;

		virtual void GenerateCubeMapArray(unsigned int mapTexture, unsigned int index, unsigned int arraySize) = 0;
		virtual void GenerateCubeMap(TextureType texType, unsigned int mapTexture) = 0;

		virtual void ClearTextureArrays() = 0;
		virtual std::vector<unsigned int>& GetTextureArrays() = 0;
		virtual bool GetStatus() = 0;
		virtual unsigned int GetId() = 0;
		virtual Texture* GetTexture() = 0;
		virtual Texture* GetDepthTexture() = 0;

		//Frist element is the GameObject Id, second is the draw Id, third is the primitive Id
		virtual std::tuple<unsigned int,unsigned int,unsigned int> GetPixelInformation(int positionX, int positionY) = 0;

		virtual std::pair<int, int> GetResolution() = 0;
		virtual void SetResolution(int nWidth, int nHeight) = 0;

		static FrameBuffer* Create(FrameBufferAttatchment frameAttatchment,unsigned int width,unsigned int height);
	};

}