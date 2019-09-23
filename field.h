#ifndef FIELD_H
#define FIELD_H

#include <QString>



class Field
{
public:
    Field(){;}
    virtual  QString  urlField() const = 0;
    bool getPassability() const {return passability;}
    bool getFlyPassability() const {return fly_passability;}
    bool getDestruction() const {return destruction;}
    void destroed(){m_destroed = true;}
    bool getDestroed(){return m_destroed;}
    bool isSuperDestruction() const {return super_destruction;}
    virtual ~Field(){}

protected:
    bool  passability;      //проходимость
    bool  fly_passability;  //проходимость снарядом
    short speed;            //скорость движения
    bool  rotation;         //возможность поворота при движении
    bool destruction;       // разрушаемость
    bool super_destruction; //разрушение мощными снарядами
    short hitCount;         // количество попаданий до разрушения
    bool m_destroed =false;

};

#endif // FIELD_H
