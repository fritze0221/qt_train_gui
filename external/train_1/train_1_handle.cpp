#include "train_1_handle.h"

TrainHandle::TrainHandle()
{

    qDebug() << "[INFO] Starting Train 1";

    curr_speed_timer = new QTimer();

    connectToServer("local");

    connect(this, &TrainHandle::dataReceived, this, &TrainHandle::onDataReceived);
    connect(this, &TrainHandle::sendUpdateAPI, this, &TrainHandle::onUpdateAPI);
    connect(this, &TrainHandle::errorOccurred, this, &TrainHandle::sendQuit);
    connect(curr_speed_timer, &QTimer::timeout, this, &TrainHandle::onCurrSpeedTimer);

    QByteArray client_id = QByteArray::number(TRAIN_1);
    sendData(client_id);
    sleep(1);
    train_1 = connection_init(TRAIN_Y,TRAIN_PORT_Y);

    curr_speed_timer->start(500);
}



void TrainHandle::onCurrSpeedTimer(){

    int curr_speed = train_reportSpeed(train_1);
    QByteArray data = QByteArray::number(curr_speed);
    sendData(data);

    if(curr_speed == 0) {

        ApiHandler::m_socket->disconnect();
        emit sendQuit();
    }

    curr_speed_timer->start(500);

}

void TrainHandle::onDataReceived(const QByteArray &data){

    int count = data.size() / sizeof(int);
    recVec.resize(count);

    std::memcpy(recVec.data(), data.constData(), data.size());

    qDebug() << "[INFO]" << recVec;

    emit sendUpdateAPI();  
  
}

void TrainHandle::onUpdateAPI(){

    qDebug() << "[INFO] Update API..,";

    train_setDirection(train_1, recVec[0]);
    
    if(recVec[2] != 0){

        train_setRamp(train_1, recVec[1], recVec[2]);

    } 

    else{

        train_setSpeed(train_1, recVec[1]);

    }


}