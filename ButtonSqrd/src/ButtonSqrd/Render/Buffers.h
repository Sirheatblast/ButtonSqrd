#pragma once

#include<vector>
#include<string>

#include<glm/glm.hpp>

namespace BtnSqd {
	struct Vertices {
		Vertices(){}

		Vertices(glm::vec3 position,glm::vec2 texCoords):position(position),texCoords(texCoords){}

		glm::vec3 position =glm::vec3();
		glm::vec2 texCoords = glm::vec2();
		glm::vec3 normal = glm::vec3();
		glm::vec3 aTangentCoords = glm::vec3();
		glm::vec3 aBitTangentCoords = glm::vec3();
		glm::ivec4 boneIds = glm::ivec4(-1);
		glm::vec4 vertexWeights = glm::vec4(0.0f);
		int boneCount = 0;
	};

	enum class ShaderDataType {
		None=0,Float,Float2,Float3,Float4,Int,Int2,Int3,Int4,Mat3,Mat4,Bool,IntArray,FloatArray
	};

	static uint32_t GetShaderDataSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float: return sizeof(float);
		case ShaderDataType::Float2: return sizeof(float)*2;
		case ShaderDataType::Float3: return sizeof(float)*3;
		case ShaderDataType::Float4: return sizeof(float)*4;

		case ShaderDataType::Int: return sizeof(int);
		case ShaderDataType::Int2: return sizeof(int) * 2;
		case ShaderDataType::Int3: return sizeof(int) * 3;
		case ShaderDataType::Int4: return sizeof(int) * 4;

		case ShaderDataType::Mat3: return sizeof(glm::mat3);
		case ShaderDataType::Mat4: return sizeof(glm::mat4);

		case ShaderDataType::Bool: return sizeof(bool);
		}

		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, const std::string& name,bool normalized=false):name(name),type(type),size(GetShaderDataSize(type)),offset(0),normalized(normalized){}

		uint32_t GetCompCount()const {
			switch (type) {
			case ShaderDataType::Float:return 1;
			case ShaderDataType::Float2:return 2;
			case ShaderDataType::Float3:return 3;
			case ShaderDataType::Float4:return 4;

			case ShaderDataType::Int:return 1;
			case ShaderDataType::Int2:return 2;
			case ShaderDataType::Int3:return 3;
			case ShaderDataType::Int4:return 4;

			case ShaderDataType::Mat3:return 9;
			case ShaderDataType::Mat4:return 16;

			case ShaderDataType::Bool:return 1;
			}

			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>elements):bufferElements(elements){
			CalcOffsetAndStride();
		}

		inline const uint32_t GetStride() { return stride; }
		inline const std::vector<BufferElement>& Get() { return bufferElements; }

		std::vector<BufferElement>::iterator begin() { return bufferElements.begin();}
		std::vector<BufferElement>::iterator end() { return bufferElements.end(); }
	private:
		void CalcOffsetAndStride() {
			uint32_t offset=0;
			stride = 0;

			for (auto& element : bufferElements) {
				element.offset = offset;
				offset += element.size;
				stride += element.size;
			}
		}

		std::vector<BufferElement> bufferElements;
		uint32_t stride;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer()=default;
		virtual void Bind() const=0;
		virtual void Unbind() const=0;

		virtual void SetLayout(BufferLayout& layout) = 0;
		virtual BufferLayout& GetLayout() = 0;
		virtual void UpdateBuffer(void* nData, size_t size, size_t offset) = 0;

		static VertexBuffer* Create(Vertices* vertices, unsigned int size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual unsigned int GetCount()const = 0;

		static IndexBuffer* Create(unsigned int* indices, unsigned int count);

	};

	class ShaderStorageBuffer {
	public:
		virtual ~ShaderStorageBuffer() = default;		
		virtual void Bind()=0;
		virtual void AttachData(const void* data, size_t dataSize)=0;
		virtual void UpdateData(const void* data, size_t dataSize, size_t offset=0) = 0;

		static ShaderStorageBuffer* Create(unsigned int bindingSlot);
	};
}