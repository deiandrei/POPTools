#pragma once

#include <QWidget>
#include "ui_FileViewerForm.h"

#include "../bf_lib/Archive.h"
#include "../popbin_lib/BinArchive.h"

class FileViewerForm : public QWidget
{
	Q_OBJECT

	public:
		FileViewerForm(bf::Archive* arc, int id, QWidget *parent = Q_NULLPTR);
		~FileViewerForm();

	public slots:
		void ParseText();

		void ParseRecursive(popbin::Entry* entry);

	protected:
		void UpdateLayout();

	private:
		Ui::FileViewerForm ui;

		bf::Archive* mArchiveRef;
		int mFileID;
		bf::File* mFile;

		popbin::BinArchive* mBinaryArchive;
		std::vector<std::pair<popbin::Entry*, std::string>> EntriesTagsTxt;

};
