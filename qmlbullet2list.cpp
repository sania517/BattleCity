#include "qmlbullet2list.h"



QmlBullet2List::QmlBullet2List(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listBullet_2.size();
    for (int i = 0; i < size; i++){
        m_listBullet_2[i] = QString();
    }

}



QHash<int, QByteArray> QmlBullet2List::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Bullet2Role] = "urlBullet_2";
    return roles;
}

int QmlBullet2List::rowCount(const QModelIndex &) const
{
    return m_listBullet_2.size();
}

QVariant QmlBullet2List::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == Bullet2Role)
        return m_listBullet_2[index.row()];
    return QVariant();
}

void QmlBullet2List::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    for (int i = 0; i < 729;i++){
        m_listBullet_2[i] = QString();
    }
}

void QmlBullet2List::newBullet_2_Slot(short beginY, short beginX)
{
    int begin = beginY*27 + beginX;
    if(auto object = m_baCiFi->getBulletAndFlash(beginY, beginX, 2))
        m_listBullet_2[begin] = object->urlView();
    else
        m_listBullet_2[begin] = QString();
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
}

void QmlBullet2List::moveBullet_2_Slot(short beginY, short beginX, short endY, short endX)
{
    int begin = beginY*27 + beginX;
    int end = endY*27+endX;
     if (beginY == endY && endX < beginX){
            beginMoveRows(QModelIndex(),begin,begin,QModelIndex(),end);
            std::swap(m_listBullet_2[begin],m_listBullet_2[end]);
            endMoveRows();
    }
}
