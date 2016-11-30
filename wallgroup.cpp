#include "wallgroup.h"
#include "mywall.h"

void WallGroup::addBarrierLine( WallType type,Qt::Orientation orientation,
                           const int count,const QPointF position)
{
    qreal x=position.x();
    qreal y=position.y();
    if(orientation==Qt::Horizontal){
        for(int i=0;i<count;i++){
            WallBox *wall=new WallBox();
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
            }
            //���ú����ͳ�ʼ��wall
            wall->initBox();
            //��ǽ���Ӽ���ǽ��
            addToGroup(wall);
            wall->setPos(x+i*wall->boundingRect().width(),y);
            //��ǽ�Ӻ����Ƶ���
            wall->clearBroup();
            wall->deleteLater();
        }
    }
    else if(orientation==Qt::Vertical)
    {
        for(int i=0;i<count;i++)
        {
            WallBox *wall=new WallBox();
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
            }
            //���ú����ͳ�ʼ��wall
            wall->initBox();
            //��ǽ���Ӽ���ǽ��
            addToGroup(wall);
            wall->setPos(x,y+i*wall->boundingRect().height());
            //��ǽ�Ӻ����Ƶ���
            wall->clearBroup();
            wall->deleteLater();
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
    addBarrierLine(WhiteWall,Qt::Horizontal,15,QPointF(25,300));
}
