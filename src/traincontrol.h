#ifndef TRAINCONTROL_H
#define TRAINCONTROL_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QPainter>

#include "config.h"
#include "winterface-config.h"

class TrainControl : public QWidget
{
    Q_OBJECT
public:
    explicit TrainControl(QWidget *parent,int train_id);

private:

    QLabel* train_label;
    QLabel* curr_speed_label;

    QLabel* speed_label;
    QSlider* speed_slider;
    QSpinBox* speed_box;
    QLabel* speed_range_label;

    QLabel* acc_label;
    QSlider* acc_slider;
    QSpinBox* acc_box;
    QLabel* acc_range_label;

    QPushButton* set_button;
    QPushButton* direction_button;

    int set_train_speed = 0;
    int set_train_time = 0;
    int set_train_direction = FORWARD;

    int train_id = 0;

    bool state = true;

    bool couple_state = false;

    QRgb backgroudColor = qRgb(89,89,89);

signals:

    void sendTrainState(int set_direction,int set_speed,int set_time);
    void sendSliderValue(int value);
    void sendSetButtonClicked();
    void sendDirectionButtonClicked(int direction);

public slots:

    void onCurrSpeed(int value);
    void onCoupleButtonClicked();

    void onSliderValue(int value);
    void onExternalSetButtonClicked();
    void onExternalDirectionButtonClicked(int direction);

private slots:

    void onSpeedSlider(int value);
    void onAccSlider(int value);
    void onDirectionButton();
    void onSetButton();


protected:

    void paintEvent(QPaintEvent* event) override;

};

#endif // TRAINCONTROL_H
