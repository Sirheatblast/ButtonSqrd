#include "OpenGLFrameBuffer.h"
#include"ButtonSqrd/Core/Logger.h"

namespace BtnSqd {

    BtnSqd::OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferAttatchment frameAttatchment,unsigned int width,unsigned int height): currentTexture(nullptr),renderBufferTexture(nullptr), bufferAttactchment(GetBufferAttatchment(frameAttatchment)), width(width), height(height) {
        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, width, height);

    }

    BtnSqd::OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        delete currentTexture;
        delete renderBufferTexture;
        glDeleteFramebuffers(1, &id);
    }

    void BtnSqd::OpenGLFrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
    }

    void OpenGLFrameBuffer::WBind() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLuint clearValue[3] = { 0, 0, 0 };
        glClearBufferuiv(GL_COLOR, 0, clearValue);
    }

    void BtnSqd::OpenGLFrameBuffer::UnBind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::AttatchTexture(TextureSlot texSlot,TextureSettings texSettings) {
        if (currentTexture != nullptr) {
            delete currentTexture;
        }

        if (bufferAttactchment == GL_DEPTH_ATTACHMENT) {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        currentTexture = Texture::Create(width, height, texSettings);
        currentTexture->Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, bufferAttactchment, GL_TEXTURE_2D, currentTexture->GetId(), 0);
    }


    void OpenGLFrameBuffer::AttatchTexture(TextureSlot texSlot) {
        if (currentTexture != nullptr) {
            delete currentTexture;
        }

        TextureSettings texSettings;
        texSettings.texMax = TextureFilterSettings::Linear;
        texSettings.texMin = TextureFilterSettings::Linear;
        texSettings.texSlot = texSlot;
        switch (bufferAttactchment) {
        case GL_COLOR_ATTACHMENT0:
            texSettings.texDataType = TextureDataType::Color;
            break;
        case GL_DEPTH_ATTACHMENT:
            texSettings.texDataType = TextureDataType::Depth;
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            break;
        }

        currentTexture = Texture::Create(width,height,texSettings);
        currentTexture->Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, bufferAttactchment, GL_TEXTURE_2D, currentTexture->GetId(), 0);
    }

    void BtnSqd::OpenGLFrameBuffer::AttatchRenderBuffer() {  
        glGenRenderbuffers(1, &rbId);
        glBindRenderbuffer(GL_RENDERBUFFER, rbId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbId);

        if (renderBufferTexture != nullptr) {
            delete renderBufferTexture;
        }

        BtnSqd::TextureSettings texSettings;
        texSettings.texMax = TextureFilterSettings::Linear;
        texSettings.texMin = TextureFilterSettings::Linear;
        texSettings.texSlot = TextureSlot::Texture9;
        texSettings.texDataType = TextureDataType::Depth;

        renderBufferTexture = Texture::Create(width, height, texSettings);
        renderBufferTexture->Bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, renderBufferTexture->GetId(), 0);
    }

    //Returns true if the frambuffer is incomplete
    bool BtnSqd::OpenGLFrameBuffer::GetStatus() {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE;
    }

    GLenum BtnSqd::OpenGLFrameBuffer::GetBufferAttatchment(FrameBufferAttatchment frameAttatchment){
        switch (frameAttatchment) {
            case FrameBufferAttatchment::ColorAttatchment:
                return GL_COLOR_ATTACHMENT0;
            break;
            case FrameBufferAttatchment::DepthAttatchment:
                return GL_DEPTH_ATTACHMENT;
            break;
            case FrameBufferAttatchment::StencilAttatchment:
                return GL_STENCIL_ATTACHMENT;
            break;
            case FrameBufferAttatchment::DepthStencilAttatchment:
                return GL_DEPTH_STENCIL_ATTACHMENT;
            break;
        }
        return GL_COLOR_ATTACHMENT0;
    }

    GLenum OpenGLFrameBuffer::GetBufferFormat(GLenum bAttactchment) {
        switch (bAttactchment) {
        case GL_COLOR_ATTACHMENT0:
            return GL_RGBA;
        case GL_DEPTH_ATTACHMENT:
            return GL_DEPTH_COMPONENT;
        }
        return GL_RGBA;
    }

    GLenum OpenGLFrameBuffer::GetBufferInternalFormat(GLenum bAttactchment) {
        switch (bAttactchment) {
        case GL_COLOR_ATTACHMENT0:
            return GL_RGBA8;
        case GL_DEPTH_ATTACHMENT:
            return GL_DEPTH_COMPONENT24;
        }
        return GL_RGBA8;
    }

    GLenum BtnSqd::OpenGLFrameBuffer::GetTextureType(TextureType texType) {
        switch (texType) {
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

    GLenum BtnSqd::OpenGLFrameBuffer::GetTextureArrayType(TextureType texType) {
        switch (texType) {
        case TextureType::Texture_1D:
            return GL_TEXTURE_1D_ARRAY;
            break;
        case TextureType::Texture_2D:
            return GL_TEXTURE_2D_ARRAY;
            break;
        case TextureType::Texture_3D:
            return GL_TEXTURE_3D;
            break;
        case TextureType::CubeMap:
            return GL_TEXTURE_CUBE_MAP_ARRAY;
            break;
        }
        return 0;
    }

    void OpenGLFrameBuffer::ABind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, width, height);
    }

    void OpenGLFrameBuffer::SetResolution(int nWidth, int nHeight) {
        width = nWidth;
        height = nHeight;
        if (currentTexture) {
            currentTexture->Resize(width, height);
        }
    }

    std::pair<int, int> BtnSqd::OpenGLFrameBuffer::GetResolution() {
        return std::make_pair(width, height);
    }

    //Frist element is the GameObject Id, second is the draw Id, third is the primitive Id
    std::tuple<unsigned int, unsigned int, unsigned int> OpenGLFrameBuffer::GetPixelInformation(int positionX, int positionY) {
        glm::uvec3 data = glm::uvec3();
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        glReadBuffer(bufferAttactchment);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(positionX, positionY, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &data);
        glReadBuffer(GL_NONE);

        return std::make_tuple(data[0],data[1],data[2]);
    }

    void BtnSqd::OpenGLFrameBuffer::GenerateCubeMapArray(unsigned int mapTexture,unsigned int index,unsigned int arraySize) {
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, mapTexture);
        glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY,1,GL_DEPTH_COMPONENT,width,height,6*arraySize);
        glFramebufferTextureLayer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, mapTexture, 0, index);
    }

    void BtnSqd::OpenGLFrameBuffer::GenerateCubeMap(TextureType texType, unsigned int mapTexture) {
        glBindTexture(GetTextureType(texType), mapTexture);
    }

    std::vector<unsigned int>& BtnSqd::OpenGLFrameBuffer::GetTextureArrays() {
        return textureArrays;
    }

    void BtnSqd::OpenGLFrameBuffer::ClearTextureArrays() {
        for (int i = 0; i < textureArrays.size(); i++) {
            glDeleteTextures(1, &textureArrays[i]);
        }
        textureArrays.clear();
    }

    void BtnSqd::OpenGLFrameBuffer::AttatchTextureArray(TextureType texType, unsigned int arraySize) {

        if (bufferAttactchment == GL_DEPTH_ATTACHMENT) {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        unsigned int textureArray;
        glGenTextures(1, &textureArray);
        glBindTexture(GetTextureArrayType(texType), textureArray);

        if (GetTextureArrayType(texType) == GL_TEXTURE_CUBE_MAP_ARRAY) {
            glTexImage3D(GetTextureArrayType(texType), 0, GetBufferInternalFormat(bufferAttactchment), width, height, 6 * arraySize, 0, GetBufferFormat(bufferAttactchment), GL_FLOAT, nullptr);
        }
        else {
            glTexImage3D(GetTextureArrayType(texType), 0, GetBufferInternalFormat(bufferAttactchment), width, height, arraySize, 0, GetBufferFormat(bufferAttactchment), GL_FLOAT, nullptr);
        }
        glFramebufferTexture(GL_FRAMEBUFFER, bufferAttactchment, textureArray, 0);

        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if (bufferAttactchment == GL_DEPTH_ATTACHMENT) {
            glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_COMPARE_FUNC, GL_LESS);
        }
        else {
            glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_COMPARE_MODE, GL_NONE);
        }

        textureArrays.push_back(textureArray);
    }

    void BtnSqd::OpenGLFrameBuffer::AttatchTextureArray(TextureType texType, unsigned int arraySize, unsigned int nWidth,unsigned int nHeight) {

        if (bufferAttactchment == GL_DEPTH_ATTACHMENT) {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        unsigned int textureArray;
        glGenTextures(1, &textureArray);
        glBindTexture(GetTextureArrayType(texType), textureArray);

        if (GetTextureArrayType(texType) == GL_TEXTURE_CUBE_MAP_ARRAY) {
            glTexImage3D(GetTextureArrayType(texType), 0, GetBufferInternalFormat(bufferAttactchment), nWidth, nHeight, 6 * arraySize, 0, GetBufferFormat(bufferAttactchment), GL_FLOAT, nullptr);
        }
        else {
            glTexImage3D(GetTextureArrayType(texType), 0, GetBufferInternalFormat(bufferAttactchment), nHeight, nHeight, arraySize, 0, GetBufferFormat(bufferAttactchment), GL_FLOAT, nullptr);
        }

        for (unsigned int layer = 0; layer < arraySize; layer++) {
            glFramebufferTextureLayer(GL_FRAMEBUFFER, bufferAttactchment, textureArray, 0, layer);
        }

        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GetTextureArrayType(texType), GL_TEXTURE_COMPARE_FUNC, GL_LESS);

        textureArrays.push_back(textureArray);
    }

    unsigned int BtnSqd::OpenGLFrameBuffer::GetId() {
        return id;
    }

    BtnSqd::Texture* BtnSqd::OpenGLFrameBuffer::GetTexture() {
        return currentTexture;
    }

    BtnSqd::Texture* BtnSqd::OpenGLFrameBuffer::GetDepthTexture() {
        return renderBufferTexture;
    }
}
