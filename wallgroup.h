#ifndef WALLGROUP_H
#define WALLGROUP_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QPainter>
class WallGroup : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum WallType{WhiteWall,RedWall,BlueWater};
    explicit WallGroup(){}
    QRectF boundingRect() const{
        return QRectF(0,0,800,600);
    }
    virtual void createMap()=0;
    void addBarrierLine(WallType type,Qt::Orientation orientation,
                   const int count,const QPointF position);
    void clearBroup();
};

class GateOne :public WallGroup
{
    Q_OBJECT
public:
    explicit GateOne(){createMap();}
    void createMap();
};


#endif // WALLGROUP_H
