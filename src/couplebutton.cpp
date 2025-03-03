#include "couplebutton.h"

CoupleButton::CoupleButton(QWidget *parent)
    : QWidget{parent}
{

    couple_button = new QPushButton("Entkoppelt", this);

    connect(couple_button, &QPushButton::clicked, this, &CoupleButton::onCoupleButtonClicked);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(couple_button);

    setLayout(layout);
}

void CoupleButton::onCoupleButtonClicked(){

    if(couple_button_state == 0){

        couple_button->setText("Gekoppelt");

        couple_button_state = 1;
    }

    else{

        couple_button->setText("Entkoppelt");

        couple_button_state = 0;
    }

    emit sendCoupleButtonClicked(couple_button_state);

}
