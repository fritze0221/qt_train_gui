#include "mainboardcontrol.h"
#include "ui_mainboardcontrol.h"

MainboardControl::MainboardControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainboardControl)
{
    QWidget* centralWidget = new QWidget();

    ui->setupUi(centralWidget);

    house_1_state.resize(house_1_state_size);
    house_2_state.resize(house_2_state_size);

    for(int i = 0;i < house_1_state_size; i++) house_1_state[i] = 0;
    for(int i = 0;i < house_2_state_size; i++) house_2_state[i] = 0;

    addCheckableItem("Haus 1", ui->combo_box_house_1, false);

    addCheckableItem("UG Zimmer 1", ui->combo_box_house_1, true);
    addCheckableItem("Dachboden", ui->combo_box_house_1, true);
    addCheckableItem("UG Zimmer 2", ui->combo_box_house_1, true);

    addCheckableItem("Haus 2", ui->combo_box_house_2, false);

    addCheckableItem("Anbau 1", ui->combo_box_house_2, true);
    addCheckableItem("Anbau 2", ui->combo_box_house_2, true);
    addCheckableItem("Dachboden", ui->combo_box_house_2, true);
    addCheckableItem("OG Zimmer 1", ui->combo_box_house_2, true);
    addCheckableItem("OG Zimmer 2", ui->combo_box_house_2, true);
    addCheckableItem("OG Balkon", ui->combo_box_house_2, true);
    addCheckableItem("OG Teppe", ui->combo_box_house_2, true);
    addCheckableItem("UG Zimmer", ui->combo_box_house_2, true);

    ui->label_switch_11->setVisible(false);
    ui->label_switch_20->setVisible(false);

    connect(ui->combo_box_house_1->model(), &QStandardItemModel::dataChanged, this, &MainboardControl::onToggleHouse1);
    connect(ui->combo_box_house_2->model(), &QStandardItemModel::dataChanged, this, &MainboardControl::onToggleHouse2);

    connect(ui->pushButtonSwitch1, &QPushButton::clicked, this, &MainboardControl::onPushButtonSwitch1);
    connect(ui->pushButtonSwitch2, &QPushButton::clicked, this, &MainboardControl::onPushButtonSwitch2);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(centralWidget);

    setLayout(layout);
    setStyleSheet("background-color: rgb(255, 255, 255);");

}


void MainboardControl::onPushButtonSwitch1(){

    if(switch_1_state == 0){

        ui->label_switch_10->setVisible(false);
        ui->label_switch_11->setVisible(true);

        switch_1_state = 1;

    }

    else{

        ui->label_switch_10->setVisible(true);
        ui->label_switch_11->setVisible(false);

        switch_1_state = 0;

    }

    emit sendSwitch1State(switch_1_state);

}

void MainboardControl::onPushButtonSwitch2(){


    if(switch_2_state == 0){

        ui->label_switch_20->setVisible(true);
        ui->label_switch_21->setVisible(false);

        switch_2_state = 1;

    }

    else{

        ui->label_switch_20->setVisible(false);
        ui->label_switch_21->setVisible(true);

        switch_2_state = 0;

    }

    emit sendSwitch2State(switch_2_state);

}



void MainboardControl::onToggleHouse1(const QModelIndex &index){

    if(house_1_state[index.row()-1] == 0) house_1_state[index.row()-1] = 1;
    else{

        house_1_state[index.row()-1] = 0;

    }

    emit sendHouse1State(house_1_state);

}


void MainboardControl::onToggleHouse2(const QModelIndex &index){

    if(house_2_state[index.row()-1] == 0) house_2_state[index.row()-1] = 1;
    else{

        house_2_state[index.row()-1] = 0;

    }

    emit sendHouse2State(house_2_state);
}

void MainboardControl::addCheckableItem(const QString &text, QComboBox* box, bool checkable){

    QStandardItem *item = new QStandardItem(text);

    if (checkable) {
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Unchecked, Qt::CheckStateRole);
    } else {
        item->setFlags(Qt::ItemIsEnabled);
    }    item->setData(Qt::Unchecked, Qt::CheckStateRole);

    static_cast<QStandardItemModel *>(box->model())->appendRow(item);


}


MainboardControl::~MainboardControl()
{
    delete ui;
}
