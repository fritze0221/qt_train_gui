
#include <QCoreApplication>

#include "train_1_handle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    TrainHandle* train = new TrainHandle();

    QObject::connect(train, &TrainHandle::sendQuit, &app, &QCoreApplication::quit);

    return app.exec();
}