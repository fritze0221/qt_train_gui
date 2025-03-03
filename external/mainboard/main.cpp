
#include "mainboard_handle.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    MainboardHandle* mainboard = new MainboardHandle();

    QObject::connect(mainboard, &MainboardHandle::sendQuit, &app, &QCoreApplication::quit);

    return app.exec();
}