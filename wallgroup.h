#ifndef WALLGROUP_H
#define WALLGROUP_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QPainter>
class WallGroup : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum WallType{WhiteWall,RedWall,BlueWater,GreenGrass};
    explicit WallGroup(){}
    QRectF boundingRect() const{
        return QRectF(0,0,800,600);
    }
    virtual void createMap()=0;
    virtual void addBarrierLine(WallType type, Qt::Orientation orientation,
                   const int count, const QPointF position, int liveVal);
    virtual void addBarrierLine4(WallType type, Qt::Orientation orientation,
                   const int count, const QPointF position, int liveVal);
    void clearBroup();
};

//关卡一
class GateOne :public WallGroup
{
    Q_OBJECT
public:
    explicit GateOne(){createMap();}
    void createMap();
};
//关卡二
class GateTwo :public WallGroup
{
    Q_OBJECT
public:
    explicit GateTwo(){createMap();}
    void createMap();
};


#endif // WALLGROUP_H
