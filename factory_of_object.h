#ifndef FACTORY_OF_OBJECT_H
#define FACTORY_OF_OBJECT_H
#include "object.h"
#include "field.h"

class Factory_of_Object
{
public:
    virtual Object* createObject(QQmlApplicationEngine* enginePointer = nullptr, short row = 0,short column = 0) = 0;
    virtual ~Factory_of_Object() {}
};

class Factory_of_Field
{
public:
    virtual Field* createField() = 0;
    virtual ~Factory_of_Field() {}
};

#endif // FACTORY_OF_OBJECT_H
