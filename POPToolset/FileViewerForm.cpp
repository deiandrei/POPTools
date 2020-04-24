#include "FileViewerForm.h"

FileViewerForm::FileViewerForm(bf::Archive* arc, int id, QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	mArchiveRef = arc;
	mFileID = id;

	mFile = &mArchiveRef->Files[mFileID];

	std::binarystream bytestream(mFile->data, mFile->data_size);
	mBinaryArchive = new popbin::BinArchive(bytestream);

	UpdateLayout();
}

FileViewerForm::~FileViewerForm() {

}

void FileViewerForm::UpdateLayout() {
	ui.nameLabel->setText(QString(mFile->name));
	ui.sizeLabel->setText(QString::number(mFile->data_size) + " (" + QString::number((float)mFile->data_size / float(1024*1024)) + " MB)");
	ui.typeLabel->setText(QString("NoType"));
	ui.grsIdLabel->setText(QString::number(mArchiveRef->FileTables.bridge_id_grs[mFileID]));
	ui.entriesCountLabel->setText(QString::number((int)mBinaryArchive->Entries.size()));

	// Fill the files list

	for (auto& entry : mBinaryArchive->Entries) {
		std::stringstream str;
		str << "[" << entry.entry_beginPos << "] -> [" << entry.entry_endPos << "] - FileID: " << std::hex << entry.fileID;

		ui.filesList->addItem(new QListWidgetItem(str.str().c_str()));
	}

	connect(ui.tryTextParse, &QPushButton::pressed, this, &FileViewerForm::ParseText);
}

void FileViewerForm::ParseText() {
	ParseRecursive(&mBinaryArchive->Entries[1]);


}

void FileViewerForm::ParseRecursive(popbin::Entry* entry) {
	std::binarystream bs(*entry);

	std::vector<popbin::Entry*> recursiveEntries;

	while ((uint4)bs.tellg() < bs.size()) {
		int4 entryID;
		bs.read((char*)&entryID, sizeof(int4));
		char tag[5];
		bs.read(&tag[0], 4);
		tag[4] = '\0';

		int entryIndex = mBinaryArchive->SearchEntryByID(entryID);
		if (entryIndex != -1) {
			//std::cout << "Found entry " << std::hex << entryID << std::dec << " at index " << entryIndex << "\n";
			EntriesTagsTxt.push_back(std::pair<popbin::Entry*, std::string>(&mBinaryArchive->Entries[entryIndex], tag));

			if(std::string(tag) == ".txg") recursiveEntries.push_back(&mBinaryArchive->Entries[entryIndex]);
		}
		else {
			std::cout << "Entry with ID " << std::hex << entryID << std::dec << " referenced in Entry " << std::hex << entry->fileID << std::dec << " not found!" << "\n";
		}
	}

	for (auto entry : recursiveEntries) {
		ParseRecursive(entry);
	}
}
