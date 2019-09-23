#include "qmltankview.h"



QmlTankView::QmlTankView(QObject *parent)
    :QAbstractListModel(parent)
{
    int size = m_listTank.size();
    for (int i = 0; i < size; i++){
        m_listTank[i] = {QString(),-1};
    }
}

QHash<int, QByteArray> QmlTankView::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[urlTankRole] = "urlTank";
    roles[directTankRole] = "directTank";
    return roles;
}

short QmlTankView::dirRotation(const int dir) const
{
    switch (dir) {
        case 0:
           return 0;
            break;
        case 1:
           return 270;
            break;
        case 2:
           return 180;
            break;
        case 3:
           return 90;
            break;
        default:
           return 270;
            break;
    }
}

int QmlTankView::rowCount(const QModelIndex &) const
{
    return m_listTank.size();
}

QVariant QmlTankView::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > rowCount())
        return QVariant();
    if (role == urlTankRole)
        return m_listTank[index.row()].m_url;
    if (role == directTankRole)
        return m_listTank[index.row()].m_dir;
    return QVariant();
}

void QmlTankView::fillList(BattleCityField *baCiFi)
{
    m_baCiFi = baCiFi;
    int y;
    int x;
    for (int i = 0; i < 676;i++){
         y = i / 26;
         x = i % 26;
        if (auto object = baCiFi->getObject(y,x)){
            m_listTank[i] = {object->urlView(), dirRotation(object->getDirectMove())};
        } else {
            m_listTank[i] = {QString(),-1};
        };
    }
}

void QmlTankView::moveTankSlot(short beginY, short beginX, short endY, short endX)
{
    int begin = beginY*26 + beginX;
    int end = endY*26+endX;
    if (beginX == endX){
        if (beginY > endY){
            beginMoveRows(QModelIndex(),begin,begin,QModelIndex(),end);
            std::swap(m_listTank[begin],m_listTank[end]);
            endMoveRows();
            beginMoveRows(QModelIndex(),end+1,end+1,QModelIndex(),begin+1);
            endMoveRows();
        }else {
            beginMoveRows(QModelIndex(),end,end,QModelIndex(),begin);
            std::swap(m_listTank[begin],m_listTank[end]);
            endMoveRows();
            beginMoveRows(QModelIndex(),begin+1,begin+1,QModelIndex(),end+1);
            endMoveRows();
        }
    }else if (beginY == endY){
        if (endX > beginX){
            beginMoveRows(QModelIndex(),end,end,QModelIndex(),begin);
            std::swap(m_listTank[begin],m_listTank[end]);
            endMoveRows();
        } else {
            beginMoveRows(QModelIndex(),begin,begin,QModelIndex(),end);
            std::swap(m_listTank[begin],m_listTank[end]);
            endMoveRows();
        }
    }
}


void QmlTankView::moveTankOfPlayerSlot(short beginY, short beginX, short endY, short endX, bool secondPlayer)
{
    int begin = beginY*26 + beginX;

    m_listTank[begin].m_dir = secondPlayer ? dirRotation(m_baCiFi->getLastDirect2Move()) : dirRotation(m_baCiFi->getLastDirectMove());
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
    moveTankSlot(beginY, beginX, endY, endX);
}

void QmlTankView::deleteTankSlot(short y,short x)
{
    int begin = y*26 + x;
    m_listTank[begin] = {QString(),dirRotation(-2)};
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
}

void QmlTankView::createTankSlot(short y,short x)
{
    int begin = y*26 + x;
    const Object * object = m_baCiFi->getObject(y,x);
    if (object != nullptr){
        m_listTank[begin] = {object->urlView(), dirRotation(object->getDirectMove())};
    } else {
        m_listTank[begin] = {QString(),dirRotation(-2)};
    }
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
    qDebug() << "createTankSlot";
}

void QmlTankView::createTankOfPlayerSlot(short y,short x, bool secondPlayer)
{
    int begin = y*26 + x;
    if (m_baCiFi->getObject(y,x) != nullptr){
        short dir =  secondPlayer ? dirRotation(m_baCiFi->getLastDirect2Move()) : dirRotation(m_baCiFi->getLastDirectMove());
        m_listTank[begin] = {m_baCiFi->getObject(y,x)->urlView(), dir};
    }
    QModelIndex beginIndex = createIndex(begin,0);
    emit dataChanged(beginIndex, beginIndex);
}
