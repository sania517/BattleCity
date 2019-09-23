#ifndef NUMBEROFTANKS_H
#define NUMBEROFTANKS_H

#include <QAbstractListModel>
#include "battlecityfield.h"


class NumberOfTanks : public QAbstractListModel
{
    Q_OBJECT
public:
    enum numberOfTanksRoles {numberOfTanksRole = Qt::UserRole+1};

    NumberOfTanks(QObject *parent = Q_NULLPTR);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void fillList(BattleCityField * baCiFi);

private:
    int point;
    QHash<int, QByteArray> roleNames() const;
    std::array<QString, 20> m_listNumberOfTanks;
    BattleCityField * m_baCiFi;

public slots:
void createNumberOfTankSlot();
//    void moveBullet_0_Slot(short beginY, short beginX, short endY, short endX);
};


#endif // NUMBEROFTANKS_H
