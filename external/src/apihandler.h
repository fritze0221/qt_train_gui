#ifndef LOCALSOCKETCLIENT_H
#define LOCALSOCKETCLIENT_H

#include <QObject>
#include <QLocalSocket>
#include <cstring>

class ApiHandler : public QObject
{
    Q_OBJECT
public:
    explicit ApiHandler(QObject *parent = nullptr);
    ~ApiHandler();

    // Baut eine Verbindung zum lokalen Server (serverName) auf
    void connectToServer(const QString &serverName);
    // Sendet Daten an den verbundenen lokalen Server
    void sendData(const QByteArray &data);

    QLocalSocket *m_socket;
    
signals:
    // Wird emittiert, wenn die Verbindung erfolgreich hergestellt wurde
    void connected();
    // Wird emittiert, wenn die Verbindung getrennt wurde
    void disconnected();
    // Wird emittiert, wenn ein Fehler auftritt
    void errorOccurred(const QString &error);
    // Wird emittiert, wenn Daten empfangen wurden
    void dataReceived(const QByteArray &data);
    // Wird emittiert, wenn Daten erfolgreich gesendet wurden (Anzahl der geschriebenen Bytes)
    void dataSent(qint64 bytes);

private slots:
    // Slot, der aufgerufen wird, wenn die Verbindung hergestellt wird
    void onConnected();
    // Slot, der aufgerufen wird, wenn die Verbindung getrennt wird
    void onDisconnected();
    // Slot, der aufgerufen wird, wenn Daten verfügbar sind
    void onReadyRead();
    // Slot, der aufgerufen wird, wenn Daten geschrieben wurden
    void onBytesWritten(qint64 bytes);
    // Slot, der aufgerufen wird, wenn ein Socket-Fehler auftritt
    void onErrorOccurred(QLocalSocket::LocalSocketError socketError);

};

#endif // LOCALSOCKETCLIENT_H
