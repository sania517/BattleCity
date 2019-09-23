#ifndef HASHOBJECTS_H
#define HASHOBJECTS_H

#include <QObject>
#include <QHash>
//#include "object.h"

class HashObjects : public QObject
{
    Q_OBJECT
public:
    HashObjects();

private:
    QHash<int, QObject*> hashTableOfObjects;
};

#endif // HASHOBJECTS_H
