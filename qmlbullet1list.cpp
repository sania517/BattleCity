#include "qmlbullet1list.h"

QmlBullet1List::QmlBullet1List(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listBullet_1.size();
    for (int i = 0; i < size; i++){
        m_listBullet_1[i] = QString();
    }

}



QHash<int, QByteArray> QmlBullet1List::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Bullet1Role] = "urlBullet_1";
    return roles;
}

int QmlBullet1List::rowCount(const QModelIndex &) const
{
    return m_listBullet_1.size();
}

QVariant QmlBullet1List::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == Bullet1Role)
        return m_listBullet_1[index.row()];
    return QVariant();
}

void QmlBullet1List::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    for (int i = 0; i < 729;i++){
        m_listBullet_1[i] = QString();
    }
}



void QmlBullet1List::newBullet_1_Slot(short beginY, short beginX)
{
    int begin = beginY*27 + beginX;
    if(auto object = m_baCiFi->getBulletAndFlash(beginY, beginX, 1))
        m_listBullet_1[begin] = object->urlView();
    else
        m_listBullet_1[begin] = QString();
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
}

void QmlBullet1List::moveBullet_1_Slot(short beginY, short beginX, short endY, short endX)
{
    int begin = beginY*27 + beginX;
    int end = endY*27+endX;
    if (beginX == endX && beginY > endY){
        beginMoveRows(QModelIndex(),begin,begin,QModelIndex(),end);
        std::swap(m_listBullet_1[begin],m_listBullet_1[end]);
        endMoveRows();
        beginMoveRows(QModelIndex(),end+1,end+1,QModelIndex(),begin+1);
        endMoveRows();
    }
}
