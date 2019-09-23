#include "qmlbullet3list.h"

QmlBullet3List::QmlBullet3List(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listBullet_3.size();
    for (int i = 0; i < size; i++){
        m_listBullet_3[i] = QString();
    }

}



QHash<int, QByteArray> QmlBullet3List::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Bullet3Role] = "urlBullet_3";
    return roles;
}

int QmlBullet3List::rowCount(const QModelIndex &) const
{
    return m_listBullet_3.size();
}

QVariant QmlBullet3List::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == Bullet3Role)
        return m_listBullet_3[index.row()];
    return QVariant();
}

void QmlBullet3List::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    for (int i = 0; i < 729;i++){
        m_listBullet_3[i] = QString();
    }
}



void QmlBullet3List::newBullet_3_Slot(short beginY, short beginX)
{
    int begin = beginY*27 + beginX;
    if(auto object = m_baCiFi->getBulletAndFlash(beginY, beginX, 3))
        m_listBullet_3[begin] = object->urlView();
    else
        m_listBullet_3[begin] = QString();
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
}

void QmlBullet3List::moveBullet_3_Slot(short beginY, short beginX, short endY, short endX)
{
    int begin = beginY*27 + beginX;
    int end = endY*27+endX;
    if (beginX == endX && beginY < endY){
            beginMoveRows(QModelIndex(),end,end,QModelIndex(),begin);
            std::swap(m_listBullet_3[begin],m_listBullet_3[end]);
            endMoveRows();
            beginMoveRows(QModelIndex(),begin+1,begin+1,QModelIndex(),end+1);
            endMoveRows();
    }
}
