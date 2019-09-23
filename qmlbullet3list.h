#ifndef QMLBULLET3LIST_H
#define QMLBULLET3LIST_H
#include <QAbstractListModel>
#include <array>
#include <QString>
#include "battlecityfield.h"

class QmlBullet3List : public QAbstractListModel
{
    Q_OBJECT
public:
    QmlBullet3List(QObject *parent = Q_NULLPTR);
    enum Bullet3Roles {Bullet3Role = Qt::UserRole+1};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void fillList(BattleCityField * baCiFi);

private:
    QHash<int, QByteArray> roleNames() const;
    std::array<QString, 729> m_listBullet_3;
    BattleCityField * m_baCiFi;

public slots:
    void newBullet_3_Slot(short beginY, short beginX);
    void moveBullet_3_Slot(short beginY, short beginX, short endY, short endX);
};

#endif // QMLBULLET3LIST_H
