#ifndef QMLBULLET0LIST_H
#define QMLBULLET0LIST_H
#include <QAbstractListModel>
#include <array>
#include <QString>
#include "battlecityfield.h"

class QmlBullet0List : public QAbstractListModel
{
    Q_OBJECT
public:
    QmlBullet0List(QObject *parent = Q_NULLPTR);
    enum Bullet0Roles {Bullet0Role = Qt::UserRole+1};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void fillList(BattleCityField * baCiFi);

private:
    QHash<int, QByteArray> roleNames() const;
    std::array<QString, 729> m_listBullet_0;
    BattleCityField * m_baCiFi;

public slots:
    void newBullet_0_Slot(short beginY, short beginX);
    void moveBullet_0_Slot(short beginY, short beginX, short endY, short endX);
};

#endif // QMLBULLET0LIST_H
