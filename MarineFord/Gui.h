#ifndef GUI1_H
#define GUI1_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QList>
#include <QMouseEvent>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QUrl>
#include <QTextEdit>

#include "Client.h"
#include "Server.h"
#include "Map.h"
#include "Ship.h"

class GUI: public QGraphicsView{
    Q_OBJECT
public:
    GUI(QWidget* parent=NULL);
    QTextEdit* chatTextEdit;
    void setBackground(QString source);
    void setMenuText(QString title, int size, int x_pos, int y_pos, bool centralized);
    void setText(QString title, int size, int x_pos, int y_pos, QString color);
    void showMenu();
    QGraphicsScene* scene;
    QLineEdit* nameLineEdit;
    void setBackground();
    void createShips(int x, int y, int size, int numid);
    void pickUpShip(Ship* ship);
    void placeShip(Ship* mapShipToReplace);
    QPointF originalPos;
    void removeFromPlayersShips(Ship *ship);
    void gameMenu();
    void gameOverWin();
    void gameOverLose();

    void handleInvalidPos();
    void shipDown();
    void updateChatBox();

    QGraphicsTextItem *kills;

    //events
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);



    QGraphicsTextItem * setMenuTitle();
    QGraphicsTextItem* titleText;

    Map * map;
    Ship* shipToPlace;
    Server server;
    Client client;
    QString ip;
    QString msg;
    QString chatMsg;
    bool inGame = false;
    bool imServer = false;
    bool imPlaying = false;
    int myHits=0;
    int enemyHits = 0;
    int invalidPos[10][10] = {};
    int countShipId7=0;
    int countShipId6=0;
    int countShipId5=0;
    int countShipId4=0;
    int countShipId3=0;
    int countShipId2=0;
    int countShipId1=0;

    int shipsDown = 0;

private:
    QList<Ship*> playerShips;
    QMediaPlayer *background;
    QAudioOutput * audiooutput;
    QMediaPlayer* shoot;
    QAudioOutput* shootoutput;
    QMediaPlayer* water;
    QAudioOutput* wateroutput;
    QMediaPlayer* victory;
    QAudioOutput* victoryoutput;
    QMediaPlayer* defeat;
    QAudioOutput* defeatoutput;
public slots:
    void serverEnd();
    void showJoinMenu();
    void servershowShipsMenu();
    void clientshowShipsMenu();
    void showMenuSlot();
    void connectingcreate();
    void connectingjoin();
    void onClientConnected();
    void newConnection();
    void readIP(const QString &text);
    void readMsg(const QString &text);
    void serversendBinMapMenu();
    void clientsendBinMapMenu();
    void clientCheck();
    void serverCheck();
    void sendMsg();
    void updateChat();
    void updateEnemyChat();
    void restartApplication();
    void discShowMenu();
    void endGame();

signals:
    void ipUpdated(const QString &texto);
    void msgUpdated(const QString &text);
    void connectionEstablished();
};
class Button:public QObject, public QGraphicsPathItem{
    Q_OBJECT
public:
    Button(int size, QString name, QColor color,QColor hover_color, int width, int height, QGraphicsItem* parent=NULL);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void clicked();
private:
    QGraphicsTextItem* text;
    QColor btt_color;
    QColor btt_hover_color;
    bool isClickable;

};

#endif // GUI1_H

