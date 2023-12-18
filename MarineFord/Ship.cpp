#include "Ship.h"
#include "Gui.h"

#include <QGraphicsItem>
#include <QVector>
#include <QPointF>
#include <QPolygonF>
#include <QBrush>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
#include <iostream>

extern GUI * UI;

Ship::Ship(int sz, int numid, QGraphicsItem *parent){
    //draw the ship
    QVector<QPointF> shipPoints;
    shipPoints << QPointF(0,0) << QPointF(30*sz,0) << QPointF(30*sz,30) << QPointF(0,30);


    // create a polygon with the scaled points
    QPolygonF ship(shipPoints);

    // draw the poly
    setPolygon(ship);
    size=sz;
    direction=QString("horizontal");
    id = numid;
}

int Ship::getSize(){
    return size;
}

void Ship::setOwner(QString onr){
    owner=onr;
    if(onr==QString("PLAYER")){
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);
        setBrush(brush);
    }
    else if(onr==QString("MAP")){
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::lightGray);
        setBrush(brush);
    }
    else if(onr==QString("REMOVABLE")){
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::darkGray);
        setBrush(brush);
    }
}

QString Ship::getOwner(){
    return owner;
}

bool Ship::getisPlaced(){
    return isPlaced;
}

void Ship::setIsPlaced(bool b){
    isPlaced=b;
}

void Ship::rotateShip(){
    QVector<QPointF> shipPoints = this->polygon().toVector();

    // Check the initial direction
    if (direction == "horizontal") {
        // Rotate the points 90 degrees clockwise
        for (int i = 0; i < shipPoints.size(); ++i) {
            qreal x = shipPoints[i].x();
            qreal y = shipPoints[i].y();
            shipPoints[i].setX(-y+30);
            shipPoints[i].setY(+x);
        }

        // Update direction to vertical
        direction = "vertical";
    } else {
        // Rotate the points -90 degrees (clockwise) for vertical
        for (int i = 0; i < shipPoints.size(); ++i) {
            qreal x = shipPoints[i].x();
            qreal y = shipPoints[i].y();
            shipPoints[i].setX(+y);
            shipPoints[i].setY(-x+30);
        }

        // Update direction to horizontal
        direction = "horizontal";
    }

    // Create a new rotated polygon
    QPolygonF rotatedPolygon(shipPoints);

    // Set the new polygon for the ship
    setPolygon(rotatedPolygon);
}

void Ship::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //if is not placed pick it up

    if(UI->inGame==false){
        if(getisPlaced()==false){
            UI->pickUpShip(this);
            this->setFlag(QGraphicsItem::ItemIsFocusable);
            this->setFocus();
            std::cout << "picking up" << std::endl;
        }
        else if(UI->shipToPlace!=NULL){
            std::cout << "placeship" << std::endl;
            UI->placeShip(this);
        }
    }
    else if(this->isClickable && UI->imPlaying){
        UI->scene->removeItem(this);
        int pos;
        for(int i = 0 ; i< UI->map->removableShips.size(); i++){
            if(UI->map->removableShips.at(i)==this){
                pos=i;
            }
        }
        std::string strNumber = std::to_string(pos);
        if(UI->imServer){
            if(pos<10){
                UI->server.write("0");
            }
            UI->server.write(const_cast<char*>(strNumber.c_str()));
        }
        else{
            if(pos<10){
                UI->client.sendData("0");
            }
            UI->client.sendData(const_cast<char*>(strNumber.c_str()));
        }
        //qDebug()<< "POS:" <<pos;
        //qDebug() << "X:" << (pos - pos%10)/10 << "Y: " << pos%10;
        if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==0){
            UI->imPlaying = !(UI->imPlaying);
            qDebug() << "switch";

            UI->scene->removeItem(UI->titleText);
            UI->titleText = new QGraphicsTextItem("Enemy's Turn");
            UI->titleText->setDefaultTextColor("red");
            QFont titleFont("Courier",25, QFont::Bold);
            UI->titleText->setFont(titleFont);
            UI->titleText->setPos(75, 400);
            UI->scene->addItem(UI->titleText);
            water=new QMediaPlayer();
            wateroutput= new QAudioOutput();
            water->setAudioOutput(wateroutput);
            wateroutput->setVolume(0.1);
            water->setSource(QUrl("qrc:/Sounds/agua.mp3"));
            water->play();
        }
        else{
            UI->myHits++;
            shoot= new QMediaPlayer();
            shootoutput= new QAudioOutput();
            shoot->setAudioOutput(shootoutput);
            shootoutput->setVolume(0.01);
            shoot->setSource(QUrl("qrc:/Sounds/shoot.mp3"));
            shoot->play();

            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==1){
                //ui->shipDown;
                UI->shipsDown++;
                UI->shipDown();
            }
            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==2){
                UI->countShipId2++;
                if(UI->countShipId2>=2){
                    //ui->shipDown;
                    UI->shipsDown++;
                    UI->shipDown();
                }
            }
            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==3){
                UI->countShipId3++;
                if(UI->countShipId3>=2){
                    UI->shipsDown++;
                    UI->shipDown();
                }
            }
            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==4){
                UI->countShipId4++;
                if(UI->countShipId4>=2){
                    //ui->shipDown;
                    UI->shipsDown++;
                    UI->shipDown();
                }
            }
            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==5){
                UI->countShipId5++;
                if(UI->countShipId5>=3){
                    //ui->shipDown;
                    UI->shipsDown++;
                    UI->shipDown();
                }
            }
            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==6){
                UI->countShipId6++;
                if(UI->countShipId6>=3){
                    //ui->shipDown;
                    UI->shipsDown++;
                    UI->shipDown();
                }
            }
            if(UI->map->enemyBinMap[(pos - pos%10)/10][pos%10]==7){
                UI->countShipId7++;
                if(UI->countShipId7>=4){
                    //ui->shipDown;
                    UI->shipsDown++;
                    UI->shipDown();
                }
            }

            if(UI->myHits>=20){
                UI->gameOverWin();
            }
        }
    }
}

void Ship::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_R){
        rotateShip();
    }
}
