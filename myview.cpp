#include "myview.h"
#include <QPixmap>
#include <QGraphicsScene>

MyView::MyView()
{
    initView();
}

void MyView::initView()
{
    setMinimumSize(810,610);
    setMaximumSize(810,610);
    setBackgroundBrush(Qt::black);
    QGraphicsScene *scene=new QGraphicsScene(this);
    scene->setSceneRect(-400,-300,800,600);
    topLine=scene->addLine(-400,-300,400,-300);
    leftLine=scene->addLine(-400,-300,-400,300);
    bottomLine=scene->addLine(-400,300,400,300);
    rightLine=scene->addLine(400,-300,400,300);
    setScene(scene);
    addBarrier();
    createTanke();

}

void MyView::createTanke()
{
    //实例化tankes后一定要将其加入场景中，因为tanke类中有访问场景，不然内存访问出错。
    Tankes *tankesArr=new Tankes[20];
    for(int i=0;i<20;i++)
    {
        if(i<6)
        {
            scene()->addItem(&tankesArr[i]);
            tankesArr[i].setPos(-380,-280);
        }else if(i<12)
        {
            scene()->addItem(&tankesArr[i]);
            tankesArr[i].setPos(0,-280);
        }else if(i<20)
        {
            scene()->addItem(&tankesArr[i]);
            tankesArr[i].setPos(380,-280);
        }
    }
//    scene()->addItem(&tankesArr[0]);
//    tankesArr[0].setPos(380,-280);
    mMyTanke=new Tanke;
    scene()->addItem(mMyTanke);
    mMyTanke->setPos(0,280);
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    if(mMyTanke->isVisible())
        mMyTanke->setFocus();
    QGraphicsView::keyPressEvent(event);
}

void MyView::addBarrier()
{
//    QPixmap *pixmapArr=new QPixmap[10];
//    QGraphicsPixmapItem *pixmapItemArr[10];
//    for(int i=0;i<10;i++)
//    {
//        pixmapArr[i].load(tr(":/images/whiteWall.png"));
//    }
//    pixmapItemArr[0]=scene()->addPixmap(pixmapArr[0]);
//    pixmapItemArr[0]->setPos(-400,-260);
    //上部分
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(-340,-260));
    addBarrierLine(REDWALL,Qt::Vertical,6,QPointF(-190,-260));
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(-75,-260));
    addBarrierLine(WHITEWALL,Qt::Horizontal,1,QPointF(-25,-135));
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(25,-260));
    addBarrierLine(REDWALL,Qt::Vertical,6,QPointF(140,-260));
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(290,-260));
    //中间部分
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(-75,-35));
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(25,-35));

    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(-400,-35));
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(350,-35));

    addBarrierLine(REDWALL,Qt::Horizontal,3,QPointF(150,0));
    addBarrierLine(REDWALL,Qt::Horizontal,3,QPointF(-300,0));
    //下面部分
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(-300,80));
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(-190,80));
    addBarrierLine(REDWALL,Qt::Vertical,3,QPointF(23,40));
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(-25,70));
    addBarrierLine(REDWALL,Qt::Vertical,3,QPointF(-73,40));
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(140,80));
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(250,80));

}

void MyView::addBarrierLine(MyView::BarrierType type, Qt::Orientation orientation, const int count,const QPointF position)
{
    qreal x=position.x();
    qreal y=position.y();
    if(orientation==Qt::Horizontal)
    {
        for(int i=0;i<count;i++)
        {
            QPixmap pixmap;
            QGraphicsPixmapItem *pixmapItem;
            switch(type)
            {
            case BLUEWATER:
                pixmap.load(tr(":/images/blueWater.png"));
                pixmapItem=scene()->addPixmap(pixmap);
                pixmapItem->setPos(x+i*pixmapItem->boundingRect().width(),y);
                break;
            case WHITEWALL:
                pixmap.load(tr(":/images/whiteWall.png"));
                pixmapItem=scene()->addPixmap(pixmap);
                pixmapItem->setPos(x+i*pixmapItem->boundingRect().width(),y);
                break;
            case REDWALL:
                pixmap.load(tr(":/images/redWall.png"));
                pixmapItem=scene()->addPixmap(pixmap);
                pixmapItem->setPos(x+i*pixmapItem->boundingRect().width(),y);
                break;
            }
        }
    }else if(orientation==Qt::Vertical)
    {
        for(int i=0;i<count;i++)
        {
            QPixmap pixmap;
            QGraphicsPixmapItem *pixmapItem;
            switch(type)
            {
            case BLUEWATER:
                pixmap.load(tr(":/images/blueWater.png"));
                pixmapItem=scene()->addPixmap(pixmap);
                pixmapItem->setPos(x,y+i*pixmapItem->boundingRect().height());
                break;
            case WHITEWALL:
                pixmap.load(tr(":/images/whiteWall.png"));
                pixmapItem=scene()->addPixmap(pixmap);
                pixmapItem->setPos(x,y+i*pixmapItem->boundingRect().height());
                break;
            case REDWALL:
                pixmap.load(tr(":/images/redWall.png"));
                pixmapItem=scene()->addPixmap(pixmap);
                pixmapItem->setPos(x,y+i*pixmapItem->boundingRect().height());
                break;
            }
        }
    }

}


