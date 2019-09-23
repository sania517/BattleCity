#include "bonusfield.h"
#include <ctime>
#include <cstdlib>

BonusField::BonusField()
{
    srand(time(NULL));
    koordX = rand() % 25;
    koordY = rand() % 25;
    int id = rand() % 8;
    switch (id) {
    case 0:
       idBonus = tank;
        break;
    case 1:
       idBonus = star;
        break;
    case 2:
       idBonus = gun;
        break;
    case 3:
       idBonus =  grenade;
        break;
    case 4:
       idBonus =  clock;
        break;
    case 5:
       idBonus = boat;
        break;
    case 6:
       idBonus = shovel;
        break;
    case 7:
       idBonus = helmet;
        break;
    }
}

QString BonusField::getUrlBonus()
{
    switch (idBonus) {
    case tank:
        return "image/...";
        break;
    case star:
        return "image/...";
        break;
    case gun:
        return "image/...";
        break;
    case grenade:
        return "image/...";
        break;
    case clock:
        return "image/...";
        break;
    case boat:
        return "image/...";
        break;
    case shovel:
        return "image/...";
        break;
    case helmet:
        return "image/...";
        break;
    }
}
