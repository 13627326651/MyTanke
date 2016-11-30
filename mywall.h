#ifndef MYWALL_H
#define MYWALL_H

#include <QGraphicsObject>
#include <phonon>

class MyWall : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Shape{RedWall,WhiteWall,BlueWater};
    explicit MyWall(Shape shape=RedWall,int liveValue=2);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setWallType(Shape shape){mShape=shape;}
    int getLiveValue(){ return mLifeVal;}
    void beShoot();
public slots:
    void slotDestroy(){hide();}
private:
    Shape mShape;
    int mLifeVal;
    Phonon::MediaObject *shootOverSound;
};



class WallBox : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum WallType{WhiteWall,RedWall,BlueWater};
    explicit WallBox(WallType type=RedWall):mWallType(type){}
    QRectF boundingRect() const{
        return QRectF(-25,-25,50,50);
    }
    void initBox();
    void setWallType(WallType type){
        mWallType=type;
    }
    void clearBroup();
signals:

public slots:
private:
    WallType mWallType;
};

#endif // MYWALL_H
