#include "battletimer.h"
#include <QDebug>


BattleTimer::BattleTimer(QObject *parent)
    : QTimer(parent)
{
    batTimer = new QTimer(this);
    connect(batTimer, SIGNAL(timeout()), this, SLOT(goTimer()));
    batTimer->start(1000);

}

void BattleTimer::goTimer()
{

    qDebug() << "qwerty";
}
