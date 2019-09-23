#ifndef OBJECT_H
#define OBJECT_H
#include <QDebug>
#include <QQmlComponent>
#include <QQmlApplicationEngine>

#include <QString>



class Object
{
public:
    Object(QQmlApplicationEngine* enginePointer = nullptr, short row = 0,short column = 0);
    virtual  QString  urlView() const = 0;
    short getDirectMove() const {return directionMove;}
    void setSpeed(short speed);
    void setDirectMove(int rand){directionMove = rand;}
    void setStepOfObject(int step){this->step = step;}
    void nextStepOfObject(){step++;}
    int getStepOfObject(){return step;}
    bool getEnemy(){return m_enemy;}
    void setEnemy(bool enemy){m_enemy = enemy;}
    void hitting ();
    short getArmor(){return m_armor;}
    void setSecondPlayer(bool secondPlayer){m_secondPlayer = secondPlayer;}
    bool getSecondPlayer(){return m_secondPlayer;}
    short getIdTank(){return idTank;}
    short getSpeed(){return m_speed;}
    void setFastBullet(bool fastBullet){m_fastSpeedBullet = fastBullet;}
    bool getFastBullet(){return m_fastSpeedBullet;}
//    void deleteQQmlComponent(){delete m_QQmlComponent;}
    QObject* createQQmlComponent(QQmlApplicationEngine* enginePointer, short row,short column);
    void rotationQQmlComponent(int rand);
    void moveQQmlComponent(short row,short column);
    QObject* getQmlObject(){return m_QQmlComponent;}
    void setBonus();
    bool getBonus() const;
    void resetBonus();
    void setTempArmor(){m_tempArmor = m_armor; m_armor = HELM_ARMOR_TEMP;}
    void returnArmor() {m_armor = m_tempArmor;}
    void plusStar();
    bool isQuickFire(){return quickFire;}
    void setQuickFire(){quickFire = true;}
    void resetQuickFire(){quickFire = false;}
    bool isSuperBullet(){return m_steelDestroed;}
    void setSuperBullet(){m_steelDestroed = true;}
    void resetSuperBullet(){m_steelDestroed = false;}
    void resetStar(){m_count_star = 0;}
    void resetArmor(){m_armor = 1;}
    void resetTank();
    virtual ~Object();

protected:
    QString m_source;
    short directionMove = 0;
    int step=0;
    bool m_enemy;
    short m_armor = 1;
    short idTank = -1;
    short m_speed = - 1;
    bool m_fastSpeedBullet = false;
    QObject* m_QQmlComponent = nullptr;

private:
    bool m_secondPlayer = false;
    bool m_bonus = false;
    const short HELM_ARMOR_TEMP = 30;
    short m_tempArmor;
    short m_count_star = 0;
    bool quickFire = false;
    bool m_steelDestroed = false;


};

#endif // OBJECT_H
