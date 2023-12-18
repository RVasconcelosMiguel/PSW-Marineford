// client.h
#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>
#include "Player.h"

class Client : public QObject, public Player
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    QByteArray receivedDataC;

public slots:
    void connectToServer(const QString &hostAddress, quint16 port);
    void sendData(char *data);
    void handleConnected();
    void handleConnectionError(QAbstractSocket::SocketError socketError);
    void disconnectFromSever();
signals:
    void clientConnected();
    void dataReceived(const QByteArray& data);
    void chatDataReceived(const QString chat);
    void clientDisconnected();

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
