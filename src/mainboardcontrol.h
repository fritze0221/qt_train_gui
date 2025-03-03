#ifndef MAINBOARDCONTROL_H
#define MAINBOARDCONTROL_H

#include <QWidget>
#include <QLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QComboBox>
#include <QLabel>

#include "config.h"

namespace Ui {
class MainboardControl;
}

class MainboardControl : public QWidget
{
    Q_OBJECT

public:
    explicit MainboardControl(QWidget *parent = nullptr);

    ~MainboardControl();


private:
    Ui::MainboardControl *ui;

    int switch_1_state = 0;
    int switch_2_state = 0;

    QVector<int> house_1_state = {0};
    int house_1_state_size = 3;

    QVector<int> house_2_state = {0};
    int house_2_state_size = 8;

    void addCheckableItem(const QString &text, QComboBox* box,bool checkable);

private slots:

    void onPushButtonSwitch1();
    void onPushButtonSwitch2();

    void onToggleHouse1(const QModelIndex &index);
    void onToggleHouse2(const QModelIndex &index);

signals:

    void sendSwitch1State(int state);
    void sendSwitch2State(int state);

    void sendHouse1State(QVector<int> state);
    void sendHouse2State(QVector<int> state);
};

#endif // MAINBOARDCONTROL_H
