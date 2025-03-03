#include "mainboard_handle.h"

MainboardHandle::MainboardHandle()
{

    qDebug() << "[INFO] Starting Mainboard";
    connectToServer("local");
    
    connect(this, &MainboardHandle::dataReceived, this, &MainboardHandle::onDataReceived);
    connect(this, &MainboardHandle::sendUpdateAPI, this, &MainboardHandle::onUpdateAPI);
    connect(this, &MainboardHandle::errorOccurred, this, &MainboardHandle::sendQuit);

    QByteArray client_id = QByteArray::number(MAINBOARD);
    sendData(client_id);

    sleep(1);

    mainboard = connection_init(MB, MB_PORT);
}

void MainboardHandle::onDataReceived(const QByteArray &data){

    int count = data.size() / sizeof(int);
    recVec.resize(count);

    std::memcpy(recVec.data(), data.constData(), data.size());

    qDebug() << "[INFO]" << recVec;

    emit sendUpdateAPI();  
  
}

void MainboardHandle::onUpdateAPI(){

    qDebug() << "[INFO] Update API..,";

    for(int i = 0;i < 3;i++) mainboard_setLed(mainboard, i,recVec[i]*4095);

    for(int i = 3;i < 11;i++) mainboard_setLed(mainboard, i, recVec[i]*4095);

    mainboard_writeLed(mainboard);

    mainboard_setWeiche(mainboard,1,recVec[11]);
    mainboard_setWeiche(mainboard,0,recVec[12]);
    
}