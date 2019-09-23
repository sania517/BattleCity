#include "qmlbullet0list.h"

QmlBullet0List::QmlBullet0List(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listBullet_0.size();
    for (int i = 0; i < size; i++){
        m_listBullet_0[i] = QString();
    }

}



QHash<int, QByteArray> QmlBullet0List::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Bullet0Role] = "urlBullet_0";
    return roles;
}

int QmlBullet0List::rowCount(const QModelIndex &) const
{
    return m_listBullet_0.size();
}

QVariant QmlBullet0List::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == Bullet0Role)
        return m_listBullet_0[index.row()];
    return QVariant();

}

void QmlBullet0List::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    for (int i = 0; i < 729;i++){
        m_listBullet_0[i] = QString();
    }
}

void QmlBullet0List::newBullet_0_Slot(short beginY, short beginX)
{
    int begin = beginY*27 + beginX;
    if(auto object = m_baCiFi->getBulletAndFlash(beginY, beginX, 0))
        m_listBullet_0[begin] = object->urlView();
    else
        m_listBullet_0[begin] = QString();
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
}

void QmlBullet0List::moveBullet_0_Slot(short beginY, short beginX, short endY, short endX)
{
    int begin = beginY*27 + beginX;
    int end = endY*27+endX;

     if (beginY == endY && endX > beginX){
            beginMoveRows(QModelIndex(),end,end,QModelIndex(),begin);
            std::swap(m_listBullet_0[end],m_listBullet_0[begin]);
            endMoveRows();
    }
}
