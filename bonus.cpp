#include "bonus.h"
#include <ctime>
#include <cstdlib>

Bonus::Bonus(QQmlApplicationEngine* engine)
{
    srand(time(NULL));
    m_Column = rand() % 25;
    m_Row = (rand() % 21)+2;
    m_Id = rand() % 7;
    QQmlComponent component(engine, QUrl(QStringLiteral("qrc:/Bonus.qml")));
    instanceBonus = component.create();
    instanceBonus->setProperty("x", m_Column*27);
    instanceBonus->setProperty("y", m_Row * 27);

    switch (m_Id) {
    case 0:
        instanceBonus->setProperty("source","qrc:/image/helm.png");
        break;
    case 1:
        instanceBonus->setProperty("source","qrc:/image/clock.png");
        break;
    case 2:
        instanceBonus->setProperty("source","qrc:/image/shovel.png");
        break;
    case 3:
        instanceBonus->setProperty("source","qrc:/image/star.png");
        break;
    case 4:
        instanceBonus->setProperty("source","qrc:/image/grenade.png");
        break;
    case 5:
        instanceBonus->setProperty("source","qrc:/image/life_tank.png");
        break;
    case 6:
        instanceBonus->setProperty("source","qrc:/image/gun.png");
        break;
    default:
        break;
    }

}

Bonus::~Bonus(){

    if (instanceBonus != nullptr)
        delete instanceBonus;

}
