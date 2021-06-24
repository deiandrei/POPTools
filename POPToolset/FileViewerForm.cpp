#include "FileViewerForm.h"
#include <QtWidgets/qfiledialog.h>
#include <qmessagebox.h>

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

	for (int i = 0; i < (int)mBinaryArchive->Entries.size(); ++i) {
		auto& entry = mBinaryArchive->Entries[i];

		std::stringstream str;
		str << "[" << entry.entry_beginPos << "] -> [" << entry.entry_endPos << "] --- [" << std::hex << entry.fileID << "] " << entry.name;

		QListWidgetItem* item = new QListWidgetItem(ui.filesList);
		item->setText(str.str().c_str());
		item->setData(Qt::UserRole, i);
		
		ui.filesList->addItem(item);
	}

	connect(ui.filesList, &QListWidget::itemSelectionChanged, [&]() {
		int entryID = ui.filesList->currentItem()->data(Qt::UserRole).toInt();
		auto& entry = mBinaryArchive->Entries[entryID];

		// Get first 4 bytes which shows the entry type
		if (entry.size > 0) {
			char data[5] = { entry.data[0], entry.data[1], entry.data[2], entry.data[3], '\0' };
			int4 idVar;
			memcpy(&idVar, &data[0], sizeof(int4));

			std::stringstream str;
			str << data << " [" << std::hex << idVar << "]";

			ui.entryTypeLabel->setText(str.str().c_str());
		}
		else {
			ui.entryTypeLabel->setText("Empty");
		}

		ui.entrySizeLabel->setText(QString::number(entry.size));
	});

	connect(ui.tryTextParse, &QPushButton::pressed, this, &FileViewerForm::ParseText);

	connect(ui.extractFile, &QPushButton::pressed, [&]() {
		QString path = QFileDialog::getExistingDirectory(0, "Save file", mFile->name);
		byte1* data = mBinaryArchive->GetData();
		uint4 dataSize = mBinaryArchive->GetDataSize();

		if (!path.isEmpty()) {
			std::ofstream writer(path.toStdString() + "/" + mFile->name, std::ios::binary);

			if (!writer.is_open()) return;

			writer.write((char*)(&data[0]), sizeof(byte1) * dataSize);

			writer.close();

			QMessageBox::information(this, "Success!", QString(std::string("Successfully exported " + std::to_string(dataSize) + " bytes of data.").c_str()));
		}
	});
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
