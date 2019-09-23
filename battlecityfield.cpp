#include "battlecityfield.h"
#include <ctime>
#include <cstdlib>
#include <QDebug>
#include "factoryofitem.h"
#include <QQmlProperty>


//#include "factory_of_object.h"


BattleCityField::BattleCityField(QQmlApplicationEngine *engine, QObject *parent)
    :QTimer(parent), m_object{{nullptr}}, m_field{{nullptr}}, m_bullet0{{nullptr}}, m_bullet1{{nullptr}},
      m_bullet2{{nullptr}},m_bullet3{{nullptr}}, m_flashArray{{nullptr}}, m_Factory_of_Object{nullptr},
      m_tankOfPlayer{nullptr},m_tankOfPlayer2{nullptr}, m_engine{engine}
{
    tankMoveTimer = new QTimer(this);
    connect(tankMoveTimer, &QTimer::timeout, this, &BattleCityField::goMoveTimer);
}

void BattleCityField:: setSecond_Player(bool secondPlayer)
{
    m_secondPlayer = secondPlayer;
    if (m_tankOfPlayer == nullptr){
        emit second_PlayerChanged(m_secondPlayer);
        firstFillField();
    } else{
        emit second_PlayerChanged(m_secondPlayer);
        resetModel();
    }

}

void BattleCityField::startGame()
{
    tankMoveTimer->start(200);
}

BattleCityField::~BattleCityField()
{

    for (int x = 0; x<26; x++){
        for (int y = 0; y < 26; y++){
            if (m_object[y][x] != nullptr)
                if (m_object[y][x] != m_tankOfPlayer && m_object[y][x] != m_tankOfPlayer2)
                    delete m_object[y][x];
            if (m_field[y][x] != nullptr && m_field[y][x] != m_emptyField){
                delete m_field[y][x];
            }
        }
    }
    for (int x = 0; x<27; x++){
        for (int y = 0; y < 27; y++){
            if (m_bullet0 [y][x] != nullptr)
                delete m_bullet0 [y][x];
            if (m_bullet1[y][x] != nullptr)
                delete m_bullet1[y][x];
            if (m_bullet2[y][x] != nullptr)
                delete m_bullet2[y][x];
            if (m_bullet3[y][x] != nullptr)
                delete m_bullet3[y][x];
            if (m_flashArray[y][x] != nullptr)
                delete m_flashArray[y][x];
        }
    }
    for(int i = 0 ; i<10 ; ++i){
        if (m_Factory_of_Object[i] != nullptr)
            delete m_Factory_of_Object[i];
    }
    delete m_emptyField;
    delete tankMoveTimer;
    delete m_tankOfPlayer;
    delete m_tankOfPlayer2;
    if(m_bonus != nullptr)
        delete m_bonus;
}

void BattleCityField::goMoveTimer()
{
    int step = getNumberStep();

    if (m_notEmptySpaceForTank){       //проверяем не стоит ли в очереди создание танка
        m_notEmptySpaceForTank = false;
        createTank();                  //пытаемся создать танк
    }

    if (m_countDelayCreationNextTank > 0)
        --m_countDelayCreationNextTank;
    else if(m_countDelayCreationNextTank == 0) {
        createTank();
        --m_countDelayCreationNextTank;
    }

    if (m_countHelmBonusTank1 > 0){
        --m_countHelmBonusTank1;
    } else if(m_countHelmBonusTank1 == 0) {
        m_tankOfPlayer->returnArmor();
        m_tankOfPlayer->getQmlObject()->setProperty("helm", false);
        --m_countHelmBonusTank1;
    }
    if (m_countHelmBonusTank2 > 0){
        --m_countHelmBonusTank2;
    } else if (m_countHelmBonusTank2 == 0){
        m_tankOfPlayer2->returnArmor();
        m_tankOfPlayer2->getQmlObject()->setProperty("helm", false);
        --m_countHelmBonusTank2;
    }

    bool clockBonusActive  = false;
    if (m_countClockBonus > 0){
        --m_countClockBonus;
        clockBonusActive = true;
    }

    if (m_countShovelBonus > 0){
        --m_countShovelBonus;
    } else if (m_countShovelBonus == 0){
        resetArmorBase();
        --m_countShovelBonus;
    }

    if(step % 4 == 2){
        if (getIsShotPlayer()){
            if ((m_tankOfPlayer->isQuickFire()) || ((step / 4)) & 1){
                shotPlayer(m_y,m_x,lastDirect, false);
                resetShotPlayer();
            }

        }
        if (m_secondPlayer){
            if (getIsShotPlayer2()){
                if ((m_tankOfPlayer2->isQuickFire()) || ((step / 4)) & 1){
                    shotPlayer(m_y2,m_x2,lastDirect2, true);
                    resetShotPlayer2();
                }

            }
        }
    }

    if (step % 18 == 0 && !clockBonusActive){
        shot();
    }
    moveBullet();
    if ((step % 2) == 1)
        moveTank(clockBonusActive);
    setNumberStep();

    if (countTimeForEndGame < CONST_TIME_FOR_END_GAME){
        if (countTimeForEndGame < 1)
            stopGame();
        --countTimeForEndGame;
        return;
    }

    if(m_field[24][12]->getDestroed() || m_field[25][12]->getDestroed() || m_field[24][13]->getDestroed() || m_field[25][13]->getDestroed() ){

        m_field[24][12]->destroed();
        m_field[25][12]->destroed();
        m_field[24][13]->destroed();
        m_field[25][13]->destroed();
        emit deleteFieldSignal(24,12);
        emit deleteFieldSignal(25,12);
        emit deleteFieldSignal(24,13);
        emit deleteFieldSignal(25,13);
        --countTimeForEndGame;
        m_gameOver = true;
        emit gameOverChanged(m_gameOver);
        return;
    }
    if (m_countLife < 0 && m_countLife2 < 0){
        --countTimeForEndGame;
        m_gameOver = true;
        emit gameOverChanged(m_gameOver);
        return;
    }

    if (m_countLife < 0 && !m_secondPlayer){
        --countTimeForEndGame;
        m_gameOver = true;
        emit gameOverChanged(m_gameOver);
        return;
    }

    if (tankCount < -3){
        --countTimeForEndGame;
        return;
    }
    stopGame();
}

void BattleCityField::stopGame()
{
    if (countTimeForEndGame < 1){
        tankMoveTimer->stop();
        emit endGame(m_tank_1stPlayer, m_fastTank_1stPlayer, m_armorTank_1stPlayer, m_heavyTank_1stPlayer,
                     m_tank_2stPlayer, m_fastTank_2stPlayer, m_armorTank_2stPlayer, m_heavyTank_2stPlayer);
    }
}

void BattleCityField::setField( Field * field, int x, int y)
{
    m_field[x][y] = field;
}

void BattleCityField::setObject( Object * object, int y, int x)
{
    m_object[y][x] = object;
}

const Object * BattleCityField::getObject(int i, int y)
{
    return m_object[i][y];
}

const Object * BattleCityField::getBulletAndFlash(int i, int y, int direct)
{
    switch (direct) {
    case 0:
        return m_bullet0[i][y];
    case 1:
        return m_bullet1[i][y];
    case 2:
        return m_bullet2[i][y];
    case 3:
        return m_bullet3[i][y];
    case -1:
        return m_flashArray[i][y];
    }
    return nullptr;
}

const Field * BattleCityField::getField(int i, int y)
{
    return m_field[i][y];
}

void BattleCityField::moveTank(bool bonusClock)
{

    int stepMoveObject = getNumberStep();
    int stepObject;
    srand(time(NULL));
    short random;

    for (int x = 0; x<26; x++){
        for (int y = 0; y < 26; y++){
            if (auto obj = m_object[y][x]){
                stepObject = m_object[y][x]->getStepOfObject();
                if ( ((stepMoveObject) == stepObject)){    // если был уже ход на этой итерации пропускаем
                    continue;
                }
                if (stepMoveObject % ( m_object[y][x]->getSpeed()) != 1)
                    continue;                                    // движение с разной скоростью танков
                m_object[y][x]->setStepOfObject(stepMoveObject);
                short dir = obj->getDirectMove();
                bool isPlayerTank  = (m_object[y][x] == m_tankOfPlayer) ? true : false ;
                bool isPlayerTank2 = (m_object[y][x] == m_tankOfPlayer2) ? true : false ;
                switch (dir) {
//---------------------------------------------------------------
                    case 0:{
                        if (isPlayerTank || isPlayerTank2){
                            m_object[y][x]->setDirectMove(-1);
                        }
                        bool space = true;
                        if(x > 23){
                            space = false;
                        }else if( !((m_object[y][x+2] == nullptr) && (m_object[y+1][x+2] == nullptr)) ){
                            space = false;
                        }else if( !(m_field[y][x+2]->getPassability()) ){
                            space = false;
                        }else if (!(m_field[y+1][x+2]->getPassability())){
                            space = false;
                        }else if ( y > 0 ){
                            if( m_object[y-1][x+2] != nullptr ){
                                space = false;
                            }
                        }

                        if (space){
                            if( !(bonusClock && m_object[y][x] != m_tankOfPlayer && m_object[y][x] != m_tankOfPlayer2) ){
                                m_object[y][x]->moveQQmlComponent(y,x+1);
                                m_object[y][x+1] = m_object[y][x];
                                m_object[y][x] = nullptr;
                                if (isPlayerTank){
                                    m_x = x+1;
                                    emit moveTankOfPlayerSignal(y,x,y,x+1,false);
                                    m_object[y][x+1]->rotationQQmlComponent(0);
                                    takingBonus(y,x+1,m_object[y][x+1]);
                                } else  if(isPlayerTank2) {
                                    m_x2 = x+1;
                                    emit moveTankOfPlayerSignal(y,x,y,x+1,true);
                                    m_object[y][x+1]->rotationQQmlComponent(0);
                                    takingBonus(y,x+1,m_object[y][x+1]);
                                } else {
                                    emit moveTankSignal(y,x,y,x+1);
                                }
                            }
                        } else {
                            if (isPlayerTank){
                                emit createTankOfPlayerSignal(y,x,false);
                                m_object[y][x]->rotationQQmlComponent(0);
                            } else if ( isPlayerTank2){
                                emit createTankOfPlayerSignal(y,x,true);
                                m_object[y][x]->rotationQQmlComponent(0);
                            } else  if (!bonusClock){
                                do{
                                    random = short(rand() % 4);
                                    m_object[y][x]->setDirectMove(random);

                                }while(random == dir);
                                m_object[y][x]->rotationQQmlComponent(m_object[y][x]->getDirectMove());
                                emit createTankSignal(y,x);
                            }

                         }
                        }
                        break;
//------------------------------------------------------------------------
                    case 1:{
                        if (isPlayerTank || isPlayerTank2){
                            m_object[y][x]->setDirectMove(-1);
                        }
                        bool space = true;
                        if(y < 1){
                            space = false;
                        }else if(y < 2){
                            if( !(m_field[y-1][x]->getPassability()) || !(m_field[y-1][x+1]->getPassability()) ){
                                space = false;
                            }
                        }else if( !((m_object[y-2][x] == nullptr) && (m_object[y-2][x+1]==nullptr))  ){
                            space = false;
                        }else if( !(m_field[y-1][x]->getPassability()) ){
                            space = false;
                        }else if (!(m_field[y-1][x+1]->getPassability())){
                            space = false;
                        }else if (x > 0){
                            if( m_object[y-2][x-1] != nullptr ){
                                space = false;
                            }
                        }
                        if (space){
                            if( !(bonusClock && m_object[y][x] != m_tankOfPlayer && m_object[y][x] != m_tankOfPlayer2) ){
                                m_object[y][x]->moveQQmlComponent(y-1,x);
                                m_object[y-1][x] = m_object[y][x];
                                m_object[y][x] = nullptr;
                                if (isPlayerTank){
                                    m_y = y-1;
                                    emit moveTankOfPlayerSignal(y,x,y-1,x,false);
                                    m_object[y-1][x]->rotationQQmlComponent(1);
                                    takingBonus(y-1,x, m_object[y-1][x]);
                                } else if(isPlayerTank2){
                                    m_y2 = y-1;
                                    emit moveTankOfPlayerSignal(y,x,y-1,x, true);
                                    m_object[y-1][x]->rotationQQmlComponent(1);
                                    takingBonus(y-1,x, m_object[y-1][x]);
                                } else {
                                    emit moveTankSignal(y,x,y-1,x);
                                }
                            }
                        }else {
                            if (isPlayerTank){
                                emit createTankOfPlayerSignal(y,x,false);
                                m_object[y][x]->rotationQQmlComponent(1);
                            } else if ( isPlayerTank2){
                                emit createTankOfPlayerSignal(y,x,true);
                                m_object[y][x]->rotationQQmlComponent(1);
                            } else if (!bonusClock){
                                do{
                                    random = short(rand() % 4);
                                    m_object[y][x]->setDirectMove(random);
                                }while(random == dir);
                                m_object[y][x]->rotationQQmlComponent(m_object[y][x]->getDirectMove());
                                emit createTankSignal(y,x);
                            }
                        }
                    }
                        break;
//-------------------------------------------------------------------------------
                    case 2:{
                        if (isPlayerTank || isPlayerTank2){
                            m_object[y][x]->setDirectMove(-1);
                        }
                        bool space = true;
                        if (x < 1) {
                            space = false;
                        } else if (x < 2){
                            if ( !(m_field[y][x-1]->getPassability()) || !(m_field[y+1][x-1]->getPassability()) ){
                                space = false;
                            }
                        } else  if( (m_object[y][x-2]!=nullptr) || (m_object[y+1][x-2] != nullptr) ){
                            space = false;
                        }else if( !(m_field[y][x-1]->getPassability()) ){
                            space = false;
                        }else if (!(m_field[y+1][x-1]->getPassability())){
                            space = false;
                        }else if (y > 0){
                            if( m_object[y-1][x-2] !=nullptr){
                                space = false;
                            }
                        }
                        if (space){
                            if( !(bonusClock && m_object[y][x] != m_tankOfPlayer && m_object[y][x] != m_tankOfPlayer2) ){
                                m_object[y][x]->moveQQmlComponent(y,x-1);
                                m_object[y][x-1] = m_object[y][x];
                                m_object[y][x] = nullptr;
                                if(isPlayerTank2){
                                    m_x2 = x-1;
                                    emit moveTankOfPlayerSignal(y,x,y,x-1, true);
                                    m_object[y][x-1]->rotationQQmlComponent(2);
                                    takingBonus(y,x-1, m_object[y][x-1]);
                                } else if (isPlayerTank){
                                    m_x = x-1;
                                    emit moveTankOfPlayerSignal(y,x,y,x-1, false);
                                    m_object[y][x-1]->rotationQQmlComponent(2);
                                    takingBonus(y,x-1, m_object[y][x-1]);
                                } else {
                                    emit moveTankSignal(y,x,y,x-1);
                                }
                            }
                        }else {
                            if (isPlayerTank){
                                emit createTankOfPlayerSignal(y,x,false);
                                m_object[y][x]->rotationQQmlComponent(2);
                            } else if ( isPlayerTank2){
                                emit createTankOfPlayerSignal(y,x,true);
                                m_object[y][x]->rotationQQmlComponent(2);
                            } else if (!bonusClock){
                                do{
                                    random = short(rand() % 4);
                                    m_object[y][x]->setDirectMove(random);
                                }while(random == dir);
                                m_object[y][x]->rotationQQmlComponent(m_object[y][x]->getDirectMove());
                                emit createTankSignal(y,x);
                            }
                        }
                    }
                         break;
//-------------------------------------------------------------------------------
                    case 3:{
                        if (isPlayerTank || isPlayerTank2){
                            m_object[y][x]->setDirectMove(-1);
                        }
                        bool space = true;
                        if(y > 23){
                            space = false;
                        }else if( (m_object[y+2][x] != nullptr) || (m_object[y+2][x+1] != nullptr) ){
                            space = false;
                        }else if( !(m_field[y+2][x]->getPassability()) ){
                            space = false;
                        }else if (!(m_field[y+2][x+1]->getPassability())){
                            space = false;
                        }else if (x > 0){
                            if( m_object[y+2][x-1] != nullptr){
                                space = false;
                            }
                        }
                        if (space){
                            if( !(bonusClock && m_object[y][x] != m_tankOfPlayer && m_object[y][x] != m_tankOfPlayer2) ){
                                m_object[y][x]->moveQQmlComponent(y+1,x);
                                m_object[y+1][x] = m_object[y][x];
                                m_object[y][x] = nullptr;
                                if (isPlayerTank2){
                                    m_y2 = y+1;
                                    emit moveTankOfPlayerSignal(y,x,y+1,x, true);
                                    m_object[y+1][x]->rotationQQmlComponent(3);
                                    takingBonus(y+1,x, m_object[y+1][x]);
                                } else if (isPlayerTank){
                                    m_y = y+1;
                                    emit moveTankOfPlayerSignal(y,x,y+1,x,false);
                                    m_object[y+1][x]->rotationQQmlComponent(3);
                                    takingBonus(y+1,x, m_object[y+1][x]);
                                } else {
                                    emit moveTankSignal(y,x,y+1,x);
                                }
                            }
                        } else {
                            if (isPlayerTank){
                                emit createTankOfPlayerSignal(y,x,false);
                                m_object[y][x]->rotationQQmlComponent(3);
                            } else if ( isPlayerTank2){
                                emit createTankOfPlayerSignal(y,x,true);
                                m_object[y][x]->rotationQQmlComponent(3);
                            } else if (!bonusClock){
                                do{
                                    random = short(rand() % 4);
                                    m_object[y][x]->setDirectMove(random);
                                }while(random == dir);
                                m_object[y][x]->rotationQQmlComponent(m_object[y][x]->getDirectMove());
                                emit createTankSignal(y,x);
                            }
                        }
                    }
                        break;
//-----------------------------------------------------------------------------------
                }
            }
        }
    }
    testHittingTank();
}

void BattleCityField::setNumberStep()
{
    numberStep++;
}

int BattleCityField::getNumberStep()
{
    return numberStep;
}

void BattleCityField::firstFillField()
{
    Factory_EmptyField * facEmptyField = new Factory_EmptyField();
    m_emptyField = facEmptyField->createField();

    m_Factory_of_Object[0] = new Factory_Flash();
    m_Factory_of_Object[1] = new Factory_Bullet();
    m_Factory_of_Object[2] = new Factory_TankForPlayer();
    m_Factory_of_Object[3] = new Factory_Tank();
    m_Factory_of_Object[4] = new Factory_FastTank();
    m_Factory_of_Object[5] = new Factory_ArmorTank();
    m_Factory_of_Object[6] = new Factory_HeavyArmorTank();


    Object * tank1 = m_Factory_of_Object[3]->createObject(m_engine, 0, 0);
    tank1->getQmlObject()->setProperty("speed", tank1->getSpeed());
    Object * tank2 = m_Factory_of_Object[3]->createObject(m_engine, 0, 12);
    tank2->getQmlObject()->setProperty("speed", tank2->getSpeed());
    Object * tank3 = m_Factory_of_Object[3]->createObject(m_engine, 0, 24);
    tank3->getQmlObject()->setProperty("speed", tank3->getSpeed());
    m_tankOfPlayer = m_Factory_of_Object[2]->createObject(m_engine, 24, 9);
//    emit createObject(m_tankOfPlayer->getQmlObject());

    m_tankOfPlayer2 = m_Factory_of_Object[2]->createObject(m_engine, 24, 15);
    m_tankOfPlayer2->setSecondPlayer(true);


    for (int i=0; i<26; i++){
        for (int j = 0; j < 26; j++){
             setField(m_emptyField, i, j);
        }
    }
//    fillField();
    setXTankPlayer(9);
    setYTankPlayer(24);
    setXTankPlayer2(15);
    setYTankPlayer2(24);
    setObject(m_tankOfPlayer ,m_y ,m_x);
    if (m_secondPlayer){
        setObject(m_tankOfPlayer2 ,m_y2 ,m_x2);
//        emit createObject(m_tankOfPlayer2->getQmlObject());
    }


    setObject(tank1,0,0);
    setObject(tank2,0,12);
    setObject(tank3,0,24);
}

void BattleCityField::shotPlayer(short y, short x, int direct, bool secondPlayer)
{
    bool fastBullet = secondPlayer ? m_tankOfPlayer2->getFastBullet() : m_tankOfPlayer->getFastBullet();
    bool superBullet = secondPlayer ? m_tankOfPlayer2->isSuperBullet() : m_tankOfPlayer->isSuperBullet();

    switch (direct) {
        case 0:
            createBullet(y+1,x+2,direct,false,secondPlayer,fastBullet,superBullet);
            break;
        case 1:
            createBullet(y,x+1,direct,false,secondPlayer,fastBullet,superBullet);
            break;
        case 2:
            createBullet(y+1,x,direct,false,secondPlayer,fastBullet, superBullet);
            break;
        case 3:
            createBullet(y+2,x+1,direct,false,secondPlayer,fastBullet, superBullet);
            break;
    }
}

void BattleCityField::shot()
{
    for (int x=0; x < 26; x++){
        for (int y = 0; y < 26; y ++){
            if (m_object[y][x] != nullptr){
                if (m_object[y][x]->getEnemy()){
                    int directMove = m_object[y][x]->getDirectMove();
                    switch (directMove) {
                        case 0:
                            createBullet(y+1,x+2,directMove, true, false, m_object[y][x]->getFastBullet(),false);
                            break;
                        case 1:
                            createBullet(y,x+1,directMove,true,false, m_object[y][x]->getFastBullet(),false);
                            break;
                        case 2:
                            createBullet(y+1,x,directMove,true,false, m_object[y][x]->getFastBullet(),false);
                            break;
                        case 3:
                            createBullet(y+2,x+1,directMove,true,false, m_object[y][x]->getFastBullet(),false);
                            break;
                    }
                }
            }
        }
    }
}

void BattleCityField::createBullet(const short y , const short x, int const direct, bool enemy, bool secondPlayer,
                                   bool fastBullet, bool superBullet)
{
    Object * temp = m_Factory_of_Object[1]->createObject (m_engine,y,x);

    temp->setDirectMove(direct);
    temp->rotationQQmlComponent(direct);
    temp->setEnemy(enemy);
    temp->setSecondPlayer(secondPlayer);
    temp->setFastBullet(fastBullet);
    temp->setSpeed(2-fastBullet);
    temp->setStepOfObject(numberStep - 10);
     if (superBullet)
         temp->setSuperBullet();



    switch (direct) {
        case 0:
            if(m_bullet0[y][x] == nullptr /*&& m_bullet0[y][x-1] == nullptr*/){
                m_bullet0[y][x] = temp;
                emit newBulletSignal_0((y),(x));
                emit createBulletFlashSignal(temp->getQmlObject());
            }else{
                delete temp;
            }
            break;
        case 1:
            if(m_bullet1[y][x] == nullptr /*&& m_bullet1[y+1][x] == nullptr*/){
                m_bullet1[y][x] = temp;
                emit newBulletSignal_1((y),(x));
                emit createBulletFlashSignal(temp->getQmlObject());
            }else{
                delete temp;
            }
            break;
        case 2:
            if(m_bullet2[y][x] == nullptr /*&& m_bullet2[y][x+1] == nullptr*/){
                m_bullet2[y][x] = temp;
                emit newBulletSignal_2((y),(x));
                emit createBulletFlashSignal(temp->getQmlObject());
            }else{
                delete temp;
            }
            break;
        case 3:
            if(m_bullet3[y][x] == nullptr /*&& m_bullet3[y-1][x] == nullptr*/){
                m_bullet3[y][x] = temp;
                emit newBulletSignal_3((y),(x));
                emit createBulletFlashSignal(temp->getQmlObject());
            }else{
                delete temp;
            }
            break;
        default:
            delete temp;
            break;
    }
}

void BattleCityField::createFlash(const short y, const short x)
{
    if (m_flashArray[y][x] != nullptr)
        delete m_flashArray[y][x];
    m_flashArray[y][x] = m_Factory_of_Object[0]->createObject(m_engine, y, x);
    m_flashArray[y][x]->setStepOfObject(numberStep);
    emit newFlashSignal(y,x);
    emit createBulletFlashSignal(m_flashArray[y][x]->getQmlObject());
}

void BattleCityField::createTank()
{
    --tankCount;
    if(tankCount < 0){
        return;
    }
    Object* newTank = m_Factory_of_Object[rand()%4 + 3]->createObject(m_engine , 0,0);
    newTank->setStepOfObject(numberStep/3);
    if ( true/*tankCount == 4 || tankCount == 8 || tankCount == 14*/){    // !!!!!!!!!!!!!!!!!!!!!!!
        newTank->setBonus();
        if (m_bonus != nullptr){
            delete m_bonus;
            m_bonus = nullptr;
        }

    }

    srand(time(NULL));
    bool isFree = false;
//------------------------------------------------------------
    int var = rand()%3;
    for (int i = 0; i < 3; ++i){
        switch (var) {
        case 0:
            if ( (m_object[0][0] == nullptr) && (m_object[0][1] == nullptr) && (m_object[1][0] == nullptr) && (m_object[1][1] == nullptr) && (!isFree)){
//                    setObject(newTank,0,0);
                m_object[0][0] = newTank;
                newTank->moveQQmlComponent(0,0);
                emit createTankSignal(0,0);
                isFree = true;
                qDebug() << "in case 0 newTank" << newTank;
                qDebug() << "Property imageX:" << QQmlProperty::read(newTank->getQmlObject(), "imageX").toInt();
                qDebug() << "Property imageY:" << QQmlProperty::read(newTank->getQmlObject(), "imageY").toInt();
            }
            var = (var+1) % 3;
            break;
        case 1:
            if ((m_object[0][12] == nullptr) && (m_object[0][13] == nullptr) && (m_object[1][12] == nullptr) && (m_object[1][13] == nullptr) && (!isFree)){
//                    setObject(newTank,0,12);
                m_object[0][12] = newTank;
                newTank->moveQQmlComponent(0,12);
                emit createTankSignal(0,12);
                isFree = true;
                qDebug() << "in case 1"<< newTank;
                qDebug() << "Property imageX:" << QQmlProperty::read(newTank->getQmlObject(), "imageX").toInt();
                qDebug() << "Property imageY:" << QQmlProperty::read(newTank->getQmlObject(), "imageY").toInt();
            }
            var = (var+1) % 3;
            break;
        case 2:
            if ((m_object[0][24] == nullptr) && (m_object[1][24] == nullptr) && (m_object[1][25] == nullptr) && (m_object[0][25] == nullptr) && (!isFree)){
//                setObject(newTank,0,24);
                m_object[0][24] = newTank;
                newTank->moveQQmlComponent(0,24);
                emit createTankSignal(0,24);
                isFree = true;
                qDebug() << "in case 2"<< newTank;
                qDebug() << "Property imageX:" << QQmlProperty::read(newTank->getQmlObject(), "imageX").toInt();
                qDebug() << "Property imageY:" << QQmlProperty::read(newTank->getQmlObject(), "imageY").toInt();
            }
            var = (var+1) % 3;
            break;
        }
    }
    if (!isFree){
        m_notEmptySpaceForTank = true;
        return;
    }


//------------------------------------------------------------
//    while (!isFree) {
//        int var = rand()%3;
//        switch (var) {
//            case 0:
//                if (m_object[0][0] == nullptr){
//                    setObject(newTank,0,0);
//                    newTank->moveQQmlComponent(0,0);
//                    emit createTankSignal(0,0);
//                    isFree = true;
//                }
//                break;
//            case 1:
//                if (m_object[0][12] == nullptr){
//                    setObject(newTank,0,12);
//                    newTank->moveQQmlComponent(0,12);
//                    emit createTankSignal(0,12);
//                    isFree = true;
//                }
//                break;
//            case 2:
//                if (m_object[0][24] == nullptr){
//                    setObject(newTank,0,24);
//                    newTank->moveQQmlComponent(0,24);
//                    emit createTankSignal(0,24);
//                    isFree = true;
//                }
//                break;
//        }
//    }
//-------------------------------------------------------------------------
//    --tankCount;

    newTank->getQmlObject()->setProperty("speed", newTank->getSpeed());
    emit createObject(newTank->getQmlObject());
    createTankCountSignal();
    qDebug() << "out Creare Tank";
}

void BattleCityField::deleteBullet(const short y, const short x, const int direct)
{
    switch (direct) {
        case 0:
            delete m_bullet0[y][x];
            m_bullet0[y][x] = nullptr;
            emit newBulletSignal_0(y,x);
            break;
        case 1:
            delete m_bullet1[y][x];
            m_bullet1[y][x] = nullptr;
            emit newBulletSignal_1(y,x);
            break;
        case 2:
            delete m_bullet2[y][x];
            m_bullet2[y][x] = nullptr;
            emit newBulletSignal_2(y,x);
            break;
        case 3:
            delete m_bullet3[y][x];
            m_bullet3[y][x] = nullptr;
            emit newBulletSignal_3(y,x);
            break;
        case -1:
            delete m_flashArray[y][x];
            m_flashArray[y][x] = nullptr;
            emit newFlashSignal(y,x);
            break;
        default:
            break;
    }
}

void BattleCityField::deleteTank(const short y, const short x, bool createNewTank)
 {
    qDebug() << "in delete Tank";
    if (m_object[y][x] != nullptr)
            delete m_object[y][x];
    m_object[y][x] = nullptr;
    emit deleteTankSignal(y,x);
    if (createNewTank){
        createTank();
    }
    qDebug() << "out delete Tank";

}

void BattleCityField::deleteField(const short y, const short x, bool isSuperBullet)
{
    if (isSuperBullet){
        if (m_field[y][x]->isSuperDestruction()){
            delete m_field[y][x];
            m_field[y][x] = m_emptyField;
            emit deleteFieldSignal(y,x);
        }
    } else {
        if(m_field[y][x]->getDestruction() ){
            delete m_field[y][x];
            m_field[y][x] = m_emptyField;
            emit deleteFieldSignal(y,x);
        }
    }
    m_field[y][x]->destroed(); // для перерисовки базы при попадании
}

void BattleCityField::resetTankOfPlayer(short y, short x)
{
    if (m_countLife > 0){
        m_tankOfPlayer->getQmlObject()->setProperty("speed", 0);
        m_tankOfPlayer->getQmlObject()->setProperty("speedRotation", 0);
        m_tankOfPlayer->moveQQmlComponent(24,9);
        m_tankOfPlayer->rotationQQmlComponent(1);
        m_tankOfPlayer->getQmlObject()->setProperty("speed", 4);
        m_tankOfPlayer->getQmlObject()->setProperty("speedRotation", 1);
        m_object[y][x] = nullptr;
        setXTankPlayer(9);
        setYTankPlayer(24);
        m_tankOfPlayer->setDirectMove(-1);
        setObject(m_tankOfPlayer,m_y,m_x);
        lastDirect = 1;
        emit createTankSignal(y,x);
        emit createTankOfPlayerSignal(m_y,m_x,false);
        --m_countLife;
        emit lifeCountChanged(m_countLife);
    } else{
        m_tankOfPlayer->getQmlObject()->setProperty("speed", 0);
        m_tankOfPlayer->moveQQmlComponent(50,50);
        m_tankOfPlayer->rotationQQmlComponent(1);
//        m_tankOfPlayer->getQmlObject()->setProperty("speed", 4);
        --m_countLife;
        m_object[y][x] = nullptr;
        setXTankPlayer(9);
        setYTankPlayer(24);
        m_tankOfPlayer->setDirectMove(-1);
        lastDirect = 1;
        emit createTankSignal(y,x);
//        m_gameOver = true;
    }
    emit lifeCountChanged(m_countLife);
}

void BattleCityField::resetTankOfPlayer2(short y, short x)
{
    if (m_countLife2 > 0){
        m_tankOfPlayer2->getQmlObject()->setProperty("speed", 0);
        m_tankOfPlayer2->getQmlObject()->setProperty("speedRotation", 0);
        m_tankOfPlayer2->moveQQmlComponent(24,15);
        m_tankOfPlayer2->rotationQQmlComponent(1);
        m_tankOfPlayer2->getQmlObject()->setProperty("speed", 4);
        m_tankOfPlayer2->getQmlObject()->setProperty("speedRotation", 1);
        m_object[y][x] = nullptr;
        setXTankPlayer2(15);
        setYTankPlayer2(24);
        m_tankOfPlayer2->setDirectMove(-1);
        setObject(m_tankOfPlayer2,m_y2,m_x2);
        lastDirect2 = 1;
        emit createTankSignal(y,x);
        emit createTankOfPlayerSignal(m_y2,m_x2,true);
        --m_countLife2;
        emit lifeCount2Changed(m_countLife2);
    } else{
        m_tankOfPlayer2->getQmlObject()->setProperty("speed", 0);
        m_tankOfPlayer2->moveQQmlComponent(50,50);
        m_tankOfPlayer2->rotationQQmlComponent(1);
//        m_tankOfPlayer2->getQmlObject()->setProperty("speed", 4);
        --m_countLife2;
        m_object[y][x] = nullptr;
        setXTankPlayer2(15);
        setYTankPlayer2(24);
        m_tankOfPlayer2->setDirectMove(-1);
        lastDirect2 = 1;
        emit createTankSignal(y,x);
//        m_gameOver = true;
    }
    emit lifeCount2Changed(m_countLife2);
}

void BattleCityField::moveBullet()
{

    int step = getNumberStep();

    for(short y = 0; y < 27; ++y){
        for(short x=0; x < 27; ++x){
            if ( m_flashArray[y][x] != nullptr ){
                if (m_flashArray[y][x]->getStepOfObject() < step){
                    deleteBullet(y,x,-1);
                }
            }
        }
    }
    bool isPlayerBullet;
    bool isEnemyBullet;
    bool isBullet_0;
    bool isBullet_1;
    bool isBullet_2;
    bool isBullet_3;
    for(short y = 1; y < 26; ++y){
        for(short x=1; x < 26; ++x){
            isPlayerBullet = false;
            isEnemyBullet = false;
            isBullet_0 = false;
            isBullet_1 = false;
            isBullet_2 = false;
            isBullet_3 = false;
            if (m_bullet0[y][x] != nullptr){
                isBullet_0 = true;
                m_bullet0[y][x]->getEnemy() ? isEnemyBullet = true : isPlayerBullet = true;
            }
            if (m_bullet1[y][x] != nullptr){
                isBullet_1 = true;
                m_bullet1[y][x]->getEnemy() ? isEnemyBullet = true : isPlayerBullet = true;
            }
            if (m_bullet2[y][x] != nullptr){
                isBullet_2 = true;
                m_bullet2[y][x]->getEnemy() ? isEnemyBullet = true : isPlayerBullet = true;
            }
            if (m_bullet3[y][x] != nullptr){
                isBullet_3 = true;
                m_bullet3[y][x]->getEnemy() ? isEnemyBullet = true : isPlayerBullet = true;
            }
            if (isPlayerBullet && isEnemyBullet){
                if (isBullet_0) deleteBullet(y,x,0);
                if (isBullet_1) deleteBullet(y,x,1);
                if (isBullet_2) deleteBullet(y,x,2);
                if (isBullet_3) deleteBullet(y,x,3);
                createFlash(y,x);
            }
        }
    }
    bool oddStep = (step & 1) ? true : false;
    for(short y = 0; y < 27; ++y){
        for(short x=0; x < 27; ++x){
            if ( m_bullet1[y][x] != nullptr){
                if (m_bullet1[y][x]->getStepOfObject() != step){
                    if ( oddStep && !m_bullet1[y][x]->getFastBullet())
                        continue;                                       // медленная пуля на нечетном шаге не двигается
                    m_bullet1[y][x]->setStepOfObject(step);
                    if (y > 0){
                        if (!(m_field[y-1][x]->getFlyPassability() && m_field[y-1][x-1]->getFlyPassability())){
                            deleteField(y-1,x,m_bullet1[y][x]->isSuperBullet());
                            deleteField(y-1,x-1, m_bullet1[y][x]->isSuperBullet());
                            deleteBullet(y,x,1);
                            createFlash(y,x);
                        } else {
                            if (m_bullet1[y-1][x] != nullptr){
                                if (m_bullet1[y-1][x]->getEnemy() != m_bullet1[y][x]->getEnemy()){
                                    deleteBullet(y-1,x,1);
                                    deleteBullet(y,x,1);
                                    createFlash(y-1,x);
                                } else {
                                    deleteBullet(y-1,x,1);
                                    m_bullet1[y-1][x] = m_bullet1[y][x];
                                    m_bullet1[y][x] = nullptr;
                                    emit moveBulletSignal_1(y, x, y-1, x);
                                }
                            } else {
                                m_bullet1[y][x]->moveQQmlComponent(y-1,x);
                                m_bullet1[y-1][x] = m_bullet1[y][x];
                                m_bullet1[y][x] = nullptr;
                                emit moveBulletSignal_1(y, x, y-1, x);
                            }
                        }
                    } else {
                        deleteBullet(y,x,1);
                        createFlash(y,x);
                    }
                }
            }
            if ( m_bullet2[y][x] != nullptr){
                if (m_bullet2[y][x]->getStepOfObject() != step){
                    if ( oddStep && !m_bullet2[y][x]->getFastBullet())
                        continue;                                       // медленная пуля на нечетном шаге не двигается
                    m_bullet2[y][x]->setStepOfObject(step);
                    if (x > 0){
                        if(!(m_field[y-1][x-1]->getFlyPassability() && m_field[y][x-1]->getFlyPassability())){
                            deleteField(y-1,x-1,m_bullet2[y][x]->isSuperBullet());
                            deleteField(y,x-1,m_bullet2[y][x]->isSuperBullet());
                            deleteBullet(y,x,2);
                            createFlash(y,x);
                        } else {
                            if (m_bullet2[y][x-1] != nullptr){
                                if (m_bullet2[y][x-1]->getEnemy() != m_bullet2[y][x]->getEnemy()){
                                    deleteBullet(y,x-1,2);
                                    deleteBullet(y,x,2);
                                    createFlash(y,x-1);
                                } else {
                                    deleteBullet(y,x-1,2);
                                    m_bullet2[y][x-1] = m_bullet2[y][x];
                                    m_bullet2[y][x] = nullptr;
                                    emit moveBulletSignal_2(y, x, y, x-1);
                                }
                            } else {
                                m_bullet2[y][x]->moveQQmlComponent(y,x-1);
                                m_bullet2[y][x-1] = m_bullet2[y][x];
                                m_bullet2[y][x] = nullptr;
                                emit moveBulletSignal_2(y, x, y, x-1);
                            }
                        }
                    } else {
                        deleteBullet(y,x,2);
                        createFlash(y,x);
                    }
                }
            }
        }
    }

    for(short y = 26; y > -1; --y){
        for(short x=26; x > -1; --x){
            if ( m_bullet0[y][x] != nullptr){
                if (m_bullet0[y][x]->getStepOfObject() != step){
                    if ( oddStep && !m_bullet0[y][x]->getFastBullet())
                        continue;                                       // медленная пуля на нечетном шаге не двигается
                    m_bullet0[y][x]->setStepOfObject(step);
                    if (x < 26){
                        if ( !(m_field[y][x]->getFlyPassability() && m_field[y-1][x]->getFlyPassability())){
                            deleteField(y,x,m_bullet0[y][x]->isSuperBullet());
                            deleteField(y-1,x,m_bullet0[y][x]->isSuperBullet());
                            deleteBullet(y,x,0);
                            createFlash(y,x);
                        } else {
                            if (m_bullet0[y][x+1] != nullptr){
                                if (m_bullet0[y][x+1]->getEnemy() != m_bullet0[y][x]->getEnemy()){
                                    deleteBullet(y,x+1,0);
                                    deleteBullet(y,x,0);
                                    createFlash(y,x+1);
                                } else {
                                    deleteBullet(y,x+1,0);
                                    m_bullet0[y][x+1] = m_bullet0[y][x];
                                    m_bullet0[y][x] = nullptr;
                                    emit moveBulletSignal_0(y, x, y, x+1);
                                }
                            } else {
                                m_bullet0[y][x]->moveQQmlComponent(y,x+1);
                                m_bullet0[y][x+1] = m_bullet0[y][x];
                                m_bullet0[y][x] = nullptr;
                                emit moveBulletSignal_0(y, x, y, x+1);
                            }
                        }
                    } else {
                        deleteBullet(y,x,0);
                        createFlash(y,x);
                    }
                }
            }
            if ( m_bullet3[y][x] != nullptr){
                if (m_bullet3[y][x]->getStepOfObject() != step){
                    if ( oddStep && !m_bullet3[y][x]->getFastBullet())
                        continue;                                       // медленная пуля на нечетном шаге не двигается
                    m_bullet3[y][x]->setStepOfObject(step);
                    if (y < 26){
                        if(!(m_field[y][x]->getFlyPassability() && m_field[y][x-1]->getFlyPassability())){
                            deleteField(y,x,m_bullet3[y][x]->isSuperBullet());
                            deleteField(y,x-1,m_bullet3[y][x]->isSuperBullet());
                            deleteBullet(y,x,3);
                            createFlash(y,x);
                        } else {
                            if (m_bullet3[y+1][x] != nullptr){
                                if (m_bullet3[y+1][x]->getEnemy() != m_bullet3[y][x]->getEnemy()){
                                    deleteBullet(y+1,x,3);
                                    deleteBullet(y,x,3);
                                    createFlash(y+1,x);
                                } else {
                                    deleteBullet(y+1,x,3);
                                    m_bullet3[y+1][x] = m_bullet3[y][x];
                                    m_bullet3[y][x] = nullptr;
                                    emit moveBulletSignal_3(y, x, short(y+1), x);
                                }
                            } else {
                                m_bullet3[y][x]->moveQQmlComponent(y+1,x);
                                m_bullet3[y+1][x] = m_bullet3[y][x];
                                m_bullet3[y][x] = nullptr;
                                emit moveBulletSignal_3(y, x, short(y+1), x);
                            }
                        }
                    } else {
                        deleteBullet(y,x,3);
                        createFlash(y,x);
                    }
                }
            }
        }
    }

    testHittingTank();
    for(short y = 1; y < 26; ++y){
        for(short x=1; x < 26; ++x){
            if ((m_bullet1[y][x] != nullptr) && m_bullet3[y+1][x] != nullptr){
                if (m_bullet1[y][x]->getEnemy() != m_bullet3[y+1][x]->getEnemy()){
                    deleteBullet(y,x,1);
                    deleteBullet(y+1,x,3);
                    createFlash(y+1,x);
                }
            }
            if ((m_bullet2[y][x] != nullptr) && m_bullet0[y][x+1] != nullptr){
                if (m_bullet2[y][x]->getEnemy() != m_bullet0[y][x+1]->getEnemy()){
                    deleteBullet(y,x,2);
                    deleteBullet(y,x+1,0);
                    createFlash(y,x+1);
                }
            }
        }
    }
}

void BattleCityField::testHittingTank()
{
    for(short y = 1; y < 26; ++y){
        for(short x=1; x < 26; ++x){
            if ( m_object[y-1][x-1] != nullptr){
                bool isEnemyTank = m_object[y-1][x-1]->getEnemy();
                if (m_bullet0[y][x] != nullptr){
                    if (m_bullet0[y][x]->getEnemy() != isEnemyTank){
                        if (m_object[y-1][x-1] == m_tankOfPlayer2){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer2(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else if (m_object[y-1][x-1] == m_tankOfPlayer){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else {
                            if (m_object[y-1][x-1]->getBonus()){
                                m_object[y-1][x-1]->resetBonus();
                                if (m_bonus != nullptr)
                                    delete m_bonus;
                                m_bonus = new Bonus(m_engine);
                                emit createObject(m_bonus->getQmlBonus());
                            }
                            if (m_object[y-1][x-1]->getArmor() < 2){
                                qDebug() << " m_bullet0[y][x]->getSecondPlayer() " << m_bullet0[y][x]->getSecondPlayer() <<" m_object[y-1][x-1]->getIdTank()) " << m_object[y-1][x-1]->getIdTank();
                                countHittingEachPlayer(m_bullet0[y][x]->getSecondPlayer(), m_object[y-1][x-1]->getIdTank());
                                deleteTank(y-1, x-1, true);
                            } else
                                m_object[y-1][x-1]->hitting();
                        }
                        deleteBullet(y,x,0);
                        createFlash(y,x);
                    }
                } else if (m_bullet1[y][x] != nullptr){
                    if (m_bullet1[y][x]->getEnemy() != isEnemyTank){
                        if (m_object[y-1][x-1] == m_tankOfPlayer){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else if (m_object[y-1][x-1] == m_tankOfPlayer2){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer2(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else {
                            if (m_object[y-1][x-1]->getBonus()){
                                m_object[y-1][x-1]->resetBonus();
                                if (m_bonus != nullptr)
                                    delete m_bonus;
                                m_bonus = new Bonus(m_engine);
                                emit createObject(m_bonus->getQmlBonus());
                            }

                            if (m_object[y-1][x-1]->getArmor() < 2){
                                qDebug() << " m_bullet1[y][x]->getSecondPlayer() " << m_bullet1[y][x]->getSecondPlayer() <<" m_object[y-1][x-1]->getIdTank()) " << m_object[y-1][x-1]->getIdTank();
                                countHittingEachPlayer(m_bullet1[y][x]->getSecondPlayer(), m_object[y-1][x-1]->getIdTank());
                                deleteTank(y-1, x-1, true);
                            } else
                                m_object[y-1][x-1]->hitting();
                        }
                        deleteBullet(y,x,1);
                        createFlash(y,x);
                    }
                } else if (m_bullet2[y][x] != nullptr){
                    if (m_bullet2[y][x]->getEnemy() != isEnemyTank){
                        if (m_object[y-1][x-1] == m_tankOfPlayer){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else if (m_object[y-1][x-1] == m_tankOfPlayer2){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer2(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else {
                            if (m_object[y-1][x-1]->getBonus()){
                                m_object[y-1][x-1]->resetBonus();
                                if (m_bonus != nullptr)
                                    delete m_bonus;
                                m_bonus = new Bonus(m_engine);
                                emit createObject(m_bonus->getQmlBonus());
                            }
                            if (m_object[y-1][x-1]->getArmor() < 2){
                                countHittingEachPlayer(m_bullet2[y][x]->getSecondPlayer(), m_object[y-1][x-1]->getIdTank());
                                deleteTank(y-1, x-1, true);
                            }  else
                                m_object[y-1][x-1]->hitting();
                        }
                        deleteBullet(y,x,2);
                        createFlash(y,x);
                    }
                } else if (m_bullet3[y][x] != nullptr){
                    if (m_bullet3[y][x]->getEnemy() != isEnemyTank){
                        if (m_object[y-1][x-1] == m_tankOfPlayer){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else if (m_object[y-1][x-1] == m_tankOfPlayer2){
                            qDebug() << "m_object[y-1][x-1]->getArmor() = " << m_object[y-1][x-1]->getArmor();
                            if(m_object[y-1][x-1]->getArmor() < 2){
                                resetTankOfPlayer2(y-1,x-1);
                            }else {
                                m_object[y-1][x-1]->hitting();
                            }
                        } else {
                            if (m_object[y-1][x-1]->getBonus()){
                                m_object[y-1][x-1]->resetBonus();
                                if (m_bonus != nullptr)
                                    delete m_bonus;
                                m_bonus = new Bonus(m_engine);
                                emit createObject(m_bonus->getQmlBonus());
                            }
                            if (m_object[y-1][x-1]->getArmor() < 2){
                                countHittingEachPlayer(m_bullet3[y][x]->getSecondPlayer(), m_object[y-1][x-1]->getIdTank());
                                deleteTank(y-1, x-1, true);
                            } else
                                m_object[y-1][x-1]->hitting();
                        }
                        deleteBullet(y,x,3);
                        createFlash(y,x);
                    }
                }
            }
        }
    }
}

void BattleCityField::resetModel()
{

    for (int x = 0; x<26; x++){
        for (int y = 0; y < 26; y++){
            if(m_object[y][x] != nullptr){
                if(m_object[y][x] != m_tankOfPlayer && m_object[y][x] !=m_tankOfPlayer2)
                    delete m_object[y][x];
                m_object[y][x] = nullptr;
            }
            if (m_field[y][x] != nullptr && m_field[y][x] != m_emptyField){
                delete m_field[y][x];
                m_field[y][x] = m_emptyField;
            }
        }
    }
    for (int x = 0; x<27; x++){
        for (int y = 0; y < 27; y++){
            if (m_bullet0 [y][x] != nullptr){
                delete m_bullet0 [y][x];
                m_bullet0[y][x] = nullptr;
            }
            if (m_bullet1[y][x] != nullptr){
                delete m_bullet1[y][x];
                m_bullet1[y][x] = nullptr;
            }
            if (m_bullet2[y][x] != nullptr){
                delete m_bullet2[y][x];
                m_bullet2[y][x] = nullptr;
            }
            if (m_bullet3[y][x] != nullptr){
                delete m_bullet3[y][x];
                m_bullet3[y][x] = nullptr;
            }
            if (m_flashArray[y][x] != nullptr){
                delete m_flashArray[y][x];
                m_flashArray[y][x] = nullptr;
            }
        }
    }
    Object * tank1 = m_Factory_of_Object[3]->createObject(m_engine , 0,0);
    tank1->getQmlObject()->setProperty("speed", tank1->getSpeed());
//    emit createObject(tank1->getQmlObject());
    Object * tank2 = m_Factory_of_Object[3]->createObject(m_engine , 0,12);
    tank2->getQmlObject()->setProperty("speed", tank2->getSpeed());
//    emit createObject(tank2->getQmlObject());
    Object * tank3 = m_Factory_of_Object[3]->createObject(m_engine , 0,24);
    tank3->getQmlObject()->setProperty("speed", tank3->getSpeed());
//    emit createObject(tank3->getQmlObject());
    setObject(tank1,0,0);
    setObject(tank2,0,12);
    setObject(tank3,0,24);
    setXTankPlayer(9);
    setYTankPlayer(24);
    setXTankPlayer2(15);
    setYTankPlayer2(24);
//    m_tankOfPlayer->rotationQQmlComponent(1);
//    m_tankOfPlayer->moveQQmlComponent(24,9);
    m_tankOfPlayer->setDirectMove(-1);
    m_tankOfPlayer->getQmlObject()->setProperty("helm", false);
    m_countHelmBonusTank1 = -1;
//    m_tankOfPlayer2->rotationQQmlComponent(1);
//    m_tankOfPlayer2->moveQQmlComponent(24,15);
    m_tankOfPlayer2->setDirectMove(-1);
    m_tankOfPlayer2->getQmlObject()->setProperty("helm", false);
    m_notEmptySpaceForTank = false;
    m_countDelayCreationNextTank = DELAY_CREATION_NEXT_TANK;
    m_countHelmBonusTank2 = -1;
    m_countClockBonus = -1;
    m_countShovelBonus = -1;
    isShotPlayer = false;
    isShotPlayer2 = false;
    numberStep = 0;
    numberStepBullet = 0;
    countTimeForEndGame = CONST_TIME_FOR_END_GAME;
    lastDirect = 1;
    lastDirect2 = 1;
    tankCount = constTankCount;
    m_tank_1stPlayer = 0;
    m_tank_2stPlayer = 0;
    m_fastTank_1stPlayer = 0;
    m_fastTank_2stPlayer = 0;
    m_armorTank_1stPlayer = 0;
    m_armorTank_2stPlayer = 0;
    m_heavyTank_1stPlayer = 0;
    m_heavyTank_2stPlayer = 0;

    if (m_gameOver){
        setObject(m_tankOfPlayer ,m_y ,m_x);
        m_tankOfPlayer->getQmlObject()->setProperty("speed", 0);
        m_tankOfPlayer->moveQQmlComponent(24,9);
        m_tankOfPlayer->getQmlObject()->setProperty("speed", 4);
        m_tankOfPlayer->getQmlObject()->setProperty("speedRotation", 0);
        m_tankOfPlayer->rotationQQmlComponent(1);
        m_tankOfPlayer->getQmlObject()->setProperty("speedRotation", 1);
        m_tankOfPlayer->resetTank();
        if (m_secondPlayer){
            m_tankOfPlayer2->resetTank();
            setObject(m_tankOfPlayer2 ,m_y2 ,m_x2);
            m_tankOfPlayer2->getQmlObject()->setProperty("speed", 0);
            m_tankOfPlayer2->moveQQmlComponent(24,15);
            m_tankOfPlayer2->getQmlObject()->setProperty("speed", 4);
            m_tankOfPlayer2->getQmlObject()->setProperty("speedRotation", 0);
            m_tankOfPlayer2->rotationQQmlComponent(1);
            m_tankOfPlayer2->getQmlObject()->setProperty("speedRotation", 1);
        }
        m_countLife = CONST_LIFE;
        m_countLife2 = CONST_LIFE;
        m_stage = 1;
        emit stageChanged(m_stage);
    } else {
        ++m_stage;
        emit stageChanged(m_stage);
        if (m_countLife > -1){
            setObject(m_tankOfPlayer ,m_y ,m_x);
            m_tankOfPlayer->getQmlObject()->setProperty("speed", 0);
            m_tankOfPlayer->getQmlObject()->setProperty("speedRotation", 0);
            m_tankOfPlayer->moveQQmlComponent(24,9);
            m_tankOfPlayer->rotationQQmlComponent(1);
            m_tankOfPlayer->getQmlObject()->setProperty("speed", 4);
            m_tankOfPlayer->getQmlObject()->setProperty("speedRotation", 1);
        }
        if (m_secondPlayer)
            if(m_countLife2 > -1){
                setObject(m_tankOfPlayer2 ,m_y2 ,m_x2);
                m_tankOfPlayer2->getQmlObject()->setProperty("speed", 0);
                m_tankOfPlayer2->getQmlObject()->setProperty("speedRotation", 0);
                m_tankOfPlayer2->moveQQmlComponent(24,15);
                m_tankOfPlayer2->rotationQQmlComponent(1);
                m_tankOfPlayer2->getQmlObject()->setProperty("speed", 4);
                m_tankOfPlayer2->getQmlObject()->setProperty("speedRotation", 1);
            }
    }
//    emit resetModelSignal();
    m_gameOver = false;

//    connect(tankMoveTimer, &QTimer::timeout, this, &BattleCityField::goMoveTimer);
    qDebug() << "resetModel()";
}

void BattleCityField::fillFieldFromTheFile(std::vector<char> &refFile)
{
    Factory_Water      * facWater = new Factory_Water();
    Factory_BrickWall  * facBrickWall = new Factory_BrickWall();
    Factory_SteelWall  * facStellWall = new Factory_SteelWall();
    Factory_Forest     * facForest = new Factory_Forest();
    Factory_IceRoad    * facIceRoad = new Factory_IceRoad();
    Factory_Throne1    * facThrone1 = new Factory_Throne1();
    Factory_Throne2    * facThrone2 = new Factory_Throne2();
    Factory_Throne3    * facThrone3 = new Factory_Throne3();
    Factory_Throne4    * facThrone4 = new Factory_Throne4();

    if (refFile.size() != 676)
        qDebug() << "ERROR FILE (SIZE VECTOR)";
    short sizeVector = refFile.size();
    short yyy;
    short xxx;
    int tempInt;

    for (int i = 0; i <sizeVector; ++i){
        yyy = i/26;
        xxx = i % 26;
        tempInt = int(refFile[i] - '0');
        switch (tempInt) {
        case 0:
            setField(facWater->createField(), yyy, xxx);
            break;
        case 1:
            setField(facBrickWall->createField(), yyy, xxx);
            break;
        case 2:
            setField(facStellWall->createField(), yyy, xxx);
            break;
        case 3:
            setField(facForest->createField(), yyy, xxx);
            break;
        case 4:
            setField(facIceRoad->createField(), yyy, xxx);
            break;
        default:
            setField(m_emptyField, yyy, xxx);
            break;
        }
    }
    if (m_field[24][12] != m_emptyField )
        delete m_field[24][12];
        setField(facThrone1->createField(), 24, 12);

    if (m_field[25][12] != m_emptyField )
        delete m_field[25][12];
        setField(facThrone3->createField(), 25, 12);

    if (m_field[24][13] != m_emptyField )
        delete m_field[24][13];
        setField(facThrone2->createField(), 24, 13);

    if (m_field[25][13] != m_emptyField )
        delete m_field[25][13];
        setField(facThrone4->createField(), 25, 13);


    checkTheEmptySpace(0);
    checkTheEmptySpace(12);
    checkTheEmptySpace(24);
    checkTheEmptySpace(633);
    checkTheEmptySpace(639);

    delete facWater;
    delete facBrickWall;
    delete facStellWall;
    delete facForest;
    delete facIceRoad;
    delete facThrone1;
    delete facThrone2;
    delete facThrone3;
    delete facThrone4;
}

void BattleCityField::fillField()
{
    Factory_Water      * facWater = new Factory_Water();
    Factory_BrickWall  * facBrickWall = new Factory_BrickWall();
    Factory_SteelWall  * facStellWall = new Factory_SteelWall();
    Factory_Forest     * facForest = new Factory_Forest();
    Factory_IceRoad    * facIceRoad = new Factory_IceRoad();
    Factory_Throne1    * facThrone1 = new Factory_Throne1();
    Factory_Throne2    * facThrone2 = new Factory_Throne2();
    Factory_Throne3    * facThrone3 = new Factory_Throne3();
    Factory_Throne4    * facThrone4 = new Factory_Throne4();

    setField(facThrone1->createField(), 24, 12);
    setField(facThrone3->createField(), 25, 12);
    setField(facThrone2->createField(), 24, 13);
    setField(facThrone4->createField(), 25, 13);

    setField(facStellWall->createField(), 14, 0);
    setField(facStellWall->createField(), 14, 1);
    setField(facStellWall->createField(), 6, 12);
    setField(facStellWall->createField(), 6, 13);
    setField(facStellWall->createField(), 7, 12);
    setField(facStellWall->createField(), 7, 13);
    setField(facStellWall->createField(), 14, 24);
    setField(facStellWall->createField(), 14, 25);

    setField(facIceRoad->createField(), 9, 0);
    setField(facIceRoad->createField(), 10, 0);
    setField(facIceRoad->createField(), 10, 1);
    setField(facIceRoad->createField(), 9, 1);
    setField(facIceRoad->createField(), 19, 20);
    setField(facIceRoad->createField(), 20, 20);
    setField(facIceRoad->createField(), 21, 20);
    setField(facIceRoad->createField(), 19, 21);
    setField(facIceRoad->createField(), 20, 21);
    setField(facIceRoad->createField(), 21, 21);

   for (int  i= 24; i<26; i++){
       for (int j= 17; j<26; j++){
            setField(facForest->createField(), j, i);
       }
   }

   for (int  i= 0; i<2; i++){
       for (int j= 17; j<26; j++){
            setField(facForest->createField(), j, i);
       }
   }

    setField(facWater->createField(), 18, 12);
    setField(facWater->createField(), 18, 13);
    setField(facWater->createField(), 19, 12);
    setField(facWater->createField(), 19, 13);

   for (int  i= 2; i<4; i++){
       for (int j= 2; j<11; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 6; i<8; i++){
       for (int j= 2; j<11; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 18; i<20; i++){
       for (int j= 2; j<11; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 22; i<24; i++){
       for (int j= 2; j<11; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 10; i<12; i++){
       for (int j= 2; j<9; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 14; i<16; i++){
       for (int j= 2; j<9; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 4; i<8; i++){
       for (int j= 13; j<15; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 18; i<22; i++){
       for (int j= 13; j<15; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }


       for (int j= 23; j<26; j++){
            setField(facBrickWall->createField(), j, 11);
       }

       for (int j= 23; j<26; j++){
            setField(facBrickWall->createField(), j, 14);
       }

         setField(facBrickWall->createField(), 23, 12);
         setField(facBrickWall->createField(), 23, 13);


   for (int  i= 10; i<12; i++){
       for (int j= 11; j<13; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 14; i<16; i++){
       for (int j= 11; j<13; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 12; i<14; i++){
       for (int j= 16; j<18; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 2; i<4; i++){
       for (int j= 17; j<24; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }
   for (int  i= 6; i<8; i++){
       for (int j= 17; j<21; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 10; i<12; i++){
       for (int j= 15; j<21; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 14; i<16; i++){
       for (int j= 15; j<21; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 18; i<20; i++){
       for (int j= 17; j<24; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }

   for (int  i= 22; i<24; i++){
       for (int j= 17; j<24; j++){
            setField(facBrickWall->createField(), j, i);
       }
   }
   delete facWater;
   delete facBrickWall;
   delete facStellWall;
   delete facForest;
   delete facIceRoad;
   delete facThrone1;
   delete facThrone2;
   delete facThrone3;
   delete facThrone4;
}

void BattleCityField::setLifeCount(int lifeCount)
{
    m_countLife = lifeCount;
    emit lifeCountChanged(m_countLife);
}

void BattleCityField::setLifeCount2(int lifeCount2)
{
    m_countLife2 = lifeCount2;
    emit lifeCount2Changed(m_countLife2);
}

void BattleCityField::countHittingEachPlayer(bool secondPlayer,short tankId)
{
    if (secondPlayer){
        switch (tankId) {
        case 0:
            ++m_tank_2stPlayer;
            break;
        case 1:
            ++m_fastTank_2stPlayer;
            break;
        case 2:
            ++m_armorTank_2stPlayer;
            break;
        case 3:
            ++m_heavyTank_2stPlayer;
            break;
        default:
            break;
        }
    } else {
        switch (tankId) {
        case 0:
            ++m_tank_1stPlayer;
            break;
        case 1:
            ++m_fastTank_1stPlayer;
            break;
        case 2:
            ++m_armorTank_1stPlayer;
            break;
        case 3:
            ++m_heavyTank_1stPlayer;
            break;
        default:
            break;
        }
    }
}

void BattleCityField::checkTheEmptySpace (short index)
{
    short yyy = index/26;
    short xxx = index % 26;

    if (m_field[yyy][xxx] != m_emptyField){
        delete m_field[yyy][xxx];
        m_field[yyy][xxx] = m_emptyField;
    }
    if (m_field[yyy+1][xxx] != m_emptyField){
        delete m_field[yyy+1][xxx];
        m_field[yyy+1][xxx] = m_emptyField;
    }
    if (m_field[yyy+1][xxx+1] != m_emptyField){
        delete m_field[yyy+1][xxx+1];
        m_field[yyy+1][xxx+1] = m_emptyField;
    }
    if (m_field[yyy][xxx+1] != m_emptyField){
        delete m_field[yyy][xxx+1];
        m_field[yyy][xxx+1] = m_emptyField;
    }
}

//void BattleCityField::setQuickFire(bool quickFire, bool secondPlayer)
//{
//    if (secondPlayer)
//        isQuickFire1Player = quickFire;
//    else
//        isQuickFire2Player = quickFire;
//}

void BattleCityField::firstLoadTanksSignals()
{
    if (m_object[0][0] != nullptr)
        emit createObject(m_object[0][0]->getQmlObject());
    if (m_object[0][12] != nullptr)
        emit createObject(m_object[0][12]->getQmlObject());
    if (m_object[0][24] != nullptr)
        emit createObject(m_object[0][24]->getQmlObject());
    if (m_object[24][9] != nullptr)
        emit createObject(m_object[24][9]->getQmlObject());
    if (m_object[24][15] != nullptr)
        emit createObject(m_object[24][15]->getQmlObject());
}
void BattleCityField::takingBonus(short row, short column, Object *tank)
{
    if (m_bonus == nullptr)
        return;
    if( m_bonus->getRowBonus() > (row-2) && m_bonus->getRowBonus() < row+2 &&
            m_bonus->getColumnBonus() > column-2 && m_bonus->getColumnBonus() < column +2){
        switch (m_bonus->getIdBonus()) {
        case 0:
            tank->setTempArmor();
            tank->getQmlObject()->setProperty("helm", true);
            if(tank == m_tankOfPlayer){
                m_countHelmBonusTank1 = m_bonus->getTimeBonusHelm();
            } else {
                m_countHelmBonusTank2 = m_bonus->getTimeBonusHelm();
            }
            break;
        case 1:
            m_countClockBonus = 50;
            break;
        case 2 :
            m_countShovelBonus = 100;
            createArmorBase();
            break;
        case 3:
            tank->plusStar();
            break;
        case 4:
            deleteAllTank_GrenadeBonus();
            break;
        case 5:
            if(tank == m_tankOfPlayer){
                ++m_countLife;
                emit lifeCountChanged(m_countLife);
            } else {
                ++m_countLife2;
                emit lifeCount2Changed(m_countLife2);
            }
            break;
        case 6:
            tank->plusStar();
            tank->plusStar();
            tank->plusStar();
            break;
        }
        delete m_bonus;
        m_bonus = nullptr;

    }
}

void BattleCityField::createArmorBase()
{
    Factory_SteelWall  * facStellWall = new Factory_SteelWall();
    for (int i = 23; i <26 ; ++i){
        if (m_field[i][11] != m_emptyField){
            delete m_field[i][11];
            m_field[i][11] = nullptr;
        }
        if(m_field[i][14] != m_emptyField){
            delete m_field[i][14];
            m_field[i][14] = nullptr;
        }
    }
    if (m_field[23][12] != m_emptyField){
        delete m_field[23][12];
        m_field[23][12] = nullptr;
    }
    if (m_field[23][13] != m_emptyField){
        delete m_field[23][13];
        m_field[23][13] = nullptr;
    }

    if (m_field[25][11] == nullptr){
        m_field[25][11] = facStellWall->createField();
    } else if (m_object[24][10] == nullptr){
        m_field[25][11] = facStellWall->createField();
    }
    if (m_field[24][11] == nullptr){
        m_field[24][11] = facStellWall->createField();
    } else if (m_object[24][10] == nullptr && m_object[23][10] == nullptr){
        m_field[24][11] = facStellWall->createField();
    }
    if (m_field[23][11] == nullptr){
        m_field[23][11] = facStellWall->createField();
    } else if (m_object[23][10] == nullptr && m_object[22][10] == nullptr && m_object[22][11] == nullptr){
        m_field[23][11] = facStellWall->createField();
    }
    if (m_field[23][12] == nullptr){
        m_field[23][12] = facStellWall->createField();
    } else if (m_object[22][11] == nullptr && m_object[22][12] == nullptr){
        m_field[23][12] = facStellWall->createField();
    }
    if (m_field[23][13] == nullptr){
        m_field[23][13] = facStellWall->createField();
    } else if (m_object[22][12] == nullptr && m_object[22][13] == nullptr){
        m_field[23][13] = facStellWall->createField();
    }
    if (m_field[23][14] == nullptr){
        m_field[23][14] = facStellWall->createField();
    } else if (m_object[22][13] == nullptr && m_object[22][14] == nullptr && m_object[23][14] == nullptr){
        m_field[23][14] = facStellWall->createField();
    }
    if (m_field[24][14] == nullptr){
        m_field[24][14] = facStellWall->createField();
    } else if (m_object[24][14] == nullptr && m_object[23][14] == nullptr){
        m_field[24][14] = facStellWall->createField();
    }
    if (m_field[25][14] == nullptr){
        m_field[25][14] = facStellWall->createField();
    } else if (m_object[24][14] == nullptr){
        m_field[25][14] = facStellWall->createField();
    }

    for (int i = 23; i <26 ; ++i){
        if (m_field[i][11] != m_emptyField){
            emit deleteFieldSignal(i,11);
        }
        if(m_field[i][14] != m_emptyField){
            emit deleteFieldSignal(i,14);
        }
    }
    if (m_field[23][12] != m_emptyField){
        emit deleteFieldSignal(23,12);
    }
    if (m_field[23][13] != m_emptyField){
        emit deleteFieldSignal(23,13);
    }
    delete facStellWall;
}

void BattleCityField::resetArmorBase()
{
    Factory_BrickWall * facBrickWall  = new Factory_BrickWall();
    for (int i = 23; i <26 ; ++i){
        if (m_field[i][11] != m_emptyField){
            delete m_field[i][11];
            m_field[i][11] = facBrickWall->createField();
            emit deleteFieldSignal(i,11);
        }
        if(m_field[i][14] != m_emptyField){
            delete m_field[i][14];
            m_field[i][14] = facBrickWall->createField();
            emit deleteFieldSignal(i,14);
        }
    }
    if (m_field[23][12] != m_emptyField){
        delete m_field[23][12];
        m_field[23][12] = facBrickWall->createField();
        emit deleteFieldSignal(23,12);
    }
    if (m_field[23][13] != m_emptyField){
        delete m_field[23][13];
        m_field[23][13] = facBrickWall->createField();
        emit deleteFieldSignal(23,13);
    }
    delete facBrickWall;
}

void BattleCityField::deleteAllTank_GrenadeBonus()
{
    short countDeleteTank = 0;
    for (short y = 0; y < 25; ++y){
        for (short x = 0; x < 25; ++x){
            if (m_object[y][x] != nullptr){
                if (m_object[y][x] != m_tankOfPlayer && m_object[y][x] != m_tankOfPlayer2){
                    ++countDeleteTank;
                    deleteTank(y,x,false);
                    createFlash(y+1,x+1);
                }
            }
        }
    }
    if (countDeleteTank > 3){
        m_countDelayCreationNextTank = DELAY_CREATION_NEXT_TANK;
        countDeleteTank = 3;
    }


    for (int i = 0; i < countDeleteTank; ++i){
        createTank();
    }
}


