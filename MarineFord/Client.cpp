// client.cpp
#include "Client.h"
#include "Gui.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, [=]() {
        qDebug() << "Connected to server.";
    });

    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray receivedDataC = socket->readAll();

        // Check the type of message
        if (receivedDataC.startsWith("CHAT:")) {
            // This is a chat message
            chatMessage = receivedDataC.mid(5); // Assuming "CHAT:" is a prefix
            //handle chat
            emit chatDataReceived(chatMessage);
            qDebug() << chatMessage;
        } else {
            // This is a data message
            for (size_t i = 0; i < 10; ++i) {
                for (size_t j = 0; j < 10; ++j) {
                    if (i * 10 + j < receivedDataC.size()) {
                        char byteValue = receivedDataC.at(i * 10 + j);

                        // Convert the byte to an integer
                        int intValue = static_cast<unsigned char>(byteValue);

                        // Assign the converted value to receivedMap
                        receivedMap[i][j] = intValue - '0';  // Subtract '0' to convert from ASCII to integer
                    }
                }
            }
            qDebug() << "Data message received. X:" << receivedMap[0][0] << "Y:" << receivedMap[0][1];
            emit dataReceived(receivedDataC);
        }
    });

    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        qDebug() << "Disconnected from server.";
        emit clientDisconnected();
    });
}


void Client::connectToServer(const QString &hostAddress, quint16 port)
{
    socket->connectToHost(hostAddress, port);
    connect(socket, &QTcpSocket::connected, this, &Client::handleConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &Client::handleConnectionError);
}

void Client::handleConnected() {
    //Handle successful connection
    qDebug() << "Connected to the server!";
    emit clientConnected();
}

void Client::handleConnectionError(QAbstractSocket::SocketError socketError) {
    //Handle connection error
    qDebug() << "Connection error:" << socket->errorString();
}

void Client::disconnectFromSever()
{
    qDebug() << "disc from host";
    socket->disconnectFromHost();
}

void Client::sendData(char *data)
{
    socket->write(data);
}
