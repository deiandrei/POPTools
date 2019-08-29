#include "POPToolset.h"
#include "FSItem.h"

POPToolset::POPToolset(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	mCurrentFile = "";
	mArchive = nullptr;

	connect(ui.loadBtn, &QPushButton::clicked, [&]() {
		QString path = QFileDialog::getOpenFileName(0, "Load archive", "", "BF files (*.bf)");

		if (!path.isEmpty()) {
			//now set the label
			ui.currentFileLabel->setText(path);
			mCurrentFile = path.toStdString();

			if (mArchive) delete mArchive;
			mArchive = new bf::Archive(mCurrentFile);

			if (mArchive->IsLoaded()) {
				//finished with bf reading
				FSTreeModel* fs_model = new FSTreeModel(mArchive);
				ui.fsTree->setModel(fs_model);
			}
			else delete mArchive;
		}
	});

	ui.fsTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.fsTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));
}

void POPToolset::onCustomContextMenu(const QPoint& point) {
	QModelIndex index = ui.fsTree->indexAt(point);
	
	if (index.isValid()) {
		FSItem* item = static_cast<FSTreeModel*>(ui.fsTree->model())->getItemAtIndex(index);

		QMenu* contextMenu = new QMenu(ui.fsTree);

		QAction* extractAction = new QAction("Extract", contextMenu); contextMenu->addAction(extractAction);
		connect(extractAction, &QAction::triggered, [&]() {
			QString path = QFileDialog::getExistingDirectory(0, "Save file", item->data(0).toString());

			if (!path.isEmpty()) {
				if (item->Type() == FSType::FS_FILE) {
					mArchive->ExtractFile(item->Tag(), path.toStdString());
				}
				else if (item->Type() == FSType::FS_FOLDER) {
					mArchive->ExtractFolder(item->Tag(), path.toStdString());
				}
			}
		});

		contextMenu->exec(mapToGlobal(point));
	}
}
