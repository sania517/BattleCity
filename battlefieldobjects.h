#ifndef BATTLEFIELDOBJECTS_H
#define BATTLEFIELDOBJECTS_H

#include "object.h"
#include "field.h"
//#include <QString>

class Tank : public Object
{
public:
    Tank(QQmlApplicationEngine* enginePointer, short row, short column);
    QString urlView() const;
};

class FastTank : public Object
{
public:
    FastTank(QQmlApplicationEngine* enginePointer, short row, short column);
    QString urlView() const;
};

class ArmorTank : public Object
{
public:
    ArmorTank(QQmlApplicationEngine* enginePointer, short row, short column);
    QString urlView() const;
};

class HeavyArmorTank : public Object
{
public:
    HeavyArmorTank(QQmlApplicationEngine* enginePointer, short row, short column);
    QString urlView() const;
};

class TankForPlayer : public Object
{
public:
    QString urlView() const;
    TankForPlayer(QQmlApplicationEngine* enginePointer, short row, short column);
};

class Bullet : public Object
{
public:
    Bullet(QQmlApplicationEngine* enginePointer, short row, short column);
    QString urlView() const;

};

class Flash : public Object
{
public:
    Flash(QQmlApplicationEngine* enginePointer, short row, short column);
    QString urlView() const;
};

class SteelWall : public Field
{
public:
    SteelWall();
    QString urlField() const;

};

class BrickWall : public Field
{
public:
    BrickWall();
    QString urlField() const;
};

class Water : public Field
{
public:
    Water() ;
    QString urlField() const;
};


class Forest : public Field
{
public:
    Forest();
    QString urlField() const;
};

class Throne1 : public Field
{
public:
    Throne1();
    QString urlField() const;
};

class Throne2 : public Field
{
public:
    Throne2();
    QString urlField() const;
};

class Throne3 : public Field
{
public:
    Throne3();
    QString urlField() const;
};

class Throne4 : public Field
{
public:
    Throne4();
    QString urlField() const;
};

class IceRoad : public Field
{
public:
    IceRoad();
    QString urlField() const;
};


class EmptyField : public Field
{
public:
    EmptyField();
    QString urlField() const;
};

#endif // BATTLEFIELDOBJECTS_H
