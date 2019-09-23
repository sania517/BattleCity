#include "battlefieldobjects.h"
#include "object.h"
#include <QQmlProperty>
//#include <QString>

Tank::Tank(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    directionMove = 3;
    m_QQmlComponent->setProperty("rotation", 90);
    m_enemy = true;
    m_armor = 1;
    idTank = 0;
    m_speed = 6;
    m_source = "image/Tank.png";
//    QQmlProperty(m_QQmlComponent, "source").write("qrc:/image/Tank.png");
    m_QQmlComponent->setProperty("source","qrc:/image/Tank.png");
//    m_QQmlComponent->setProperty("speed" , m_speed);
    m_QQmlComponent->setProperty("speedRotation" , 1);
}

FastTank::FastTank(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    directionMove = 3;
    m_QQmlComponent->setProperty("rotation", 90);
    m_enemy = true;
    m_armor = 1;
    idTank = 1;
    m_speed = 2;
    m_fastSpeedBullet = true;
    m_source = "image/SpeedTank.png";
    m_QQmlComponent->setProperty("source","qrc:/image/SpeedTank.png");
//    m_QQmlComponent->setProperty("speed" , m_speed);
    m_QQmlComponent->setProperty("speedRotation" , 1);
}

ArmorTank::ArmorTank(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    directionMove = 3;
    m_QQmlComponent->setProperty("rotation", 90);
    m_enemy = true;
    m_armor = 2;
    idTank = 2;
    m_speed = 6;
    m_fastSpeedBullet = true;
    m_source = "image/armorTank.png";
    m_QQmlComponent->setProperty("source","qrc:/image/armorTank.png");
//    m_QQmlComponent->setProperty("speed" , m_speed);
    m_QQmlComponent->setProperty("speedRotation" , 1);
}

HeavyArmorTank::HeavyArmorTank(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    directionMove = 3;
    m_QQmlComponent->setProperty("rotation", 90);
    m_enemy = true;
    m_armor = 3;
    idTank = 3;
    m_speed = 6;
    m_source ="image/ArmoredTank.png";
    m_QQmlComponent->setProperty("source","qrc:/image/ArmoredTank.png");
//    m_QQmlComponent->setProperty("speed" , m_speed);
    m_QQmlComponent->setProperty("speedRotation" , 1);
}

TankForPlayer::TankForPlayer(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    directionMove = -1;
    m_QQmlComponent->setProperty("rotation", 270);
    m_enemy = false;
    m_speed = 4;
    m_armor = 1;
    m_fastSpeedBullet = false;
    m_source = "image/Tank_Our.png";
    m_QQmlComponent->setProperty("source","qrc:/image/firstTank.png");
    m_QQmlComponent->setProperty("speed" , m_speed);
    m_QQmlComponent->setProperty("speedRotation" , 1);
    qDebug() << "m_armor create" << m_armor;
}

Bullet::Bullet(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    directionMove = -1;
    m_QQmlComponent->setProperty("rotation", 0);
    m_source = "image/Bullet.png";
    m_speed = 2;
    m_QQmlComponent->setProperty("source","qrc:/image/Bullet.png");
    m_QQmlComponent->setProperty("speed" , m_speed);

}

Flash::Flash(QQmlApplicationEngine *enginePointer, short row, short column)
    : Object(enginePointer,row, column)
{
    m_speed = 0;
    directionMove = -1;
    m_QQmlComponent->setProperty("rotation", 0);
    m_source ="image/Flash.png";
    m_QQmlComponent->setProperty("source","qrc:/image/Flash.png");
    m_QQmlComponent->setProperty("speed" , m_speed);
}

BrickWall::BrickWall()   : Field()
{
    passability = false;        //проходимость
    fly_passability = false;    //проходимость снарядом
    speed = 0;                  //скорость движения
    rotation = true;            //возможность поворота при движении
    destruction = true;         // разрушаемость
    super_destruction = true;   //разрушение мощными снарядами
    hitCount = 2;               // количество попаданий до разрушения

}

SteelWall::SteelWall()   : Field()
{
    passability = false;        //проходимость
    fly_passability = false;    //проходимость снарядом
    speed = 0;                  //скорость движения
    rotation = true;            //возможность поворота при движении
    destruction = false;         // разрушаемость
    super_destruction = true;   //разрушение мощными снарядами
    hitCount = 1;
}

Water::Water()           : Field()
{
    passability = false;        //проходимость
    fly_passability = true;    //проходимость снарядом
    speed = 0.5;                //скорость движения
    rotation = true;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 10;
}
Forest::Forest()         : Field()
{
    passability = true;        //проходимость
    fly_passability = true;    //проходимость снарядом
    speed = 0;                //скорость движения
    rotation = true;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 10;
}
Throne1::Throne1()         : Field()
{
    passability = false;        //проходимость
    fly_passability = false;    //проходимость снарядом
    speed = 0;                //скорость движения
    rotation = false;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 1;
}

Throne2::Throne2()         : Field()
{
    passability = false;        //проходимость
    fly_passability = false;    //проходимость снарядом
    speed = 0;                //скорость движения
    rotation = false;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 1;
}

Throne3::Throne3()         : Field()
{
    passability = false;        //проходимость
    fly_passability = false;    //проходимость снарядом
    speed = 0;                //скорость движения
    rotation = false;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 1;
}

Throne4::Throne4()         : Field()
{
    passability = false;        //проходимость
    fly_passability = false;    //проходимость снарядом
    speed = 0;                //скорость движения
    rotation = false;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 1;
}

IceRoad::IceRoad()       : Field()
{
    passability = true;        //проходимость
    fly_passability = true;    //проходимость снарядом
    speed = 2;                //скорость движения
    rotation = false;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 10;
}
EmptyField::EmptyField() : Field()
{
    passability = true;        //проходимость
    fly_passability = true;    //проходимость снарядом
    speed = 1;                //скорость движения
    rotation = true;            //возможность поворота при движении
    destruction = false;        // разрушаемость
    super_destruction = false;   //разрушение мощными снарядами
    hitCount = 10;
}


QString Tank::urlView() const
{
    return  "image/Tank.png";
}

QString FastTank::urlView() const
{
    return  "image/SpeedTank.png";
}

QString ArmorTank::urlView() const
{
    return  "image/armorTank.png";
}
QString HeavyArmorTank::urlView() const
{
    return  "image/ArmoredTank.png";
}

QString Flash::urlView() const
{
    return  "image/Flash.png";
}

QString TankForPlayer::urlView() const
{
    return  "image/Tank_Our.png";
}

QString Bullet::urlView() const
{
    return  "image/Bullet.png";
}


QString BrickWall::urlField() const
{
    return  "image/BrickWall.png";
}

QString SteelWall::urlField() const
{
    return  "image/steelWall.png";
}

QString Water::urlField() const
{
    return  "image/Water.png";
}

QString Forest::urlField() const
{
    return  "image/Forest.png";
}

QString Throne1::urlField() const
{
   return m_destroed ? "image/ThroneDestroed1.png" :  "image/Throne1.png";
}

QString Throne2::urlField() const
{
    return m_destroed ? "image/ThroneDestroed2.png" : "image/Throne2.png";
}

QString Throne3::urlField() const
{
    return m_destroed ? "image/ThroneDestroed3.png" :  "image/Throne3.png";
}

QString Throne4::urlField() const
{
    return m_destroed ? "image/ThroneDestroed4.png" : "image/Throne4.png";
}

QString IceRoad::urlField() const
{
    return  "image/Ice.png";
}

QString EmptyField::urlField() const
{
    return  "image/EmptyField.png";

}










