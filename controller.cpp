#include "controller.h"
#include "factoryofitem.h"
#include "factory_of_object.h"
#include <QDebug>

Controller::Controller(QQmlApplicationEngine *engine)
    :QObject(), m_battleCityField(engine)
{



    connect(&m_battleCityField, &BattleCityField::newFlashSignal,    &m_qmlDataView, &QmlDataView::newFlashSlot );
    connect(&m_battleCityField, &BattleCityField::newBulletSignal_0, &m_QmlBullet0List, &QmlBullet0List::newBullet_0_Slot);
    connect(&m_battleCityField, &BattleCityField::newBulletSignal_1, &m_QmlBullet1List, &QmlBullet1List::newBullet_1_Slot);
    connect(&m_battleCityField, &BattleCityField::newBulletSignal_2, &m_QmlBullet2List, &QmlBullet2List::newBullet_2_Slot);
    connect(&m_battleCityField, &BattleCityField::newBulletSignal_3, &m_QmlBullet3List, &QmlBullet3List::newBullet_3_Slot);

    connect(&m_battleCityField, &BattleCityField::moveBulletSignal_0, &m_QmlBullet0List, &QmlBullet0List::moveBullet_0_Slot);
    connect(&m_battleCityField, &BattleCityField::moveBulletSignal_1, &m_QmlBullet1List, &QmlBullet1List::moveBullet_1_Slot);
    connect(&m_battleCityField, &BattleCityField::moveBulletSignal_2, &m_QmlBullet2List, &QmlBullet2List::moveBullet_2_Slot);
    connect(&m_battleCityField, &BattleCityField::moveBulletSignal_3, &m_QmlBullet3List, &QmlBullet3List::moveBullet_3_Slot);

    connect(&m_battleCityField, &BattleCityField::deleteFieldSignal,  &m_qmlDataView, &QmlDataView::deleteFieldSlot);

    connect(&m_battleCityField, &BattleCityField::moveTankSignal,         &m_QmlTankView, &QmlTankView::moveTankSlot);
    connect(&m_battleCityField, &BattleCityField::moveTankOfPlayerSignal, &m_QmlTankView, &QmlTankView::moveTankOfPlayerSlot);
    connect(&m_battleCityField, &BattleCityField::deleteTankSignal,       &m_QmlTankView, &QmlTankView::deleteTankSlot);
    connect(&m_battleCityField, &BattleCityField::createTankSignal,       &m_QmlTankView, &QmlTankView::createTankSlot);
    connect(&m_battleCityField, &BattleCityField::createTankOfPlayerSignal, &m_QmlTankView, &QmlTankView::createTankOfPlayerSlot);

    connect(&m_battleCityField, &BattleCityField::createTankCountSignal, &m_NumberOfTanks, &NumberOfTanks::createNumberOfTankSlot);

}


BattleCityField * Controller::get_BattleCityField()
{
    return &m_battleCityField;
}

QmlDataView * Controller::get_QmlDataView()
{
    return &m_qmlDataView;
}

QmlTankView * Controller::get_QmlTankView()
{
    return &m_QmlTankView;
}

void Controller::fillQmlLists()
{
    m_qmlDataView.fillList(&m_battleCityField);
    m_QmlTankView.fillList(&m_battleCityField);
    m_QmlBullet0List.fillList(&m_battleCityField);
    m_QmlBullet1List.fillList(&m_battleCityField);
    m_QmlBullet2List.fillList(&m_battleCityField);
    m_QmlBullet3List.fillList(&m_battleCityField);
    m_NumberOfTanks.fillList(&m_battleCityField);
    qDebug() << "fillQmlLists()";
}

QmlBullet0List * Controller::get_QmlBullet0List(){return &m_QmlBullet0List;}
QmlBullet1List * Controller::get_QmlBullet1List(){return &m_QmlBullet1List;}
QmlBullet2List * Controller::get_QmlBullet2List(){return &m_QmlBullet2List;}
QmlBullet3List * Controller::get_QmlBullet3List(){return &m_QmlBullet3List;}
NumberOfTanks *  Controller::get_NumberOfTanks(){return &m_NumberOfTanks;}

void Controller::loadStage()
{
    m_battleCityField.fillFieldFromTheFile(m_ReaderStageInFile.readFile(m_battleCityField.stage()));
    //m_battleCityField.fillField();

    fillQmlLists();


}

void Controller::startGame()
{
    m_battleCityField.firstLoadTanksSignals();
    m_battleCityField.startGame();
}
