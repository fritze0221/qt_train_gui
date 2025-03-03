#ifndef CONNECTIONCONTROL_H
#define CONNECTIONCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>

class ConnectionControl : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionControl(QWidget *parent = nullptr);

private:

    QPushButton* button_connect;

public slots:

    void onConnectionEstablished();
    void onConnectionLost();

signals:

    void sendConnectButtonClicked();
};

#endif // CONNECTIONCONTROL_H
