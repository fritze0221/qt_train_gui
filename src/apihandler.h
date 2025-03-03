#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QObject>
#include <QVector>
#include <QProcess>
#include <QDebug>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>

#include "winterface-config.h"
#include "config.h"

class ApiHandler : public QObject
{
    Q_OBJECT
public:
    explicit ApiHandler(QObject *parent = nullptr);

private:

    QLocalServer* server;

    QLocalSocket* train_1_socket;
    QMetaObject::Connection train_1_socket_receive_qconnection;
    QMetaObject::Connection train_1_socket_error_qconnection;

    QLocalSocket* train_2_socket;
    QMetaObject::Connection train_2_socket_receive_qconnection;
    QMetaObject::Connection train_2_socket_error_qconnection;

    QLocalSocket* mainboard_socket;
    QMetaObject::Connection mainboard_socket_error_qconnection;

    QVector<QLocalSocket*> tmp_client;
    QVector<QMetaObject::Connection> tmp_client_qconnection ;

    QProcess* train_1_process;
    QTimer* train_1_watchdog;
    QMetaObject::Connection train_1_watchdog_qconnection;

    QProcess* train_2_process;
    QTimer* train_2_watchdog;
    QMetaObject::Connection train_2_watchdog_qconnection;

    QProcess* mainboard_process;

    QString train_1_process_path = "/home/urs/uni/cpp-gui/qt_gui/build/Api-Build/train1";
    QString train_2_process_path = "/home/urs/uni/cpp-gui/qt_gui/build/Api-Build/train2";
    QString mainboard_process_path = "/home/urs/uni/cpp-gui/qt_gui/build/Api-Build/mainboard";

    int switch_1_state = 0;
    int switch_2_state = 0;

    QVector<int> house_1_state = {0};
    int house_1_state_size = 3;

    QVector<int> house_2_state = {0};
    int house_2_state_size = 8;

    int set_train_1_speed = 0;
    int set_train_1_time = 0;
    int set_train_1_direction = FORWARD;

    int set_train_2_speed = 0;
    int set_train_2_time = 0;
    int set_train_2_direction = FORWARD;

signals:

    void sendTrain1CurrSpeed(int curr_speed);
    void sendTrain2CurrSpeed(int curr_speed);

    void sendTrain1StateChange();
    void sendTrain2StateChange();
    void sendMainboardStateChange();

    void sendConnectionEstablished();
    void sendConnectionLost();

    void sendConnectionID(int index);

public slots:

    void onTrain1State(int set_direction, int set_speed, int set_time);
    void onTrain2State(int set_direction, int set_speed, int set_time);

    void onHouse1State(QVector<int> state);
    void onHouse2State(QVector<int> state);

    void onSwitch1State(int state);
    void onSwitch2State(int state);

    void onConnectButton();

    void onStopButton();

private slots:

    void onTrain1StateChange();
    void onTrain2StateChange();
    void onMainboardStateChange();

    void onServerConnection();
    void onConnectionID(int clinet_index);

    void onTrain1DataReceived();
    void onTrain2DataReceived();

    void onSocketError(QLocalSocket* socket);

    void onTrain1Watchdog();
    void onTrain2Watchdog();


};

#endif // APIHANDLER_H
