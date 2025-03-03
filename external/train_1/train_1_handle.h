#ifndef TRAIN1HANDLE_H
#define TRAIN1HANDLE_H

#include <QTimer>

#include "../src/apihandler.h"
#include "../../src/config.h"
#include "../../src/config.h"
#include "../api/src_winterface/winterface-api.h"

class TrainHandle : public ApiHandler
{
    Q_OBJECT

public:

    TrainHandle();

private:

    WINTP train_1;
    QTimer* curr_speed_timer;

    //0;direction 1;speed 2;time
    QVector<int> recVec;

signals: 

    void sendUpdateAPI();
    void sendQuit();

private slots:

    void onDataReceived(const QByteArray &data);
    void onUpdateAPI();
    void onCurrSpeedTimer();
};

#endif 
