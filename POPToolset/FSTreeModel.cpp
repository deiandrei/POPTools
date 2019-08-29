#include "FSTreeModel.h"
#include "FSItem.h"

FSTreeModel::FSTreeModel(bf::Archive* arc, QObject *parent) : QAbstractItemModel(parent) {
    rootItem = new FSItem("Root", 0, FSType::FS_FOLDER);
    setupModelData(rootItem, arc, 0);
}

FSTreeModel::~FSTreeModel()
{
    delete rootItem;
}

int FSTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<FSItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant FSTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    FSItem *item = static_cast<FSItem*>(index.internalPointer());

    return item->data(index.column());
}

FSItem* FSTreeModel::getItemAtIndex(const QModelIndex& index) {
	if (!index.isValid())
		return nullptr;

	FSItem* item = static_cast<FSItem*>(index.internalPointer());

	return item;
}

Qt::ItemFlags FSTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant FSTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex FSTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FSItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FSItem*>(parent.internalPointer());

    FSItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex FSTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FSItem *childItem = static_cast<FSItem*>(index.internalPointer());
    FSItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FSTreeModel::rowCount(const QModelIndex &parent) const
{
    FSItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FSItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void FSTreeModel::setupModelData(FSItem* parent, bf::Archive* arc, int folderId) {
	for (size_t i = 0; i < arc->Folders.size(); ++i) {
		if (arc->Folders[i].parentFolder == folderId) {
			FSItem* item = new FSItem(arc->Folders[i].name, i, FSType::FS_FOLDER, parent);
			parent->appendChild(item);

			setupModelData(item, arc, i);
		}
	}

	for (size_t i = 0; i < arc->Files.size(); ++i) {
		if (arc->Files[i].folder == folderId) {
			FSItem* item = new FSItem(arc->Files[i].name, i, FSType::FS_FILE, parent);
			parent->appendChild(item);
		}
	}
}