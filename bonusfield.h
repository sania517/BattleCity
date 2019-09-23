#ifndef BONUSFIELD_H
#define BONUSFIELD_H

#include <QString>



struct BonusField
{
    enum bonusId {tank = 0, star, gun, grenade, clock, boat, shovel, helmet};
    BonusField();
    QString getUrlBonus();
    short koordX;
    short koordY;
    bonusId idBonus;
};

#endif // BONUSFIELD_H
