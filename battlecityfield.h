#ifndef BATTLECITYFIELD_H
#define BATTLECITYFIELD_H

#include "object.h"
#include "field.h"
#include <QTimer>
#include "factory_of_object.h"
#include <QDebug>
#include "bonus.h"

class BattleCityField : public QTimer
{
    Q_OBJECT
    Q_PROPERTY(bool second_Player READ second_Player WRITE setSecond_Player NOTIFY second_PlayerChanged)
    Q_PROPERTY (int lifeCount READ lifeCount WRITE setLifeCount NOTIFY lifeCountChanged)
    Q_PROPERTY (int lifeCount2 READ lifeCount2 WRITE setLifeCount2 NOTIFY lifeCount2Changed)
    Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverChanged)
    Q_PROPERTY(int stage READ stage  NOTIFY stageChanged)

public:
    /*Q_INVOKABLE*/ void resetModel();

    BattleCityField(QQmlApplicationEngine *engine, QObject *parent = nullptr);
    void fillFieldFromTheFile(std::vector<char> &refFile);
    bool second_Player(){return m_secondPlayer;}
    void setSecond_Player(bool secondPlayer);
    bool gameOver(){return m_gameOver;}
    int stage(){return m_stage;}
    int lifeCount() const {return m_countLife;}
    int lifeCount2() const {return m_countLife2;}
    void setLifeCount(int lifeCount);
    void setLifeCount2(int lifeCount2);
    void fillField();
    void startGame();
    void setField(Field*, int, int);
    void setObject(Object *, int, int);
    void setBulletAndFlesh(Object *, int, int);
    const Object * getObject(int,int);
    const Field * getField(int,int);
    const Field * getEmptyField() const {return m_emptyField;}
    const Object * getBulletAndFlash(int, int, int);
    void moveTank(bool bonusClock);
    short getLastDirectMove() const {return lastDirect;}
    short getLastDirect2Move() const {return lastDirect2;}
    int getNumberStep();
    void setNumberStep();
    int getTankCount(){return tankCount;}
    int getNumberStepBullet(){return numberStepBullet;}
    void setNumberStepBullet(){++numberStepBullet;}
    void firstFillField();
    void shot();
    void moveBullet();
    void setDirectTankOfPlayer (int direct)    {m_tankOfPlayer->setDirectMove(direct);  lastDirect = direct;}
    void setDirectTankOfPlayer2(int direct)    {m_tankOfPlayer2->setDirectMove(direct); lastDirect2 = direct;}
    void shotPlayer(short, short, int, bool);
    bool getIsShotPlayer()const {return isShotPlayer;}
    bool getIsShotPlayer2()const {return isShotPlayer2;}
    void doShotPlayer(){isShotPlayer = true;}
    void doShotPlayer2(){isShotPlayer2 = true;}
    void resetShotPlayer(){isShotPlayer = false;}
    void resetShotPlayer2(){isShotPlayer2 = false;}
    void setCountLife(int setCount);
    void setPointerFields (std::vector<char> * pointerField){m_pointerField = pointerField;}
    void firstLoadTanksSignals();

    ~BattleCityField();

signals:
    void second_PlayerChanged(bool second_Player);
    void gameOverChanged(bool game_Over);
    void stageChanged(int _stage);
    void lifeCountChanged(int countLife);
    void lifeCount2Changed(int countLife2);
    void moveTankSignal(short beginX, short beginY,short endX,short endY);
    void newFlashSignal(short beginX, short beginY);
    void newBulletSignal_0(short beginX, short beginY);
    void newBulletSignal_1(short beginX, short beginY);
    void newBulletSignal_2(short beginX, short beginY);
    void newBulletSignal_3(short beginX, short beginY);
    void moveBulletSignal_0(short beginX, short beginY,short endX,short endY);
    void moveBulletSignal_1(short beginX, short beginY,short endX,short endY);
    void moveBulletSignal_2(short beginX, short beginY,short endX,short endY);
    void moveBulletSignal_3(short beginX, short beginY,short endX,short endY);
    void deleteFieldSignal(short, short);
    void createTankSignal(short y,short x);
    void createTankCountSignal();
    void deleteTankSignal(short y,short x);
    void createTankOfPlayerSignal(short y,short x, bool secondPlayer);
    void moveTankOfPlayerSignal(short beginX, short beginY,short endX,short endY, bool secondPlayer);
    void endGame(short lightTank_1p,short speedTank_1p,short heavyTank_1p,short durtyheavyTank_1p,short lightTank_2p,
                 short speedTank_2p,short heavyTank_2p,short durtyheavyTank_2p);
    void createObject(QObject* newObject);
    void createBulletFlashSignal(QObject* newObject);

private:
    void createBullet(const short, const short, int const, bool , bool, bool fastBullet, bool superBullet);
    void createFlash(const short, const short);
    void createTank();
    void deleteBullet(const short, const short, int const direct);
    void deleteField(const short, const short, bool isSuperBullet);
    void deleteTank(const short, const short, bool createNewTank);
    void resetTankOfPlayer(short y, short x);
    void resetTankOfPlayer2(short y, short x);
    void testHittingTank();
    void stopGame();
    void setXTankPlayer(const short x){m_x = x;}
    void setYTankPlayer (const short y){m_y = y;}
    short getXTankPlayer() const {return m_x;}
    short getYTankPlayer() const {return m_y;}
    void setXTankPlayer2(const short x){m_x2 = x;}
    void setYTankPlayer2 (const short y){m_y2 = y;}
    short getXTankPlayer2() const {return m_x2;}
    short getYTankPlayer2() const {return m_y2;}
    void countHittingEachPlayer(bool,short);
    void checkTheEmptySpace (short index);
    void deleteAllTank_GrenadeBonus();
    void setQuickFire(bool quickFire, bool secondPlayer);
    void takingBonus(short row, short column, Object* tank);
    void createArmorBase();
    void resetArmorBase();

    bool isShotPlayer = false;
    bool isShotPlayer2 = false;
    int  numberStep = 0;
    int  numberStepBullet = 0;
    const int constTankCount = 17;
    int  tankCount = constTankCount;
    Field * m_emptyField = nullptr;
    short m_x;
    short m_y;
    short m_x2;
    short m_y2;
    int lastDirect = 1;
    int lastDirect2 = 1;
    int CONST_TIME_FOR_END_GAME = 10;
    int countTimeForEndGame = CONST_TIME_FOR_END_GAME;
    const int CONST_LIFE = 1;
    int m_countLife = CONST_LIFE;
    int m_countLife2 = CONST_LIFE;
    const short DELAY_CREATION_NEXT_TANK = 20;
    short m_countDelayCreationNextTank = DELAY_CREATION_NEXT_TANK;
    bool m_gameOver = false;
    bool m_secondPlayer = false;
    int m_stage = 1;
    bool m_notEmptySpaceForTank = false;
    short m_tank_1stPlayer = 0;
    short m_tank_2stPlayer = 0;
    short m_fastTank_1stPlayer = 0;
    short m_fastTank_2stPlayer = 0;
    short m_armorTank_1stPlayer = 0;
    short m_armorTank_2stPlayer = 0;
    short m_heavyTank_1stPlayer = 0;
    short m_heavyTank_2stPlayer = 0;

    short m_countHelmBonusTank1 = -1;
    short m_countHelmBonusTank2 = -1;
    short m_countClockBonus = -1;
    short m_countShovelBonus = -1;

    std::vector<char> * m_pointerField;
    Object* m_object[26][26];
    Field*  m_field[26][26];
    Object* m_bullet0[27][27];
    Object* m_bullet1[27][27];
    Object* m_bullet2[27][27];
    Object* m_bullet3[27][27];
    Object* m_flashArray[27][27];
    Factory_of_Object * m_Factory_of_Object[10];
    QTimer *  tankMoveTimer;
    Object * m_tankOfPlayer;
    Object * m_tankOfPlayer2;
    QQmlApplicationEngine *m_engine;
    Bonus* m_bonus;

public slots:
    void goMoveTimer();
    void resetModelSlot(){resetModel();}
};

#endif // BATTLECITYFIELD_H
