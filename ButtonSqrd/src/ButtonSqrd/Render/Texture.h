#pragma once
#include<string>

namespace BtnSqd {

	enum class TextureType {
		Texture_1D,Texture_2D,Texture_3D,CubeMap,Texture_2D_Array
	};

	enum class TextureWrappingInfo {
		Repeat,Mirrored_Repeat,Clamp,Clamp_Border
	};

	enum class TextureFilterSettings {
		Linear, Linear_Mip_Map,Near,Near_Linear_mip
	};

	enum class TextureDataType {
		Color,SingleColor,Depth
	};

	enum class TextureSlot {
		Albedo = 0, Normal,Texture2,Texture3,Texture4, Texture5, Texture6, Texture7, Texture8, Texture9, Texture10, Texture11, Texture12, Texture13, Texture14, Texture15, Texture16
	};

	enum class TexFormat {
		RGBA,RGB_INT,SINGLE
	};

	struct TextureSettings {
		TextureSettings():texType(TextureType::Texture_2D),wrappingMode(TextureWrappingInfo::Repeat),texMax(TextureFilterSettings::Linear),texMin(TextureFilterSettings::Linear_Mip_Map),texDataType(TextureDataType::Color),texSlot(TextureSlot::Albedo),texFormat(TexFormat::RGBA){}

		TextureWrappingInfo wrappingMode;
		TextureType texType;
		TextureFilterSettings texMax;
		TextureFilterSettings texMin;
		TextureDataType texDataType;
		TextureSlot texSlot;
		TexFormat texFormat;
		unsigned int maxLayers = 1;
	};

	class Texture {
	public:
		virtual void Bind() = 0;
		virtual void Bind(TextureSlot texSlot) = 0;
		virtual void UnBind() = 0;
		virtual void SetType(std::string tType) = 0;

		virtual std::string GetPath()=0;
		virtual unsigned int GetId() = 0;

		virtual std::tuple<unsigned int, unsigned int> GetResolution() = 0;

		virtual void Resize(unsigned int nWidth, unsigned int nHeight) = 0;;

		static Texture* Create(std::string const path,TextureSettings texSettings);
		static Texture* Create(unsigned int width,unsigned int height,TextureSettings texSettings,void* data = nullptr);
	};
}