#include "GameActorModel.h"
#include "../BinArchive.h"
#include "TextureInfoModel.h"
#include "TexturePackModel.h"
#include "TextureModel.h"

namespace popbin {
	GameActorModel::GameActorModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;
		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		bb->Position() = 4; // skip the .gao block

		uint4 name_len;
		uint4 unk_flags1, unk_flags2;
		byte1 flags1[4], flags2[4];
		
		bb->Read<uint4>(&unk_flags1);
		bb->Read<uint4>(&unk_flags2);

		bb->ReadArray<byte1>(&flags1[0], 4);
		Has12Floats = ByteBuffer::GetBitFromByte(3, flags1[2]);
		HasParentGao = ByteBuffer::GetBitFromByte(7, flags1[1]);
		
		bb->Read<uint4>(&name_len);
		bb->ReadArray<char>(&entry->name[0], name_len);

		bb->ReadArray<byte1>(&flags2[0], 4);
		AllFloatsTheSame = ByteBuffer::GetBitFromByte(7, flags1[0]);

		bb->Position() += 6; // skip the next flags bytes
		bb->ReadArray<float>(&Matrix[0], 16);

		byte1 data_flags[4];
		bb->ReadArray<byte1>(&data_flags[0], 4);

		int floatsCount = Has12Floats ? 12 : 6;
		for (int i = 0; i < floatsCount; i += 3) {
			float temp1, temp2, temp3;
			bb->Read<float>(&temp1);
			bb->Read<float>(&temp2);
			bb->Read<float>(&temp3);

			Vectors.push_back(vec3(temp1, temp2, temp3));
		}

		if (HasParentGao) {

		}
		else {
			uint4 geometry_fileId, textureInfo_fileId;
			bb->Read<uint4>(&geometry_fileId);
			bb->Read<uint4>(&textureInfo_fileId);

			GeometryEntry = { geometry_fileId, archive->SearchEntryByID(geometry_fileId) };
			TextureEntry = { textureInfo_fileId, archive->SearchEntryByID(textureInfo_fileId) };
		}
	}

	void GameActorModel::Export(const std::string& file) {
		if (GeometryEntry.second == -1 || TextureEntry.second == -1) return;

		BinArchive* archive = mEntry->parentArchive;

		Entry& geometryEntry = archive->Entries[GeometryEntry.second];
		Entry& textureEntry = archive->Entries[TextureEntry.second];

		if (geometryEntry.model == nullptr) geometryEntry.model = new popbin::GameActorModel(&geometryEntry);
		auto geomModel = (GeometryModel*)geometryEntry.model;

		if (textureEntry.type == EntryType::TEXTURE_INFO) {
			auto texInfoModel = (TextureInfoModel*)textureEntry.model;

			int texId = texInfoModel->TextureList[0].second;
			if (texId != -1) {
				auto texModel = (TextureModel*)archive->Entries[texId].model;
				texModel->Export(file + "." + texModel->FileExtension());
			}

			geomModel->Export(file + "/" + std::string(mEntry->name) + ".obj");
		}
		else if(textureEntry.type == EntryType::TEXTURE_PACK) {
			auto texPackModel = (TexturePackModel*)textureEntry.model;

			texPackModel->Export(file);

			geomModel->MaterialExportName = textureEntry.GetFileIdString();
			geomModel->Export(file + "/" + std::string(mEntry->name) + ".obj");
			geomModel->MaterialExportName = "";
		}
		

	}

}
