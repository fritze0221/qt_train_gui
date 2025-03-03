#ifndef STOPBUTTON_H
#define STOPBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>

class StopButton : public QWidget
{
    Q_OBJECT
public:
    explicit StopButton(QWidget *parent = nullptr);

private:

    QPushButton* stop_button;

signals:

    void sendStopButtonClicked();

};

#endif // STOPBUTTON_H
