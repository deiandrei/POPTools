#include "FileViewerForm.h"
#include <QtWidgets/qfiledialog.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qprocess.h>
#include <qlineedit.h>

#include "../popbin_lib/DataModels/GeometryModel.h"
#include "../popbin_lib/DataModels/TextureModel.h"
#include "../popbin_lib/DataModels/TextureInfoModel.h"
#include "../popbin_lib/DataModels/TexturePackModel.h"
#include "../popbin_lib/DataModels/GameActorModel.h"

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
	delete mBinaryArchive;
}

void FileViewerForm::UpdateLayout() {
	ui.nameLabel->setText(QString(mFile->name));
	ui.sizeLabel->setText(QString::number(mBinaryArchive->GetBuffer()->Size()) + " (" + QString::number((float)mBinaryArchive->GetBuffer()->Size() / float(1024*1024)) + " MB)");
	ui.typeLabel->setText(QString("NoType"));
	ui.grsIdLabel->setText(QString::number(mArchiveRef->FileTables.bridge_id_grs[mFileID]));
	ui.entriesCountLabel->setText(QString::number((int)mBinaryArchive->Entries.size()));

	//mBinaryArchive->TryParseLinksHeader();

	// Fill the files list

	for (int i = 0; i < (int)mBinaryArchive->Entries.size(); ++i) {
		auto& entry = mBinaryArchive->Entries[i];

		int strrr = strlen(entry.name);

		std::stringstream str;
		str << "[" << entry.entry_beginPos << "] -> [" << entry.entry_endPos << "] " << entry.GetDebugName(i);

		QListWidgetItem* item = new QListWidgetItem(ui.filesList);
		item->setText(str.str().c_str());
		item->setData(Qt::UserRole, i);
		
		ui.filesList->addItem(item);
	}

	connect(ui.filesList, &QListWidget::itemSelectionChanged, [&]() {
		ui.listWidget->clear();

		int entryID = ui.filesList->currentItem()->data(Qt::UserRole).toInt();
		auto& entry = mBinaryArchive->Entries[entryID];

		ui.entryTypeLabel->setText(popbin::BinArchive::EntryTypeToString(entry.type).c_str());

		ui.entrySizeLabel->setText(QString::number(entry.size));

		if (entry.type == popbin::EntryType::GAO) {
			auto model = (popbin::GameActorModel*)entry.model;

			ui.listWidget->addItem("Has 12 floats?: " + QString::number(model->Has12Floats));
			ui.listWidget->addItem("Has all same floats?: " + QString::number(model->AllFloatsTheSame));
			ui.listWidget->addItem("Has paarent gao?: " + QString::number(model->HasParentGao));

			for (int i = 0; i < (int)model->Vectors.size(); ++i) {
				ui.listWidget->addItem("Vector " + QString::number(i) + ".x: " + QString::number(model->Vectors[i].x));
				ui.listWidget->addItem("Vector " + QString::number(i) + ".y: " + QString::number(model->Vectors[i].y));
				ui.listWidget->addItem("Vector " + QString::number(i) + ".z: " + QString::number(model->Vectors[i].z));
			}

			ui.listWidget->addItem("Geometry file " + QString::number(model->GeometryEntry.second));
			ui.listWidget->addItem("Texture file " + QString::number(model->TextureEntry.second));
		}
		else if (entry.type == popbin::EntryType::TEXTURE) {
			auto model = (popbin::TextureModel*)entry.model;

			int palleteid = -1;
			if (model->PaletteID != -1) palleteid = mBinaryArchive->SearchEntryByIDandType(model->PaletteID, popbin::EntryType::TEXTURE_PALETTE);

			ui.listWidget->addItem("Width: " + QString::number(model->Width));
			ui.listWidget->addItem("Height: " + QString::number(model->Height));
			ui.listWidget->addItem("Type: " + QString(model->FileExtension().c_str()));
			ui.listWidget->addItem("Palette: " + QString::number(palleteid));
		}
		else if (entry.type == popbin::EntryType::GEOMETRY) {
			if (entry.model == nullptr) entry.model = new popbin::GeometryModel(&entry);
			auto model = (popbin::GeometryModel*)entry.model;

			ui.listWidget->addItem("Vertices: " + QString::number(model->vertices.size()));
			ui.listWidget->addItem("Mesh Count: " + QString::number(model->mesh_count));
			for (int i = 0; i < model->mesh_count; ++i) {
				ui.listWidget->addItem("Material ID [" + QString::number(i) + "]: " + QString::number(model->meshparts[i].materialId));
				ui.listWidget->addItem("Indices Count [" + QString::number(i) + "]: " + QString::number(model->meshparts[i].indices.size()));
			}
			ui.listWidget->addItem("Has Normals: " + QString::number(model->HasNormals));
			ui.listWidget->addItem("Has Colors: " + QString::number(model->HasNormals));
		}
		else if (entry.type == popbin::EntryType::TEXTURE_INFO) {
			auto model = (popbin::TextureInfoModel*)entry.model;

			ui.listWidget->addItem("Texture Count: " + QString::number(model->TextureList.size()));
			for (int i = 0; i < (int)model->TextureList.size(); ++i) {
				ui.listWidget->addItem("Texture " + QString::number(i) + ": " + QString::number(model->TextureList[i].second));
			}

		}
		else if (entry.type == popbin::EntryType::TEXTURE_PACK) {
			auto model = (popbin::TexturePackModel*)entry.model;

			int i = 0;
			ui.listWidget->addItem("Texture Count: " + QString::number(model->TextureList.size()));
			for (auto tex : model->TextureList) {
				ui.listWidget->addItem("Texture " + QString::number(i) + ": " + QString::number(tex.second));

				i++;
			}

		}

	});

	connect(ui.tryTextParse, &QPushButton::pressed, this, &FileViewerForm::ParseText);

	connect(ui.extractFile, &QPushButton::pressed, [&]() {
		QString path = QFileDialog::getExistingDirectory(0, "Save file", mFile->name);
		byte1* data = mBinaryArchive->GetBuffer()->Data();
		uint4 dataSize = mBinaryArchive->GetBuffer()->Size();

		if (!path.isEmpty()) {
			std::ofstream writer(path.toStdString() + "/" + mFile->name, std::ios::binary);

			if (!writer.is_open()) return;

			writer.write((char*)(&data[0]), sizeof(byte1) * dataSize);

			writer.close();

			QMessageBox::information(this, "Success!", QString(std::string("Successfully exported " + std::to_string(dataSize) + " bytes of data.").c_str()));
		}
	});

	connect(ui.extractEntries, &QPushButton::pressed, [&]() {
		QString path = QFileDialog::getExistingDirectory(0, "Save files");

		if (!path.isEmpty()) {
			mBinaryArchive->ExtractAllEntries(path.toStdString());
		}
	});

	connect(ui.geometryExtractBtn, &QPushButton::pressed, [&]() {
		int entryID = ui.filesList->currentItem()->data(Qt::UserRole).toInt();
		auto& entry = mBinaryArchive->Entries[entryID];

		QString path = QFileDialog::getExistingDirectory(0, "Save model");

		if (entry.type == popbin::EntryType::TEXTURE) {
			auto model = static_cast<popbin::TextureModel*>(entry.model);

			std::string format = model->Type == popbin::TextureImageType::TEXTURE_DDS ? ".dds" : ".tga";

			model->Export(path.toStdString() + "/image_" + std::to_string(entryID));
			return;
		}

		if (entry.type == popbin::EntryType::GAO) {
			auto model = static_cast<popbin::GameActorModel*>(entry.model);

			model->Export(path.toStdString());
			return;
		}

		if (entry.type == popbin::EntryType::TEXTURE_PACK) {
			auto model = static_cast<popbin::TexturePackModel*>(entry.model);

			model->Export(path.toStdString());
			return;
		}

		if (entry.type != popbin::EntryType::GEOMETRY) return;

		if (!path.isEmpty()) {
			auto modelPtr = static_cast<popbin::GeometryModel*>(entry.model);
			if (modelPtr) {
				modelPtr->Export(path.toStdString() + "/" + std::to_string(entryID) + "_model.obj");
			}
		}
	});

	connect(ui.openInHexBtn, &QPushButton::pressed, [&]() {
		int entryID = ui.filesList->currentItem()->data(Qt::UserRole).toInt();
		auto& entry = mBinaryArchive->Entries[entryID];

		const QString hxdPath = "C:\\Program Files\\HxD\\HxD.exe";

		QProcess* process = new QProcess(this);
		
		connect(process, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
		{
			std::cout << "error enum val = " << error << "\n";
		});

		process->startDetached(hxdPath);
	});

	connect(ui.searchInput, &QLineEdit::textChanged, [&](QString text) {
		for (int i = 0; i < ui.filesList->count(); ++i) {
			auto item = ui.filesList->item(i);

			if (text.isEmpty()) {
				item->setHidden(false);
			}
			else {
				item->setHidden(!item->text().contains(text));
			}
		}
	});
}

void FileViewerForm::ParseText() {
	ParseRecursive(&mBinaryArchive->Entries[ui.filesList->currentItem()->data(Qt::UserRole).toInt()]);
}

void FileViewerForm::ParseRecursive(popbin::Entry* entry) {
	//std::binarystream bs(*entry);

	//std::vector<popbin::Entry*> recursiveEntries;

	//while ((uint4)bs.tellg() < bs.size()) {
	//	int4 entryID;
	//	bs.read((char*)&entryID, sizeof(int4));
	//	char tag[5];
	//	bs.read(&tag[0], 4);
	//	tag[4] = '\0';

	//	int entryIndex = mBinaryArchive->SearchEntryByID(entryID);
	//	if (entryIndex != -1) {
	//		//std::cout << "Found entry " << std::hex << entryID << std::dec << " at index " << entryIndex << "\n";
	//		EntriesTagsTxt.push_back(std::pair<popbin::Entry*, std::string>(&mBinaryArchive->Entries[entryIndex], tag));

	//		if(std::string(tag) == ".txg") recursiveEntries.push_back(&mBinaryArchive->Entries[entryIndex]);
	//	}
	//	else {
	//		std::cout << "Entry with ID " << std::hex << entryID << std::dec << " referenced in Entry " << std::hex << entry->fileID << std::dec << " not found!" << "\n";
	//	}
	//}

	//for (auto entry : recursiveEntries) {
	//	ParseRecursive(entry);
	//}

	int2 x;
}
