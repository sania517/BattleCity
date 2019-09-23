#ifndef QMLBULLET1LIST_H
#define QMLBULLET1LIST_H
#include <QAbstractListModel>
#include <array>
#include <QString>
#include "battlecityfield.h"

class QmlBullet1List : public QAbstractListModel
{
    Q_OBJECT
public:
    QmlBullet1List(QObject *parent = Q_NULLPTR);
    enum Bullet1Roles {Bullet1Role = Qt::UserRole+1};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void fillList(BattleCityField * baCiFi);

private:
    QHash<int, QByteArray> roleNames() const;
    std::array<QString, 729> m_listBullet_1;
    BattleCityField * m_baCiFi;

public slots:
    void newBullet_1_Slot(short beginY, short beginX);
    void moveBullet_1_Slot(short beginY, short beginX, short endY, short endX);

};

#endif // QMLBULLET1LIST_H
