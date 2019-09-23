#ifndef QMLDATAVIEW_H
#define QMLDATAVIEW_H
#include <QAbstractListModel>
#include <array>
#include <QString>
#include "battlecityfield.h"




class QmlDataView : public QAbstractListModel
{
    Q_OBJECT

public:
    QmlDataView(QObject *parent = Q_NULLPTR);
    enum BattleCityRoles {urlFieldRole = Qt::UserRole+1, urlFlashRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void fillList(BattleCityField * baCiFi);

private:
    QHash<int, QByteArray> roleNames() const;
    std::array<QString, 729> m_listField;
    std::array<QString, 729> m_listFlash;
    BattleCityField * m_baCiFi;

public slots:
    void newFlashSlot(short beginY, short beginX);
     void deleteFieldSlot(short,short);
};

#endif // QMLDATAVIEW_H
