#ifndef COUPLEBUTTON_H
#define COUPLEBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QLayout>

class CoupleButton : public QWidget
{
    Q_OBJECT
public:
    explicit CoupleButton(QWidget *parent = nullptr);

private:

    QPushButton* couple_button;

    int couple_button_state = 0;

private slots:

    void onCoupleButtonClicked();

signals:

    void sendCoupleButtonClicked(int state);

};

#endif // COUPLEBUTTON_H
