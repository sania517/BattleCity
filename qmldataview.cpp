#include "qmldataview.h"
#include <QDebug>

QmlDataView::QmlDataView(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listField.size();
    for (int i = 0; i < size; i++){
        m_listField[i] = QString();
        m_listFlash[i] = QString();
    }
}

QHash<int, QByteArray> QmlDataView::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[urlFieldRole] = "urlField";
    roles[urlFlashRole] = "urlFlash";
    return roles;
}




int QmlDataView::rowCount(const QModelIndex &) const
{
    return m_listField.size();
}

QVariant QmlDataView::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == urlFieldRole)
        return m_listField[index.row()];
    if (role == urlFlashRole)
        return m_listFlash[index.row()];
    return QVariant();

}

void QmlDataView::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    int y;
    int x;
    for (int i = 0; i < 676;i++){
        y = i/26;
        x = i % 26;
        m_listField[i] = baCiFi->getField(y,x)->urlField();
    }
    for (int i = 0; i < 729;i++){
        m_listFlash[i] = QString();
    }
}

void QmlDataView::newFlashSlot(short beginY, short beginX)
{
    int begin = beginY*27 + beginX;
    QVector<int> chengedRoles;
    chengedRoles.push_back(urlFlashRole);
    if(auto object = m_baCiFi->getBulletAndFlash(beginY, beginX, -1))
        m_listFlash[begin] = object->urlView();
    else{
        m_listFlash[begin] = QString();
    }
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex, chengedRoles);
}

void QmlDataView::deleteFieldSlot(short y,short x)
{
    int begin = y*26 + x;
    const Field* field = m_baCiFi->getField(y, x);
    if (field != nullptr)
        m_listField[begin] = field->urlField();
     else
        m_listField[begin] = m_baCiFi->getEmptyField()->urlField();
    QVector<int> chengedRoles {urlFieldRole};
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex, chengedRoles);
}
