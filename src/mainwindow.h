#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>

#include "apihandler.h"
#include "traincontrol.h"
#include "mainboardcontrol.h"
#include "connectioncontrol.h"
#include "stopbutton.h"
#include "couplebutton.h"

#include "config.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    ApiHandler* apihandler;

    TrainControl* train_1;
    TrainControl* train_2;

    MainboardControl* mainboard;

    ConnectionControl* connection_control;

    CoupleButton* couple_button;

    StopButton* stop_button;

};
#endif // MAINWINDOW_H
