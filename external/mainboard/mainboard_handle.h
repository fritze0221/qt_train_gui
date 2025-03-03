#ifndef MAINBOARDHANDLE_H
#define MAINBOARDHANDLE_H

#include <QCoreApplication>

#include "../src/apihandler.h"
#include "../../src/config.h"

#include "../api/src_winterface/winterface-config.h"
#include "../api/src_winterface/winterface-api.h"

class MainboardHandle : public ApiHandler
{
    Q_OBJECT

public:

    MainboardHandle();

private: 

    WINTP mainboard;

    //0-2;HOUSE_1 3-10;HOUSE_2  11;Switch_1 12;Switch_2
    QVector<int> recVec;

signals: 

    void sendUpdateAPI();
    void sendQuit();

private slots:

    void onDataReceived(const QByteArray &data);
    void onUpdateAPI();

};

#endif // MAINBOARDHANDLE_H
