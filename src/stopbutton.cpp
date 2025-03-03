#include "stopbutton.h"

StopButton::StopButton(QWidget *parent)
    : QWidget{parent}
{

    stop_button = new QPushButton("Stop", this);

    connect(stop_button,&QPushButton::clicked, this, &StopButton::sendStopButtonClicked);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(stop_button);

    setLayout(layout);

}
