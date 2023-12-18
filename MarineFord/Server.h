// server.h
#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "Player.h"

class Server : public QTcpServer, public Player
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    QByteArray receivedDataS;
    void write(char *data);
    void end();


protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void serverDisconnect();
    void dataReceived(const QByteArray& data);
    void chatDataReceived(const QString chat);

private:
    QTcpSocket *socket;
};

#endif // SERVER_H
