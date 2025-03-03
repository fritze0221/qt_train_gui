#include "apihandler.h"
#include <QDebug>


ApiHandler::ApiHandler(QObject *parent)
    : QObject(parent),
      m_socket(new QLocalSocket(this))
{
    // Verbinde die Signale des QLocalSocket mit den entsprechenden Slots
    connect(m_socket, &QLocalSocket::connected, this, &ApiHandler::onConnected);
    connect(m_socket, &QLocalSocket::disconnected, this, &ApiHandler::onDisconnected);
    connect(m_socket, &QLocalSocket::readyRead, this, &ApiHandler::onReadyRead);
    
    connect(m_socket, QOverload<qlonglong>::of(&QLocalSocket::bytesWritten), this, &ApiHandler::onBytesWritten);
    connect(m_socket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::errorOccurred), this, &ApiHandler::onErrorOccurred);
}

ApiHandler::~ApiHandler()
{
    if (m_socket->state() == QLocalSocket::ConnectedState) {
        m_socket->disconnectFromServer();
    }
}

void ApiHandler::connectToServer(const QString &serverName)
{
    // Versuche, eine Verbindung zum lokalen Server herzustellen
    m_socket->connectToServer(serverName);
}

void ApiHandler::sendData(const QByteArray &data)
{
    // Prüfe, ob der Socket verbunden ist, bevor Daten gesendet werden
    if (m_socket->state() == QLocalSocket::ConnectedState) {
        m_socket->write(data);
    } else {
        qWarning() << "Socket ist nicht verbunden!";
    }
}

void ApiHandler::onConnected()
{
    qDebug() << "Verbindung zum lokalen Server hergestellt.";
    emit connected();
}

void ApiHandler::onDisconnected()
{
    qDebug() << "Verbindung zum lokalen Server getrennt.";
    emit disconnected();
}

void ApiHandler::onReadyRead()
{
    // Lese alle verfügbaren Daten
    QByteArray data = m_socket->readAll();
    emit dataReceived(data);
}

void ApiHandler::onBytesWritten(qint64 bytes)
{
    qDebug() << "Daten gesendet:" << bytes << "Bytes";
    emit dataSent(bytes);
}

void ApiHandler::onErrorOccurred(QLocalSocket::LocalSocketError socketError)
{
    Q_UNUSED(socketError)
    QString errorString = m_socket->errorString();
    qWarning() << "Socket-Fehler:" << errorString;
    emit errorOccurred(errorString);
}
