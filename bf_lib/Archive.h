#pragma once

#include <filesystem>

#include "File.h"
#include "FileTable.h"
#include "Folder.h"
#include "ResourcesTable.h"

namespace bf {

	class Archive {
		public:
			Archive(const std::string& fileName);
			~Archive();

			bool IsLoaded() { return mIsLoaded; }

			std::string FullFolderPath(int id);

			void ExtractFile(int id, const std::string& savePath);
			void ExtractFolder(int id, const std::string& savePath);

		public:
			FileList Files;
			FolderList Folders;
			FileTable FileTables;
			ResourcesTable ResourcesTables;

		protected:
			Header mHeader;
			bool mIsLoaded;

		private:
			void LinkIds(bf::Header& header, bf::ResourcesTable& resTable, bf::FileTable& fileTable);
			void ParseFilesData(std::ifstream& in, bf::Header& header, bf::FileTable& fileTable, bf::ResourcesTable& resTable, std::vector<bf::File>& files);

	};

}
