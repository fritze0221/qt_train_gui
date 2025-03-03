#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    apihandler = new ApiHandler();

    train_1 = new TrainControl(this, TRAIN_1);
    train_2 = new TrainControl(this, TRAIN_2);
    train_1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    train_2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    mainboard = new MainboardControl(this);

    connection_control = new ConnectionControl(this);
    connection_control->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    stop_button = new StopButton(this);
    stop_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    couple_button = new CoupleButton(this);
    couple_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    connect(train_1, &TrainControl::sendTrainState, apihandler,&ApiHandler::onTrain1State);
    connect(train_2, &TrainControl::sendTrainState, apihandler,&ApiHandler::onTrain2State);
    connect(apihandler, &ApiHandler::sendTrain1CurrSpeed, train_1, &TrainControl::onCurrSpeed);
    connect(apihandler, &ApiHandler::sendTrain2CurrSpeed, train_2, &TrainControl::onCurrSpeed);
    connect(train_1, &TrainControl::sendSliderValue, train_2, &TrainControl::onSliderValue);
    connect(train_2, &TrainControl::sendSliderValue, train_1, &TrainControl::onSliderValue);
    connect(train_1, &TrainControl::sendSetButtonClicked, train_2, &TrainControl::onExternalSetButtonClicked);
    connect(train_2, &TrainControl::sendSetButtonClicked, train_1, &TrainControl::onExternalSetButtonClicked);
    connect(train_1, &TrainControl::sendDirectionButtonClicked, train_2, &TrainControl::onExternalDirectionButtonClicked);
    connect(train_2, &TrainControl::sendDirectionButtonClicked, train_1, &TrainControl::onExternalDirectionButtonClicked);


    connect(mainboard, &MainboardControl::sendHouse1State, apihandler,&ApiHandler::onHouse1State);
    connect(mainboard, &MainboardControl::sendHouse2State, apihandler,&ApiHandler::onHouse2State);
    connect(mainboard, &MainboardControl::sendSwitch1State, apihandler,&ApiHandler::onSwitch1State);
    connect(mainboard, &MainboardControl::sendSwitch2State, apihandler,&ApiHandler::onSwitch2State);

    connect(apihandler, &ApiHandler::sendConnectionLost, connection_control,&ConnectionControl::onConnectionLost);
    connect(connection_control, &ConnectionControl::sendConnectButtonClicked,apihandler, &ApiHandler::onConnectButton);
    connect(apihandler, &ApiHandler::sendConnectionEstablished, connection_control, &ConnectionControl::onConnectionEstablished);

    connect(stop_button, &StopButton::sendStopButtonClicked, apihandler, &ApiHandler::onStopButton);

    connect(couple_button, &CoupleButton::sendCoupleButtonClicked, train_1, &TrainControl::onCoupleButtonClicked);
    connect(couple_button, &CoupleButton::sendCoupleButtonClicked, train_2, &TrainControl::onCoupleButtonClicked);

    QWidget* centralWidget = new QWidget(this);

    QHBoxLayout* train_layout = new QHBoxLayout();
    train_layout->addWidget(train_1);
    train_layout->addWidget(train_2);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(connection_control);
    layout->addWidget(mainboard);
    layout->addWidget(couple_button);
    layout->addLayout(train_layout);
    layout->addWidget(stop_button);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

}

MainWindow::~MainWindow() {}
