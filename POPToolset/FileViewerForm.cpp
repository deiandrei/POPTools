#include "FileViewerForm.h"

FileViewerForm::FileViewerForm(bf::Archive* arc, int id, QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);

	mArchiveRef = arc;
	mFileID = id;

	mFile = &mArchiveRef->Files[mFileID];

	UpdateLayout();
}

FileViewerForm::~FileViewerForm() {

}

void FileViewerForm::UpdateLayout() {
	ui.nameLabel->setText(QString(mFile->name));
	ui.sizeLabel->setText(QString::number(mFile->data_size) + " (" + QString::number((float)mFile->data_size / float(1024*1024)) + " MB)");
	ui.typeLabel->setText(QString("NoType"));
	ui.grsIdLabel->setText(QString::number(mArchiveRef->FileTables.bridge_id_grs[mFileID]));
}
