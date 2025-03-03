#include "traincontrol.h"

TrainControl::TrainControl(QWidget *parent, int train_id)
    : QWidget{parent},train_id(train_id)
{

    train_label = new QLabel();

    if(train_id == TRAIN_1) train_label->setText("Zug Gelb");
    if(train_id == TRAIN_2) train_label->setText("Zug Rot");

    train_label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    train_label->setAlignment(Qt::AlignCenter);

    curr_speed_label = new QLabel();
    curr_speed_label->setText("Ist-Geschwindigkeit: 0");
    curr_speed_label->setAlignment(Qt::AlignCenter);
    curr_speed_label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    speed_label = new QLabel();
    speed_label->setText("Soll-Geschwindigkeit:");

    speed_slider = new QSlider(Qt::Horizontal);
    speed_slider->setRange(0,100);
    speed_slider->setTickInterval(1);
    speed_slider->setValue(0);

    speed_box = new QSpinBox();
    speed_box->setValue(0);
    speed_box->setRange(0,100);

    speed_range_label = new QLabel();
    speed_range_label->setText("0-100 %");
    speed_range_label->setAlignment(Qt::AlignCenter);

    acc_label = new QLabel();
    acc_label->setText("Rampenzeit:");
    acc_label->setMinimumSize(speed_label->minimumSizeHint());

    acc_slider = new QSlider(Qt::Horizontal);
    acc_slider->setRange(0,2000);
    acc_slider->setTickInterval(50);
    acc_slider->setValue(0);

    acc_box = new QSpinBox();
    acc_box->setValue(0);
    acc_box->setRange(0,2000);

    acc_range_label = new QLabel();
    acc_range_label->setText("0-2000 ms");
    acc_range_label->setAlignment(Qt::AlignCenter);

    direction_button = new QPushButton("Vorwärts");
    set_button = new QPushButton("Setzen");

    connect(speed_slider, &QSlider::valueChanged, speed_box, &QSpinBox::setValue);
    connect(speed_slider,&QSlider::valueChanged, this, &TrainControl::onSpeedSlider);
    connect(speed_box, &QSpinBox::valueChanged, speed_slider, &QSlider::setValue);

    connect(acc_slider, &QSlider::valueChanged, acc_box, &QSpinBox::setValue);
    connect(acc_slider,&QSlider::valueChanged, this, &TrainControl::onAccSlider);
    connect(acc_box, &QSpinBox::valueChanged, acc_slider, &QSlider::setValue);

    connect(direction_button, &QPushButton::clicked, this ,&TrainControl::onDirectionButton);
    connect(set_button, &QPushButton::clicked, this, &TrainControl::onSetButton);

    QVBoxLayout* train_speed_layout = new QVBoxLayout();
    train_speed_layout->addWidget(train_label);
    train_speed_layout->addWidget(curr_speed_label);

    QVBoxLayout* speed_range_layout = new QVBoxLayout();
    speed_range_layout->addWidget(speed_slider);
    speed_range_layout->addWidget(speed_range_label);

    QHBoxLayout* speed_slider_layout = new QHBoxLayout();
    speed_slider_layout->addWidget(speed_label);
    speed_slider_layout->addLayout(speed_range_layout);
    speed_slider_layout->addWidget(speed_box);

    QVBoxLayout* acc_range_layout = new QVBoxLayout();
    acc_range_layout->addWidget(acc_slider);
    acc_range_layout->addWidget(acc_range_label);

    QHBoxLayout* acc_slider_layout = new QHBoxLayout();
    acc_slider_layout->addWidget(acc_label);
    acc_slider_layout->addLayout(acc_range_layout);
    acc_slider_layout->addWidget(acc_box);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addLayout(train_speed_layout);
    layout->addLayout(speed_slider_layout);
    //layout->addLayout(acc_slider_layout);
    layout->addWidget(direction_button);
    layout->addWidget(set_button);

    setLayout(layout);


}

void TrainControl::onExternalSetButtonClicked(){

    emit sendTrainState(set_train_direction,set_train_speed,set_train_time);

}

void TrainControl::onSliderValue(int value){

    if(couple_state){

        speed_slider->setValue(value);

    }

}

void TrainControl::onCoupleButtonClicked(){

    couple_state = !couple_state;
}

void TrainControl::onExternalDirectionButtonClicked(int direction){

    if(couple_state){

        set_train_direction = direction;

        if(direction == BACKWARD) direction_button->setText("Rückwärts");
        if(direction == FORWARD) direction_button->setText("Vorwärts");

    }

}

void TrainControl::onCurrSpeed(int value){

    if(value < 0) value = 0;

    if(train_id == TRAIN_1) value = (float)(((float)value)/(TRAIN_1_MAX_SPEED-10))*100;
    if(train_id == TRAIN_2) value = (float)(((float)value)/(TRAIN_2_MAX_SPEED-100))*100;

    if(value  > 100) value = 100;

    curr_speed_label->setText(QString("Ist-Geschwindigkeit: %1").arg(value));

}

void TrainControl::onSpeedSlider(int value){

    if(train_id == TRAIN_1) set_train_speed = ((float)(TRAIN_1_MAX_SPEED-1)/(float)100)*(float)abs(value);
    if(train_id == TRAIN_2) set_train_speed = ((float)(TRAIN_2_MAX_SPEED-1)/(float)100)*(float)abs(value);

    emit sendSliderValue(value);

}

void TrainControl::onAccSlider(int value){

    set_train_time = value;


}

void TrainControl::onDirectionButton(){

    if(state){
        set_train_direction = BACKWARD;
        direction_button->setText("Rückwärts");
    }
    if(!state){

        set_train_direction = FORWARD;
        direction_button->setText("Vorwärts");

    }

    if(couple_state) emit sendDirectionButtonClicked(set_train_direction);

    state = !state;

}

void TrainControl::onSetButton(){

    emit sendTrainState(set_train_direction,set_train_speed,set_train_time);

    if(couple_state) emit sendSetButtonClicked();

}

void TrainControl::paintEvent(QPaintEvent* event){
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor color(backgroudColor);
    QBrush brush(color);

    // Bereich für das Zeichnen berechnen
    QRect rect = this->rect().adjusted(1, 1, -1, -1);

    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, 15, 15);
}

