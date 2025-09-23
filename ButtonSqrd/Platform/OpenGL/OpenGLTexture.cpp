#include"OpenGLTexture.h"

#include<stb_image.h>

namespace BtnSqd {
	OpenGLTexture::OpenGLTexture(std::string path,TextureSettings texSettings):texSettings(texSettings),path(path){
		glActiveTexture(GetTextreSlot());
		switch (texSettings.texType) {
			case TextureType::CubeMap:
				LoadCubeMap();
			break;
			
			default:
				LoadNormal();
			break;
		}
		glBindTexture(GetTextureType(), 0);
	}
	OpenGLTexture::OpenGLTexture(unsigned int width, unsigned int height, TextureSettings texSettings):texSettings(texSettings),width(width),height(height) {
		glActiveTexture(GetTextreSlot());
		glGenTextures(1, &textureId);
		glBindTexture(GetTextureType(), textureId);
		if (texSettings.texFormat == TexFormat::RGBA) {
			glTexImage2D(GetTextureType(), 0, GetTextureDataType(), width, height, 0, GetTextureDataType(), GL_FLOAT, nullptr);
		}
		else if(texSettings.texFormat==TexFormat::RGB_INT) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
		}

		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GetTextureType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GetTextureType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexParameteri(GetTextureType(), GL_TEXTURE_WRAP_S, GetTextureWrapData());
		glTexParameteri(GetTextureType(), GL_TEXTURE_WRAP_T, GetTextureWrapData());

		glBindTexture(GetTextureType(), 0);
	}

	OpenGLTexture::~OpenGLTexture(){
		glDeleteTextures(1, &textureId);
	}

	OpenGLTexture::OpenGLTexture(OpenGLTexture& glTex):texSettings(glTex.texSettings),width(glTex.width),height(glTex.height),textureId(glTex.textureId),path(glTex.path),nrComponents(glTex.nrComponents){}

	void OpenGLTexture::Bind(){
		glActiveTexture(GetTextreSlot());
		glBindTexture(GetTextureType(), textureId);
	}

	void OpenGLTexture::Bind(TextureSlot texSlot) {
		texSettings.texSlot = texSlot;
		glActiveTexture(GetTextreSlot());
		glBindTexture(GetTextureType(), textureId);
	}

	void OpenGLTexture::LoadCubeMap(){
		std::vector<std::string> faces = {
			path+"/right.jpg",
			path+"/left.jpg",
			path+"/top.jpg",
			path+"/bottom.jpg",
			path+"/front.jpg",
			path+"/back.jpg"
		};

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		stbi_set_flip_vertically_on_load(false);
		for (int i = 0; i < faces.size(); i++) {
			unsigned char* data = stbi_load(faces[i].c_str(),&width,&height,&nrChannels,0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GetTextureInternal(), width, height, 0, GetTextureFormat(), GetTextureDataInternalType(), data);
				stbi_image_free(data);
			}
			else {
				BTN_CORE_LOG_ERROR("Failed to load texture at path: "+ faces[i]);
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void OpenGLTexture::LoadNormal() { 
		glGenTextures(1, &textureId);
		glBindTexture(GetTextureType(), textureId);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GetTextureType(), GL_TEXTURE_WRAP_S, GetTextureWrapData());
		glTexParameteri(GetTextureType(), GL_TEXTURE_WRAP_T, GetTextureWrapData());
		glTexParameteri(GetTextureType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GetTextureType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// load and generate the texture
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GetTextureType(), 0, GetTextureInternal(), width, height, 0, GetTextureFormat(), GetTextureDataInternalType(), data);
			glGenerateMipmap(GetTextureType());
		}
		else {
			BTN_CORE_LOG_ERROR("Failed to load texture at path: " + path);
		}
		stbi_image_free(data);
	}

	GLenum OpenGLTexture::GetTextureType() {
		switch (texSettings.texType) {
			case TextureType::Texture_1D:
				return GL_TEXTURE_1D;
				break;
			case TextureType::Texture_2D:
				return GL_TEXTURE_2D;
				break;
			case TextureType::Texture_3D:
				return GL_TEXTURE_3D;
				break;
			case TextureType::CubeMap:
				return GL_TEXTURE_CUBE_MAP;
				break;
		}
		return 0;
	}
	GLenum OpenGLTexture::GetTextureWrapData(){
		switch (texSettings.wrappingMode) {
		case TextureWrappingInfo::Repeat:
			return GL_REPEAT;
		case TextureWrappingInfo::Mirrored_Repeat:
			return GL_MIRRORED_REPEAT;
		case TextureWrappingInfo::Clamp:
			return GL_CLAMP_TO_EDGE;
		case TextureWrappingInfo::Clamp_Border:
			return GL_CLAMP_TO_BORDER;
		}

		return 0;
	}
	GLenum OpenGLTexture::GetTextureMaxFilterSettings() {
		switch (texSettings.texMax) {
		case TextureFilterSettings::Linear:
			return GL_LINEAR;
		case TextureFilterSettings::Linear_Mip_Map:
			return GL_LINEAR_MIPMAP_LINEAR;
		case TextureFilterSettings::Near:
			return GL_NEAREST;
		case TextureFilterSettings::Near_Linear_mip:
			return GL_NEAREST_MIPMAP_LINEAR;

		}

		return 0;
	}
	GLenum OpenGLTexture::GetTextureMinFilterSettings()	{

		switch (texSettings.texMin) {
		case TextureFilterSettings::Linear:
			return GL_LINEAR;
		case TextureFilterSettings::Linear_Mip_Map:
			return GL_LINEAR_MIPMAP_LINEAR;
		}
		return 0;
	}
	GLenum OpenGLTexture::GetTextureDataType() {
		switch (texSettings.texDataType) {
		case TextureDataType::Color:
			return GL_RGBA;
		case TextureDataType::Depth:
			return GL_DEPTH_COMPONENT;
		}
		return GLenum();
	}
	GLenum OpenGLTexture::GetTextreSlot() {
		switch (texSettings.texSlot) {
		case TextureSlot::Albedo:
			return GL_TEXTURE0;
		case TextureSlot::Normal:
			return GL_TEXTURE1;
		case TextureSlot::Texture2:
			return GL_TEXTURE2;
		case TextureSlot::Texture3:
			return GL_TEXTURE3;
		case TextureSlot::Texture4:
			return GL_TEXTURE4;
		case TextureSlot::Texture5:
			return GL_TEXTURE5;
		case TextureSlot::Texture6:
			return GL_TEXTURE6;
		case TextureSlot::Texture7:
			return GL_TEXTURE7;
		case TextureSlot::Texture8:
			return GL_TEXTURE8;
		case TextureSlot::Texture9:
			return GL_TEXTURE9;
		case TextureSlot::Texture10:
			return GL_TEXTURE10;
		case TextureSlot::Texture11:
			return GL_TEXTURE11;
		case TextureSlot::Texture12:
			return GL_TEXTURE12;
		case TextureSlot::Texture13:
			return GL_TEXTURE13;
		case TextureSlot::Texture14:
			return GL_TEXTURE14;
		case TextureSlot::Texture15:
			return GL_TEXTURE15;
		case TextureSlot::Texture16:
			return GL_TEXTURE16;

		}
		return GLenum();
	}
	GLenum OpenGLTexture::GetTextureInternal(){
		switch (texSettings.texFormat) {
			case TexFormat::RGBA:
				return GL_RGB;
			break;
			case TexFormat::RGB_INT:
				return GL_RGB32UI;
			break;
		}
	}
	GLenum OpenGLTexture::GetTextureFormat() {
		switch (texSettings.texFormat) {
		case TexFormat::RGBA:
			if (nrChannels==3) {
				return GL_RGB;
			}
			else if (nrChannels ==4) {
				return GL_RGBA;
			}
			break;
		case TexFormat::RGB_INT:
			return GL_RGB_INTEGER;
			break;
		}
	}
	GLenum OpenGLTexture::GetTextureDataInternalType() {
		switch (texSettings.texFormat) {
		case TexFormat::RGBA:
			return GL_UNSIGNED_BYTE;
			break;
		case TexFormat::RGB_INT:
			return GL_UNSIGNED_INT;
			break;
		}
	}
	std::tuple<unsigned int, unsigned int> OpenGLTexture::GetResolution() {
		return {width,height};
	}
	void OpenGLTexture::Resize(unsigned int nWidth, unsigned int nHeight) {
		Bind();
		width = nWidth;
		height = nHeight;
		if (texSettings.texFormat == TexFormat::RGBA) {
			glTexImage2D(GetTextureType(), 0, GetTextureDataType(), width, height, 0, GetTextureDataType(), GL_FLOAT, nullptr);
		}
		else if (texSettings.texFormat == TexFormat::RGB_INT) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
		}
		UnBind();
	}
	void OpenGLTexture::UnBind(){
		glBindTexture(GetTextureType(), 0);
	}
	unsigned int OpenGLTexture::GetId()	{
		return textureId;
	}
	void OpenGLTexture::SetType(std::string tType){

	}
	std::string OpenGLTexture::GetPath()
	{
		return path;
	}
}