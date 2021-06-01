#include "GltfLoader.h"

#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>

#include <fstream>
#include <map>
#include <optional>

namespace trg
{
	namespace GltfLoaderPrivate
	{
		class StreamReader : public Microsoft::glTF::IStreamReader
		{
		public:
			StreamReader(std::filesystem::path pathBase)
			  : m_pathBase(std::move(pathBase))
			{
			}

			// Resolves the relative URIs of any external resources declared in the glTF manifest
			std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
			{
				// function taken from https://github.com/microsoft/glTF-SDK/blob/master/GLTFSDK.Samples/Deserialize/Source/main.cpp

				// In order to construct a valid stream:
				// 1. The filename argument will be encoded as UTF-8 so use filesystem::u8path to
				//    correctly construct a path instance.
				// 2. Generate an absolute path by concatenating m_pathBase with the specified filename
				//    path. The filesystem::operator/ uses the platform's preferred directory separator
				//    if appropriate.
				// 3. Always open the file stream in binary mode. The glTF SDK will handle any text
				//    encoding issues for us.
				auto streamPath = m_pathBase / filename;
				auto stream = std::make_shared<std::ifstream>(streamPath, std::ios_base::binary);

				// Check if the stream has no errors and is ready for I/O operations
				if(!stream || !(*stream))
				{
					throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);
				}

				return stream;
			}

		private:
			std::filesystem::path m_pathBase;
		};

		BufferBlockComponentType mapEnum(Microsoft::glTF::AccessorType accessorType)
		{
			switch(accessorType)
			{
				case Microsoft::glTF::TYPE_UNKNOWN:
					throw std::runtime_error("TYPE_UNKNOWN is invalid.");
				case Microsoft::glTF::TYPE_SCALAR:
					return BufferBlockComponentType::Scalar;
				case Microsoft::glTF::TYPE_VEC2:
					return BufferBlockComponentType::Vec2;
				case Microsoft::glTF::TYPE_VEC3:
					return BufferBlockComponentType::Vec3;
				case Microsoft::glTF::TYPE_VEC4:
					return BufferBlockComponentType::Vec4;
				case Microsoft::glTF::TYPE_MAT2:
					return BufferBlockComponentType::Mat2;
				case Microsoft::glTF::TYPE_MAT3:
					return BufferBlockComponentType::Mat3;
				case Microsoft::glTF::TYPE_MAT4:
					return BufferBlockComponentType::Mat4;
				default:
					assert(false);
					throw std::runtime_error("Invalid Range");
			}
		}

		BufferBlockValueType mapEnum(Microsoft::glTF::ComponentType componentType)
		{
			switch(componentType)
			{
				case Microsoft::glTF::COMPONENT_UNKNOWN:
					throw std::runtime_error("COMPONENT_UNKNOWN is invalid.");
				case Microsoft::glTF::COMPONENT_BYTE:
				case Microsoft::glTF::COMPONENT_UNSIGNED_BYTE:
				case Microsoft::glTF::COMPONENT_SHORT:
				case Microsoft::glTF::COMPONENT_UNSIGNED_SHORT:
					throw std::runtime_error(fmt::format("Unsuported mapping. Microsoft::glTF::ComponentType: {}", componentType));
				case Microsoft::glTF::COMPONENT_UNSIGNED_INT:
					return BufferBlockValueType::Uint;
				case Microsoft::glTF::COMPONENT_FLOAT:
					return BufferBlockValueType::Float;
				default:
					assert(false);
					throw std::runtime_error("Invalid Range");
			}
		}

		class DataReader
		{
		public:
			DataReader(const Microsoft::glTF::GLTFResourceReader& resourceReader,
					   const Microsoft::glTF::Document& document,
					   const Microsoft::glTF::MeshPrimitive& meshPrimitive)
			  : m_resourceReader(resourceReader)
			  , m_document(document)
			  , m_meshPrimitive(meshPrimitive)
			{
			}

			std::vector<MeshAttribute> readMeshAttributes()
			{
				static std::map<std::string, StandardAttributes> supportedAttributes = {
					{Microsoft::glTF::ACCESSOR_POSITION, StandardAttributes::Position},
					{Microsoft::glTF::ACCESSOR_NORMAL, StandardAttributes::Normal},
					{Microsoft::glTF::ACCESSOR_COLOR_0, StandardAttributes::Color},
					{Microsoft::glTF::ACCESSOR_TEXCOORD_0, StandardAttributes::TexCoords}};

				std::vector<MeshAttribute> attributes;

				for(auto& attribute: supportedAttributes)
				{
					readAttribute(attributes, attribute.first, attribute.second);
				}

				return attributes;
			}

			template <typename T>
			std::optional<std::vector<T>> readData(const std::string& accessorName)
			{
				if(auto accessorId = getAccessorId(accessorName))
				{
					return readAccessor<T>(*accessorId);
				}
				return std::nullopt;
			}

			std::vector<uint16_t> readIndices()
			{
				return readAccessor<uint16_t>(m_meshPrimitive.indicesAccessorId);
			}

			std::unique_ptr<Material> readMaterial()
			{
				const auto& gltgMaterial = m_document.materials.Get(m_meshPrimitive.materialId);
				auto textureId = gltgMaterial.metallicRoughness.baseColorTexture.textureId;

				auto baseColorFactor = gltgMaterial.metallicRoughness.baseColorFactor;
				auto material =
					std::make_unique<Material>(gltgMaterial.name,
											   glm::vec4(baseColorFactor.r, baseColorFactor.g, baseColorFactor.b, baseColorFactor.a));

				if(!textureId.empty())
				{
					const auto& texture = m_document.textures.Get(textureId);

					const auto& image = m_document.images.Get(texture.imageId);

					auto data = m_resourceReader.ReadBinaryData(m_document, image);

					material->setBaseColorTexture(std::make_unique<Image>(image.name, std::move(data)));
				}

				return material;
			}

		private:
			std::optional<std::string> getAccessorId(const std::string& accessorName)
			{
				if(m_meshPrimitive.HasAttribute(accessorName))
				{
					return m_meshPrimitive.GetAttributeAccessorId(accessorName);
				}
				return std::nullopt;
			}

			void readAttribute(std::vector<MeshAttribute>& attributes, const std::string& attributeName, StandardAttributes attributeType)
			{
				if(auto accessorId = getAccessorId(attributeName))
				{
					const auto& accessor = m_document.accessors.Get(*accessorId);

					auto data = m_resourceReader.ReadBinaryData<float>(m_document, accessor);

					auto bufferBlock = BufferBlock(mapEnum(accessor.type), mapEnum(accessor.componentType), attributeType);

					attributes.emplace_back(data, bufferBlock);
				}
			}

			template <typename T>
			std::vector<T> readAccessor(const std::string& accessorId)
			{
				const auto& accessor = m_document.accessors.Get(accessorId);

				return m_resourceReader.ReadBinaryData<T>(m_document, accessor);
			}


			const Microsoft::glTF::GLTFResourceReader& m_resourceReader;
			const Microsoft::glTF::Document& m_document;
			const Microsoft::glTF::MeshPrimitive& m_meshPrimitive;
		};

	}  // namespace GltfLoaderPrivate


	GltfLoader::GltfLoader()
	{
	}

	Model GltfLoader::loadFromFile(const std::filesystem::path& path)
	{
		namespace glTF = Microsoft::glTF;

		// validate path


		auto streamReader = std::make_shared<GltfLoaderPrivate::StreamReader>(path.parent_path());

		auto resourceReader = glTF::GLTFResourceReader(streamReader);

		auto stream = streamReader->GetInputStream(path.filename().string());

		auto bufferIterator = std::istreambuf_iterator(*stream);
		auto fileContent = std::string(bufferIterator, {});

		auto document = glTF::Deserialize(fileContent);

		Model model = Model(path.filename().replace_extension().string());

		for(const auto& mesh: document.meshes.Elements())
		{
			for(const auto& meshPrimitive: mesh.primitives)
			{
				auto dataReader = GltfLoaderPrivate::DataReader(resourceReader, document, meshPrimitive);

				auto attributes = dataReader.readMeshAttributes();

				auto indices = dataReader.readIndices();

				auto material = dataReader.readMaterial();

				model.addMesh(Mesh(mesh.name, std::move(attributes), std::move(indices), std::move(material)));
			}
		}

		return model;
	}
}  // namespace trg