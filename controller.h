#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "battlecityfield.h"
#include <QObject>
#include "qmldataview.h"
#include "qmltankview.h"
#include "qmlbullet0list.h"
#include "qmlbullet1list.h"
#include "qmlbullet2list.h"
#include "qmlbullet3list.h"
#include "numberoftanks.h"
#include "readerstageinfile.h"
#include <QQmlApplicationEngine>


class Controller : public QObject
{

    Q_OBJECT

public:
    Q_INVOKABLE void setDirectMoveTankOfPlayer(const int direct){m_battleCityField.setDirectTankOfPlayer(direct);}
    Q_INVOKABLE void setDirectMoveTankOfPlayer2(const int direct){m_battleCityField.setDirectTankOfPlayer2(direct);}
    Q_INVOKABLE void setShotOfPlayer(){m_battleCityField.doShotPlayer();}
    Q_INVOKABLE void setShotOfPlayer2(){m_battleCityField.doShotPlayer2();}
    Q_INVOKABLE void loadStage();
    Q_INVOKABLE void fillQmlLists();
    Q_INVOKABLE void startGame();



public:
    Controller(QQmlApplicationEngine* engine);
    BattleCityField *get_BattleCityField();
    QmlDataView * get_QmlDataView();
    QmlTankView * get_QmlTankView();
    QmlBullet0List* get_QmlBullet0List();
    QmlBullet1List *get_QmlBullet1List();
    QmlBullet2List* get_QmlBullet2List();
    QmlBullet3List *get_QmlBullet3List();
    NumberOfTanks*  get_NumberOfTanks();
    ~Controller(){ qDebug() << "controller Delete";}




private:
    BattleCityField m_battleCityField;
    QmlDataView m_qmlDataView;
    QmlTankView m_QmlTankView;
    QmlBullet0List m_QmlBullet0List;
    QmlBullet1List m_QmlBullet1List;
    QmlBullet2List m_QmlBullet2List;
    QmlBullet3List m_QmlBullet3List;
    NumberOfTanks m_NumberOfTanks;
    ReaderStageInFile m_ReaderStageInFile;


};

#endif // CONTROLLER_H
