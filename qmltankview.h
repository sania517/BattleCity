#ifndef QMLTANKVIEW_H
#define QMLTANKVIEW_H

#include <QAbstractListModel>
#include <array>
#include <QString>
#include "battlecityfield.h"


struct QmlTank
{
    QString m_url;
    short m_dir;
};

class QmlTankView : public QAbstractListModel
{
    Q_OBJECT

public:
    QmlTankView(QObject *parent = Q_NULLPTR);
    enum BattleCityRoles {urlTankRole = Qt::UserRole+1, directTankRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void fillList(BattleCityField * baCiFi);

private:
    short dirRotation(const int) const;
    QHash<int, QByteArray> roleNames() const;
    std::array<QmlTank, 676> m_listTank;
    BattleCityField * m_baCiFi;

public slots:
    void createTankOfPlayerSlot(short y, short x, bool secondPlayer);
    void moveTankOfPlayerSlot(short beginY, short beginX, short endY, short endX, bool secondPlayer);
    void moveTankSlot(short beginY, short beginX, short endY, short endX);
    void deleteTankSlot(short,short);
    void createTankSlot(short,short);
};


#endif // QMLTANKVIEW_H
