#include "object.h"


Object::Object(QQmlApplicationEngine* enginePointer, short row ,short column)
{
   m_QQmlComponent = createQQmlComponent(enginePointer, row, column);
}

QObject* Object::createQQmlComponent(QQmlApplicationEngine* enginePointer, short row,short column)
{
    QQmlComponent component(enginePointer, QUrl(QStringLiteral("qrc:/ImageOfObject.qml")));
    QObject* qmlObject = component.create();
    qmlObject->setProperty("imageX", column*27);
    qmlObject->setProperty("imageY", row*27);
    qmlObject->setProperty("rotation", directionMove*90);
    qmlObject->setProperty("source",m_source);
    return qmlObject;
}

Object::~Object()
{
    if (m_QQmlComponent != nullptr)
        delete m_QQmlComponent;
}

void Object::moveQQmlComponent(short row,short column)
{
    m_QQmlComponent->setProperty("imageX", column*27);
    m_QQmlComponent->setProperty("imageY", row*27);
}

void Object::rotationQQmlComponent(int rand)
{
    switch (rand) {
    case 1:
        m_QQmlComponent->setProperty("rotation", 270);
        break;
    case 3:
        m_QQmlComponent->setProperty("rotation", 90);
        break;
    default:
        m_QQmlComponent->setProperty("rotation", rand * 90);
        break;
    }
}

void Object::setSpeed(short speed)
{
    m_speed = speed;
    if (m_QQmlComponent != nullptr){
        m_QQmlComponent->setProperty("speed" , speed);
    }
}
void Object::setBonus() {m_bonus = true; m_QQmlComponent->setProperty("bonus", true);}
bool Object::getBonus() const {return m_bonus; }
void Object::resetBonus(){m_bonus = false; m_QQmlComponent->setProperty("bonus", false);}

void Object::hitting ()
{
    qDebug() << "m_armor == " << m_armor;
    if (m_count_star > 0){
        m_count_star = 0;
        getQmlObject()->setProperty("source","qrc:/image/firstTank.png");
        m_fastSpeedBullet = false;
        quickFire = false;
        m_steelDestroed = false;
    }
    --m_armor;
}

void Object::plusStar()
{
    switch (m_count_star) {
    case 0:
        ++m_count_star;
        getQmlObject()->setProperty("source","qrc:/image/secondTank.png");
        m_fastSpeedBullet = true;
        qDebug() << "m_armor =+= " << m_armor;
        ++m_armor;
        qDebug() << "m_armor = " << m_armor;
        break;
    case 1:
        ++m_count_star;
        getQmlObject()->setProperty("source","qrc:/image/thirdTank.png");
        quickFire = true;
        qDebug() << "m_armor = " << m_armor;
        break;
    case 2:
        ++m_count_star;
        getQmlObject()->setProperty("source","qrc:/image/fourTank.png");
        m_steelDestroed = true;
        qDebug() << "m_armor = " << m_armor;
        break;
    }
}

void Object::resetTank()
{
    m_count_star = 0;
    quickFire = false;
    m_armor = 1;
    m_fastSpeedBullet = false;
    m_steelDestroed = false;
    m_QQmlComponent->setProperty("source","qrc:/image/firstTank.png");
}
