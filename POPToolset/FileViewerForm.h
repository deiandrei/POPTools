#pragma once

#include <QWidget>
#include "ui_FileViewerForm.h"

#include "../bf_lib/Archive.h"

class FileViewerForm : public QWidget
{
	Q_OBJECT

	public:
		FileViewerForm(bf::Archive* arc, int id, QWidget *parent = Q_NULLPTR);
		~FileViewerForm();

	protected:
		void UpdateLayout();

	private:
		Ui::FileViewerForm ui;

		bf::Archive* mArchiveRef;
		int mFileID;
		bf::File* mFile;

};
