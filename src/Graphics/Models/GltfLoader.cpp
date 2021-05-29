#include "GltfLoader.h"

#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>
#include <GLTFSDK/GLTFResourceReader.h>

#include <fstream>
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

			std::optional<std::string> getAccessorId(const std::string& accessorName)
			{
				if(m_meshPrimitive.HasAttribute(accessorName))
				{
					return m_meshPrimitive.GetAttributeAccessorId(accessorName);
				}
				return std::nullopt;
			}

			template <typename T>
			std::optional<std::vector<T>> readData(const std::string& accessorName)
			{
				if(auto accessorId = getAccessorId(accessorName))
				{
					const auto& accessor = m_document.accessors.Get(*accessorId);

					return m_resourceReader.ReadBinaryData<T>(m_document, accessor);
				}
				return std::nullopt;
			}

		private:
			const Microsoft::glTF::GLTFResourceReader& m_resourceReader;
			const Microsoft::glTF::Document& m_document;
			const Microsoft::glTF::MeshPrimitive& m_meshPrimitive;
		};

	}  // namespace GltfLoaderPrivate


	GltfLoader::GltfLoader()
	{
	}

	void GltfLoader::loadFromFile(const std::filesystem::path& path)
	{
		namespace glTF = Microsoft::glTF;

		// validate path


		auto streamReader = std::make_shared<GltfLoaderPrivate::StreamReader>(path.parent_path());

		auto resourceReader = glTF::GLTFResourceReader(streamReader);

		auto stream = streamReader->GetInputStream(path.filename().string());

		auto bufferIterator = std::istreambuf_iterator(*stream);
		auto fileContent = std::string(bufferIterator, {});

		auto document = glTF::Deserialize(fileContent);

		for(const auto& mesh: document.meshes.Elements())
		{
			for(const auto& meshPrimitive: mesh.primitives)
			{
				auto dataReader = GltfLoaderPrivate::DataReader(resourceReader, document, meshPrimitive);

				auto positionData = dataReader.readData<float>(glTF::ACCESSOR_POSITION);
				auto normalData = dataReader.readData<float>(glTF::ACCESSOR_NORMAL);
				auto colorData = dataReader.readData<float>(glTF::ACCESSOR_COLOR_0);
				auto textCoordData = dataReader.readData<float>(glTF::ACCESSOR_TEXCOORD_0);
			}
		}
	}
}  // namespace trg