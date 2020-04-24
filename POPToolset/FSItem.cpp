#include "FSItem.h"

FSItem::FSItem(QString nodename, int tag, FSType type, FSItem *parent) : m_parentItem(parent) {
	mNodeName = nodename;

	mTag = tag;
	mType = type;
}

FSItem::~FSItem()
{
    qDeleteAll(m_childItems);
}

void FSItem::appendChild(FSItem *item)
{
    m_childItems.append(item);
}

FSItem *FSItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int FSItem::childCount() const {
    return m_childItems.count();
}

int FSItem::columnCount() const {
    return 1;
}

QVariant FSItem::data(int column) const {
    return mNodeName;
}

FSItem *FSItem::parentItem() {
    return m_parentItem;
}

int FSItem::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<FSItem*>(this));

    return 0;
}
