#include "wallgroup.h"
#include "mywall.h"

void WallGroup::addBarrierLine(WallType type,
                               Qt::Orientation orientation,
                               const int count, const QPointF position,
                               int liveVal )
{
    qreal x=position.x();
    qreal y=position.y();
    QList<WallBox*>list;
    for(int i=0;i<count;i++){
        WallBox *wall=new WallBox();
        wall->setLiveVal(liveVal);
        switch(type)
        {
        case BlueWater:
            wall->setWallType(WallBox::BlueWater);
            break;
        case WhiteWall:
            wall->setWallType(WallBox::WhiteWall);
            break;
        case RedWall:
            wall->setWallType(WallBox::RedWall);
            break;
        case GreenGrass:
            wall->setWallType(WallBox::GreenGrass);
            break;
        }
        wall->initBox();
        //把墙盒子加入墙组
        addToGroup(wall);
        list<<wall;
    }
    if(orientation==Qt::Horizontal)
    {
        for(int i=0;i<list.count();i++)
        {
            list.at(i)->setPos(x+i*list.at(i)->boundingRect().width(),y);
            list.at(i)->clearBroup();
            list.at(i)->deleteLater();
        }
    }else if(orientation==Qt::Vertical)
    {
        for(int i=0;i<list.count();i++)
        {
            list.at(i)->setPos(x,y+i*list.at(i)->boundingRect().height());
            list.at(i)->clearBroup();
            list.at(i)->deleteLater();
        }
    }

}

void WallGroup::addBarrierLine4(WallGroup::WallType type,
                                Qt::Orientation orientation,
                                const int count, const QPointF position,
                                int liveVal)
{
    qreal x=position.x();
    qreal y=position.y();
    QList<WallBox*>list;
    for(int i=0;i<count;i++){
        WallBox *wall=new WallBox4();
        wall->setLiveVal(liveVal);
        switch(type)
        {
        case BlueWater:
            wall->setWallType(WallBox::BlueWater);
            break;
        case WhiteWall:
            wall->setWallType(WallBox::WhiteWall);
            break;
        case RedWall:
            wall->setWallType(WallBox::RedWall);
            break;
        case GreenGrass:
            wall->setWallType(WallBox::GreenGrass);
            break;
        }
        wall->initBox();
        //把墙盒子加入墙组
        addToGroup(wall);
        list<<wall;
    }
    if(orientation==Qt::Horizontal)
    {
        for(int i=0;i<list.count();i++)
        {
            list.at(i)->setPos(x+i*list.at(i)->boundingRect().width(),y);
            list.at(i)->clearBroup();
            list.at(i)->deleteLater();
        }
    }else if(orientation==Qt::Vertical)
    {
        for(int i=0;i<list.count();i++)
        {
            list.at(i)->setPos(x,y+i*list.at(i)->boundingRect().height());
            list.at(i)->clearBroup();
            list.at(i)->deleteLater();
        }
    }

}

void WallGroup::clearBroup()
{
    QList<QGraphicsItem*>list=childItems();
    foreach(QGraphicsItem*item,list)
    {
        removeFromGroup(item);
    }
}



void GateOne::createMap()
{
    addBarrierLine4(GreenGrass,Qt::Horizontal,15,QPointF(25,300),3);
}


void GateTwo::createMap()
{
    QPointF point1(85,85);//redWall 2
    QPointF point2(195,25);//whitewall 2
    QPointF point3(460,25);
    QPointF point4(460,75);
    QPointF point5(620,85);
    QPointF point6(670,135);
    QPointF point7(720,85);

    QPointF point8(25,245);
    QPointF point9(250,305);
    QPointF point10(195,185);
    QPointF point11(305,305);
    QPointF point12(355,235);
    QPointF point13(455,305);
    QPointF point14(670,235);
    QPointF point15(720,235);

    addBarrierLine(RedWall,Qt::Vertical,2,point1,2);
    addBarrierLine4(WhiteWall,Qt::Vertical,2,point2,-1);
    addBarrierLine4(WhiteWall,Qt::Vertical,1,point3,-1);
    addBarrierLine(RedWall,Qt::Vertical,2,point4,2);
    addBarrierLine(RedWall,Qt::Vertical,4,point5,2);
    addBarrierLine4(WhiteWall,Qt::Vertical,1,point6,-1);
    addBarrierLine(RedWall,Qt::Vertical,2,point7,2);

    addBarrierLine4(GreenGrass,Qt::Vertical,2,point8,0);
    addBarrierLine4(GreenGrass,Qt::Vertical,1,point9,0);
    addBarrierLine(RedWall,Qt::Vertical,2,point10,2);
    addBarrierLine(RedWall,Qt::Vertical,1,point11,2);
    addBarrierLine4(WhiteWall,Qt::Vertical,1,point12,-1);
    addBarrierLine4(WhiteWall,Qt::Vertical,1,point13,-1);
    addBarrierLine4(GreenGrass,Qt::Vertical,3,point14,0);
    addBarrierLine(RedWall,Qt::Horizontal,2,point15,2);

    //addBarrierLine(BlueWater,Qt::Vertical,15,QPoint);
}
