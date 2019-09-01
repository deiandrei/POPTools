#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qfiledialog.h>
#include "ui_POPToolset.h"

#include <fstream>
#include <vector>

#include "../bf_lib/Archive.h"
#include "FSTreeModel.h"
#include "FileViewerForm.h"

class POPToolset : public QMainWindow {
	Q_OBJECT

	public:
		POPToolset(QWidget *parent = Q_NULLPTR);

	public slots:
		void onCustomContextMenu(const QPoint& point);

	private:
		Ui::POPToolsetClass ui;

		std::string mCurrentFile;

		bf::Archive* mArchive;

};
