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

		std::optional<std::string> getAccessorId(const Microsoft::glTF::MeshPrimitive& meshPrimitive, const std::string& accessorName)
		{
			if(meshPrimitive.HasAttribute(accessorName))
			{
				return meshPrimitive.GetAttributeAccessorId(accessorName);
			}
			return std::nullopt;
		}
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
				if(auto accessorId = GltfLoaderPrivate::getAccessorId(meshPrimitive, glTF::ACCESSOR_POSITION))
				{
					const auto& accessor = document.accessors.Get(*accessorId);

					const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
				}
				if(auto accessorId = GltfLoaderPrivate::getAccessorId(meshPrimitive, glTF::ACCESSOR_NORMAL))
				{
				}
				if (auto accessorId = GltfLoaderPrivate::getAccessorId(meshPrimitive, glTF::ACCESSOR_COLOR_0))
				{
				}
				if (auto accessorId = GltfLoaderPrivate::getAccessorId(meshPrimitive, glTF::ACCESSOR_TEXCOORD_0))
				{
				}
			}
		}
	}
}  // namespace trg