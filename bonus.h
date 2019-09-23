#ifndef BONUS_H
#define BONUS_H

#include <QQmlComponent>
#include <QQmlApplicationEngine>


class Bonus
{
public:
    Bonus(QQmlApplicationEngine* engine);
    short getIdBonus() const {return m_Id;}
    short getColumnBonus() const {return m_Column;}
    short getRowBonus() const {return m_Row;}
    QObject* getQmlBonus() const {return instanceBonus;}
    short getTimeBonusHelm(){return HELM_CLOCK_TIME_BONUS;}
    ~Bonus();
private:
    short m_Row;
    short m_Column;
    short m_Id = -1;
    QObject* instanceBonus;
    const short HELM_CLOCK_TIME_BONUS = 100; // вернуть 50!!!!!!
    const short SHOVEL_TIME_BONUS = 100;
    const short RESPAUN_TIME_BONUS = 7;


};

#endif // BONUS_H
