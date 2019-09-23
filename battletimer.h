#ifndef BATTLETIMER_H
#define BATTLETIMER_H
#include <QTimer>


class BattleTimer : public QTimer
{
    Q_OBJECT

public:
    BattleTimer(QObject *parent = nullptr);

private:
    QTimer *  batTimer;

public slots:
    void goTimer();
};

#endif // BATTLETIMER_H
