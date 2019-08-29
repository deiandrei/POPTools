#ifndef FSItem_H
#define FSItem_H

#include <QVariant>
#include <QVector>

enum FSType { FS_FOLDER, FS_FILE };

class FSItem {
	public:
		explicit FSItem(QString nodename, int tag, FSType type, FSItem *parentItem = nullptr);
		~FSItem();

		FSType Type() { return mType; }
		int Tag() { return mTag; }

		void appendChild(FSItem *child);

		FSItem *child(int row);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		int row() const;
		FSItem *parentItem();

	private:
		QVector<FSItem*> m_childItems;
		QString mNodeName;
		FSItem *m_parentItem;
		FSType mType;
		int mTag;

};


#endif