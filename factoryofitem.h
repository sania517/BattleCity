#ifndef FACTORYOFITEM_H
#define FACTORYOFITEM_H
#include "factory_of_object.h"
#include "battlefieldobjects.h"

class Factory_Tank :public Factory_of_Object
{
public:
    Factory_Tank() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    { return new Tank(enginePointer, row ,column);}
};

class Factory_FastTank :public Factory_of_Object
{
public:
    Factory_FastTank() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    { return new FastTank(enginePointer, row ,column);}
};

class Factory_ArmorTank :public Factory_of_Object
{
public:
    Factory_ArmorTank() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    { return new ArmorTank(enginePointer, row ,column);}
};

class Factory_HeavyArmorTank :public Factory_of_Object
{
public:
    Factory_HeavyArmorTank() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    { return new HeavyArmorTank(enginePointer, row ,column);}
};




class Factory_TankForPlayer :public Factory_of_Object
{
public:
    Factory_TankForPlayer() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    {return new TankForPlayer(enginePointer, row ,column);}
};


class Factory_Bullet :public Factory_of_Object
{
public:
    Factory_Bullet() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    {return new Bullet(enginePointer, row ,column);}
};

class Factory_Flash :public Factory_of_Object
{
public:
    Factory_Flash() : Factory_of_Object(){;}
    Object* createObject(QQmlApplicationEngine* enginePointer, short row,short column)
    {return new Flash(enginePointer, row ,column);}
};

//------------------------------------------------

class Factory_Forest :public Factory_of_Field
{
public:
    Factory_Forest() : Factory_of_Field(){;}
    Field* createField(){return new Forest();}
};

class Factory_IceRoad :public Factory_of_Field
{
public:
    Factory_IceRoad() : Factory_of_Field(){;}
    Field* createField(){return new IceRoad();}
};

class Factory_SteelWall :public Factory_of_Field
{
public:
    Factory_SteelWall() : Factory_of_Field(){;}
    Field* createField(){return new SteelWall();}
};

class Factory_Throne1 :public Factory_of_Field
{
public:
    Factory_Throne1() : Factory_of_Field(){;}
    Field* createField(){return new Throne1();}
};

class Factory_Throne2 :public Factory_of_Field
{
public:
    Factory_Throne2() : Factory_of_Field(){;}
    Field* createField(){return new Throne2();}
};

class Factory_Throne3 :public Factory_of_Field
{
public:
    Factory_Throne3() : Factory_of_Field(){;}
    Field* createField(){return new Throne3();}
};

class Factory_Throne4 :public Factory_of_Field
{
public:
    Factory_Throne4() : Factory_of_Field(){;}
    Field* createField(){return new Throne4();}
};

class Factory_Water :public Factory_of_Field
{
public:
    Factory_Water() : Factory_of_Field(){;}
    Field* createField(){return new Water();}
};

class Factory_BrickWall :public Factory_of_Field
{
public:
    Factory_BrickWall() : Factory_of_Field(){;}
    Field* createField(){return new BrickWall();}
};

class Factory_EmptyField :public Factory_of_Field
{
public:
    Factory_EmptyField() : Factory_of_Field(){;}
    Field* createField(){return new EmptyField();}
};

#endif // FACTORYOFITEM_H
