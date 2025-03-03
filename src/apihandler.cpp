#include "apihandler.h"

ApiHandler::ApiHandler(QObject *parent)
    : QObject{parent}
{

    house_1_state.resize(house_1_state_size);
    house_2_state.resize(house_2_state_size);

    server = new QLocalServer();

    train_1_socket = new QLocalSocket();
    train_2_socket = new QLocalSocket();
    mainboard_socket = new QLocalSocket();

    train_1_process = new QProcess();
    train_2_process = new QProcess();
    mainboard_process = new QProcess();

    train_1_watchdog = new QTimer();
    train_2_watchdog = new QTimer();

    connect(server, &QLocalServer::newConnection, this, &ApiHandler::onServerConnection);
    connect(this, &ApiHandler::sendTrain1StateChange, this, &ApiHandler::onTrain1StateChange);
    connect(this, &ApiHandler::sendTrain2StateChange, this, &ApiHandler::onTrain2StateChange);
    connect(this, &ApiHandler::sendMainboardStateChange, this, &ApiHandler::onMainboardStateChange);
    connect(this, &ApiHandler::sendConnectionID, this, &ApiHandler::onConnectionID);

    server->removeServer("local");
    if (!server->listen("local")) {
        qDebug() << "[SERVER] Server could not be started";
    }

}


void ApiHandler::onTrain1Watchdog(){

    train_1_process->terminate();
    emit sendConnectionLost();

}

void ApiHandler::onTrain2Watchdog(){

    train_2_process->terminate();
    emit sendConnectionLost();

}


void ApiHandler::onTrain1StateChange(){

    QVector<int> sendVec;

    sendVec.append(set_train_1_direction);
    sendVec.append(set_train_1_speed);
    sendVec.append(set_train_1_time);

    QByteArray data = QByteArray(reinterpret_cast<const char*>(sendVec.constData()), sendVec.size() * sizeof(int));

    if(train_1_socket->state() == QLocalSocket::ConnectedState){

        train_1_socket->write(data);
        qDebug() << "[SERVER] Send Train 1 Data";
    }
}

void ApiHandler::onTrain2StateChange(){

    QVector<int> sendVec;

    sendVec.append(set_train_2_direction);
    sendVec.append(set_train_2_speed);
    sendVec.append(set_train_2_time);

    QByteArray data = QByteArray(reinterpret_cast<const char*>(sendVec.constData()), sendVec.size() * sizeof(int));

    if(train_2_socket->state() == QLocalSocket::ConnectedState){

        train_2_socket->write(data);
        qDebug() << "[SERVER] Send Train 2 Data";

    }

}

void ApiHandler::onMainboardStateChange(){

    QVector<int> sendVec;

    sendVec.append(house_1_state);
    sendVec.append(house_2_state);
    sendVec.append(switch_1_state);
    sendVec.append(switch_2_state);

    QByteArray data = QByteArray(reinterpret_cast<const char*>(sendVec.constData()), sendVec.size() * sizeof(int));

    if(mainboard_socket->state() == QLocalSocket::ConnectedState){

        mainboard_socket->write(data);
        qDebug() << "[SERVER] Send Mainboard Data";
    }

}

void ApiHandler::onConnectButton(){

    qDebug() << "[PROCESS]: Starting API-Process... ";

    if(!(train_1_process->state() == QProcess::Running)) train_1_process->start(train_1_process_path);

    if(!(train_2_process->state() == QProcess::Running)) train_2_process->start(train_2_process_path);

    if(!(mainboard_process->state() == QProcess::Running)) mainboard_process->start(mainboard_process_path);

    emit sendConnectionEstablished();
}

void ApiHandler::onStopButton(){

    qDebug() << "[INFO] Train 1 and Train 2 stopped";

    set_train_1_speed = 0;
    set_train_1_time = 0;

    set_train_2_speed = 0;
    set_train_2_time = 0;

    emit sendTrain1StateChange();
    emit sendTrain2StateChange();

}

void ApiHandler::onTrain1State(int set_direction, int set_speed, int set_time){

    qDebug() << "[INFO] Train 1 State Update: " << set_direction << set_speed << set_time;

    set_train_1_direction = set_direction;
    set_train_1_speed = set_speed;
    set_train_1_time = set_time;

    emit sendTrain1StateChange();

}

void ApiHandler::onTrain2State(int set_direction, int set_speed, int set_time){

    qDebug() << "[INFO] Train 2 State Update"  << set_direction << set_speed << set_time;;

    set_train_2_direction = set_direction;
    set_train_2_speed = set_speed;
    set_train_2_time = set_time;

    emit sendTrain2StateChange();

}

void ApiHandler::onHouse1State(QVector<int> state){

    qDebug() << "[INFO] House 1 State Update" << state;

    for(int i = 0;i < house_1_state_size;i++){

        house_1_state[i] = state[i];

    }

    emit sendMainboardStateChange();

}

void ApiHandler::onHouse2State(QVector<int> state){

    qDebug() << "[INFO] House 2 State Update" << state;

    for(int i = 0;i < house_2_state_size;i++){

        house_2_state[i] = state[i];

    }

    emit sendMainboardStateChange();

}

void ApiHandler::onSwitch1State(int state){

    qDebug() << "[INFO] Switch 1 State Update" << state;

    switch_1_state = state;

    emit sendMainboardStateChange();

}

void ApiHandler::onSwitch2State(int state){

    qDebug() << "[INFO] Switch 2 State Update" << state;

    switch_2_state = state;

    emit sendMainboardStateChange();

}

void ApiHandler::onServerConnection(){

    qDebug() << "[SERVER] New Socket-Connection...";

    QLocalSocket* client = server->nextPendingConnection();
    tmp_client.append(client);

    int size = tmp_client.size()-1;
    QMetaObject::Connection conn = connect(client, &QLocalSocket::readyRead, this, [=](){emit sendConnectionID(size);});

    tmp_client_qconnection.append(conn);
}

void ApiHandler::onConnectionID(int client_index){

    QByteArray data = tmp_client[client_index]->readAll();

    if(data.toInt() == TRAIN_1){

        train_1_socket = tmp_client[client_index];

        disconnect(tmp_client_qconnection[client_index]);
        tmp_client[client_index] = nullptr;

        train_1_socket_receive_qconnection = connect(train_1_socket, &QLocalSocket::readyRead, this, &ApiHandler::onTrain1DataReceived);
        train_1_socket_error_qconnection = connect(train_1_socket, &QLocalSocket::errorOccurred, this, [=](){onSocketError(train_1_socket);});

        train_1_watchdog_qconnection = connect(train_1_watchdog, &QTimer::timeout, this, &ApiHandler::onTrain1Watchdog);
        train_1_watchdog->start(5000);

        qDebug() << "[SERVER] Train 1 Connected";

        emit sendTrain1StateChange();

    }

    if(data.toInt() == TRAIN_2){

        train_2_socket = tmp_client[client_index];

        disconnect(tmp_client_qconnection[client_index]);
        tmp_client[client_index] = nullptr;

        train_2_socket_receive_qconnection = connect(train_2_socket, &QLocalSocket::readyRead, this, &ApiHandler::onTrain2DataReceived);
        train_2_socket_error_qconnection = connect(train_2_socket, &QLocalSocket::errorOccurred, this, [=](){onSocketError(train_2_socket);});

        train_2_watchdog_qconnection = connect(train_2_watchdog, &QTimer::timeout, this, &ApiHandler::onTrain2Watchdog);
        train_2_watchdog->start(5000);

        qDebug() << "[SERVER] Train 2 Connected";

        emit sendTrain2StateChange();

    }

    if(data.toInt() == MAINBOARD){

        mainboard_socket = tmp_client[client_index];

        disconnect(tmp_client_qconnection[client_index]);
        tmp_client[client_index] = nullptr;

        mainboard_socket_error_qconnection = connect(mainboard_socket, &QLocalSocket::errorOccurred, this, [=](){onSocketError(mainboard_socket);});

        qDebug() << "[SERVER] Mainboard Connected";

        emit sendMainboardStateChange();

    }

}

void ApiHandler::onTrain1DataReceived(){

    QByteArray data = train_1_socket->readAll();

    qDebug() << "[SERVER] Received current Speed from Train 1: " << data.toInt();

    train_1_watchdog->start(5000);

    emit sendTrain1CurrSpeed(data.toInt());

}

void ApiHandler::onTrain2DataReceived(){

    QByteArray data = train_2_socket->readAll();

    qDebug() << "[SERVER] Received current Speed from Train 2: " << data.toInt();

    train_2_watchdog->start(5000);

    emit sendTrain2CurrSpeed(data.toInt());

}

void ApiHandler::onSocketError(QLocalSocket* socket){

    if(socket == train_1_socket){

        qDebug() << "[SERVER] Train 1 Socket disconnected";

        disconnect(train_1_socket_error_qconnection);
        disconnect(train_1_socket_receive_qconnection);
        disconnect(train_1_watchdog_qconnection);

        train_1_watchdog->stop();

        train_1_socket->flush();
        train_1_socket->disconnect();

        if(train_1_process->state() == QProcess::Running) train_1_process->terminate();

    }

    if(socket == train_2_socket){

        qDebug() << "[SERVER] Train 2 Socket disconnected";

        disconnect(train_2_socket_error_qconnection);
        disconnect(train_2_socket_receive_qconnection);
        disconnect(train_2_watchdog_qconnection);

        train_2_watchdog->stop();

        train_2_socket->flush();
        train_2_socket->disconnect();

        if(train_2_process->state() == QProcess::Running) train_2_process->terminate();

    }

    if(socket == mainboard_socket){

        qDebug() << "[SERVER] Mainboard Socket disconnected";

        disconnect(mainboard_socket_error_qconnection);

        if(mainboard_process->state() == QProcess::Running) mainboard_process->terminate();

    }

    socket->close();

    emit sendConnectionLost();

}
