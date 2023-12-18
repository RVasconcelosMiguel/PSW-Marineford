#include "Server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    socket = nullptr;
}

void Server::write(char *data)
{
    socket->write(data);
}

void Server::end()
{
    if (socket != nullptr) {
        close();
        socket->deleteLater();  // Optionally delete the socket
        socket = nullptr;  // Reset the pointer to nullptr
        qDebug() << "Socket closed.";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    if (socket == nullptr) {
        socket = new QTcpSocket(this);
        socket->setSocketDescriptor(socketDescriptor);

        connect(socket, &QTcpSocket::readyRead, this, [=]() {
            QByteArray receivedDataS = socket->readAll();
            if (receivedDataS.startsWith("CHAT:")) {
                // This is a chat message
                chatMessage = receivedDataS.mid(5); // Assuming "CHAT:" is a prefix
                //handle chat
                emit chatDataReceived(chatMessage);
                qDebug() << "CHAT RECEIVED: " << chatMessage;
            } else {
                // This is a data message
                for (size_t i = 0; i < 10; ++i) {
                    for (size_t j = 0; j < 10; ++j) {
                        if (i * 10 + j < receivedDataS.size()) {
                            char byteValue = receivedDataS.at(i * 10 + j);
                            int intValue = static_cast<unsigned char>(byteValue);
                            receivedMap[i][j] = intValue - '0';
                        }
                    }
                }
                qDebug() << "Data message received. X:" << receivedMap[0][0] << "Y:" << receivedMap[0][1];
                emit dataReceived(receivedDataS);
            }
        });

        qDebug() << "Connection established with client.";

        connect(socket, &QTcpSocket::disconnected, this, [=]() {
            qDebug() << "Client disconnecteddd.";
            emit serverDisconnect();
            // Your handling code for client disconnection
            end();
        });

        // close();
    }
    else {
        qDebug() << "Another connection attempt ignored.";
        QTcpSocket tempSocket;
        tempSocket.setSocketDescriptor(socketDescriptor);
        tempSocket.waitForDisconnected();
    }
}
