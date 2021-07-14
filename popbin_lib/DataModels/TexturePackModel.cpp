#include "TexturePackModel.h"
#include "../BinArchive.h"
#include "TextureInfoModel.h"
#include "TextureModel.h"

namespace popbin {
	TexturePackModel::TexturePackModel(Entry* entry) : AbstractModel(entry) {
		BinArchive* archive = entry->parentArchive;

		ByteBuffer* bb = new ByteBuffer(entry->data, entry->size);

		uint4 texCount = 0;

		bb->Position() += 8;
		bb->Read<uint4>(&texCount);

		for (int i = 0; i < texCount; ++i) {
			uint4 texId;
			bb->Read<uint4>(&texId);

			uint4 texId_search = archive->SearchEntryByID(texId);
			if (texId_search != -1) TextureList.insert({ texId, texId_search });
		}

		std::cout << "Texture Pack " << std::hex << mEntry->fileID << std::dec << " has " << TextureList.size() << "/" << texCount << " textures inside.\n";
	}

	void TexturePackModel::Export(const std::string& file) {
		std::stringstream ss1;
		ss1 << "mtl_" << mEntry->GetFileIdString() << ".mtl";

		std::ofstream out(file + "/" + ss1.str());

		out << "# POPBin mtl file\n";

		int i = 0;

		for (auto tex : TextureList) {
			if (tex.second != -1) {
				auto texInfoEntry = mEntry->parentArchive->Entries[tex.second];
				TextureInfoModel* infomodel = (TextureInfoModel*)texInfoEntry.model;

				if (infomodel->TextureList[0].second == -1) continue;

				auto texEntry = mEntry->parentArchive->Entries[infomodel->TextureList[0].second];
				TextureModel* model = (TextureModel*)texEntry.model;

				out << "newmtl " << mEntry->GetFileIdString() << "_" << i << "\n";
				out << "Ka 1.00000 1.00000 1.00000\n";
				out << "Kd 1.00000 1.00000 1.00000\n";
				out << "Ks- 0.00000 0.00000 0.00000\n";
				out << "Ns 0.00000\n";
				out << "map_Kd .\\" << texEntry.GetFileIdString() << "." << model->FileExtension() << "\n\n";

				std::stringstream ss;
				ss << texEntry.GetFileIdString() << "." << model->FileExtension();
				model->Export(file + "/" + ss.str());
			}
			i++;
		}
	}

}
