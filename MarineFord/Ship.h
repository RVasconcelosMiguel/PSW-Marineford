#ifndef SHIP_H
#define SHIP_H

#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMediaPlayer>

class Ship: public QGraphicsPolygonItem{
    //Q_OBJECT
public:
    Ship(int sz, int numid, QGraphicsItem* parent =NULL);
    void setSize(int sz);
    int getSize();
    void setOwner(QString onr);
    QString getOwner();
    bool getisPlaced();
    void setIsPlaced(bool b);

    bool isClickable;
    int id;

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void rotateShip();

    QString direction;
private:
    bool isPlaced;
    QString owner;
    int size;
    QMediaPlayer* shoot;
    QAudioOutput* shootoutput;
    QMediaPlayer* water;
    QAudioOutput* wateroutput;
};

#endif // SHIP_H
