#include "FSItem.h"

//! [0]
FSItem::FSItem(QString nodename, int tag, FSType type, FSItem *parent) : m_parentItem(parent) {
	mNodeName = nodename;

	mTag = tag;
	mType = type;
}
//! [0]

//! [1]
FSItem::~FSItem()
{
    qDeleteAll(m_childItems);
}
//! [1]

//! [2]
void FSItem::appendChild(FSItem *item)
{
    m_childItems.append(item);
}
//! [2]

//! [3]
FSItem *FSItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}
//! [3]

//! [4]
int FSItem::childCount() const {
    return m_childItems.count();
}
//! [4]

//! [5]
int FSItem::columnCount() const {
    return 1;
}
//! [5]

//! [6]
QVariant FSItem::data(int column) const {
    return mNodeName;
}
//! [6]

//! [7]
FSItem *FSItem::parentItem() {
    return m_parentItem;
}
//! [7]

//! [8]
int FSItem::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<FSItem*>(this));

    return 0;
}
//! [8]