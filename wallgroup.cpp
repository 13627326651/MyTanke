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


//    if(orientation==Qt::Horizontal){
//        for(int i=0;i<count;i++){
//            WallBox *wall=new WallBox();
//            wall->setLiveVal(liveVal);
//            switch(type)
//            {
//            case BlueWater:
//                wall->setWallType(WallBox::BlueWater);
//                break;
//            case WhiteWall:
//                wall->setWallType(WallBox::WhiteWall);
//                break;
//            case RedWall:
//                wall->setWallType(WallBox::RedWall);
//                break;
//            case GreenGrass:
//                wall->setWallType(WallBox::GreenGrass);
//                break;
//            }
//            //设置好类型初始化wall
//            wall->initBox();
//            //把墙盒子加入墙组
//            addToGroup(wall);
//            wall->setPos(x+i*wall->boundingRect().width(),y);
//            //把墙从盒子移到组
//            wall->clearBroup();
//            wall->deleteLater();
//        }
//    }
//    else if(orientation==Qt:: Vertical)
//    {
//        for(int i=0;i<count;i++)
//        {
//            WallBox *wall=new WallBox();
//            wall->setLiveVal(liveVal);
//            switch(type)
//            {
//            case BlueWater:
//                wall->setWallType(WallBox::BlueWater);
//                break;
//            case WhiteWall:
//                wall->setWallType(WallBox::WhiteWall);
//                break;
//            case RedWall:
//                wall->setWallType(WallBox::RedWall);
//                break;
//            }
//            //设置好类型初始化wall
//            wall->initBox();
//            //把墙盒子加入墙组
//            addToGroup(wall);
//            wall->setPos(x,y+i*wall->boundingRect().height());
//            //把墙从盒子移到组
//            wall->clearBroup();
//            wall->deleteLater();
//        }
//    }

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
    addBarrierLine(RedWall,Qt::Horizontal,15,QPointF(25,300),3);
}


void GateTwo::createMap()
{
    //QPointF points[20]={};
    //addBarrierLine(BlueWater,Qt::Vertical,15,QPoint);
}
