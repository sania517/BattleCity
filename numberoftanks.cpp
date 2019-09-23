#include "numberoftanks.h"

NumberOfTanks::NumberOfTanks(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listNumberOfTanks.size() ;
    for (int i=0; i < size; ++i){
        m_listNumberOfTanks[i] = QString();
    }
}


QHash<int, QByteArray> NumberOfTanks::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[numberOfTanksRole] = "numberOfTanks";
    return roles;
}

int NumberOfTanks::rowCount(const QModelIndex &) const
{
    return m_listNumberOfTanks.size() ;
}

QVariant NumberOfTanks::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == numberOfTanksRole)
        return m_listNumberOfTanks[index.row()];
    return QVariant();

}

void NumberOfTanks::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    point = m_baCiFi->getTankCount();
    for (int i = 0; i < point;i++){
        m_listNumberOfTanks[i] = "image/count tank.png";
    }
}

void NumberOfTanks::createNumberOfTankSlot()
{
    qDebug() << "createNumberOfTankSlot "<< point;
    m_listNumberOfTanks[point-1] = QString();
    QModelIndex index = createIndex(point-1,0);
    --point;
    emit dataChanged(index, index);
}


