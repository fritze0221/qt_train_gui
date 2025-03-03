#include "connectioncontrol.h"

ConnectionControl::ConnectionControl(QWidget *parent)
    : QWidget{parent}
{

    button_connect = new QPushButton("Verbinden",this);

    connect(button_connect, &QPushButton::clicked, this, &ConnectionControl::sendConnectButtonClicked);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(button_connect);

    setLayout(layout);

}

void ConnectionControl::onConnectionEstablished(){

    button_connect->setEnabled(false);

}

void ConnectionControl::onConnectionLost(){

    button_connect->setEnabled(true);

}
