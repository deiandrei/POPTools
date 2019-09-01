#ifndef FSTM_H
#define FSTM_H

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qvariant.h>

#include "../bf_lib/Archive.h"

class FSItem;

//! [0]
class FSTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FSTreeModel(bf::Archive* arc, QObject *parent = nullptr);
    ~FSTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
	FSItem* getItemAtIndex(const QModelIndex& index);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	void doubleClicked(const QModelIndex& index);

private:
    void setupModelData(FSItem* parent, bf::Archive* arc, int folderId);

    FSItem *rootItem;
};
//! [0]

#endif // FSTreeModel_H