#include "mywall.h"
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#define qd qDebug()<<
MyWall::MyWall(Shape shape, int liveValue)
{
    mShape=shape;
    mLifeVal=liveValue;
    //³õÊ¼»¯»ÙÃðÒôÐ§
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(tr("../MyTanke/walldown.wav")));
    Phonon::AudioOutput *audioOutput=new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(shootOverSound,audioOutput);
}

QRectF MyWall::boundingRect() const
{
    return QRectF(0,0,25,25);
}


void MyWall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    switch(mShape)
    {
    case RedWall:
        pixmap.load(tr(":/images/redWall.png"));
        break;
    case WhiteWall:
        pixmap.load(tr(":/images/whiteWall.png"));
        break;
    case BlueWater :
        pixmap.load(tr(":/images/blueWater.png"));
        break;
    }
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawPixmap(0,0,25,25,pixmap);
}


void MyWall::beShoot()
{

    QGraphicsBlurEffect *blur=new QGraphicsBlurEffect;
    setGraphicsEffect(blur);

    QPropertyAnimation *animation=new QPropertyAnimation(this,"scale");
    animation->setDuration(100);
    animation->setStartValue(1.0);
    mLifeVal--;
    if(mLifeVal==1)
    {
        animation->setEndValue(0.9);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }else if(mLifeVal==0)
    {
        shootOverSound->play();
        animation->setEndValue(0.0);
        connect(animation,SIGNAL(finished()),this,SLOT(slotDestroy()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }else
    {
        animation->deleteLater();
    }
}



void WallBox::initBox()
{
    for(int i=0;i<4;i++)
    {
        MyWall *wall=new MyWall();
        switch(mWallType)
        {
        case WhiteWall:
            wall->setWallType(MyWall::WhiteWall);
            break;
        case RedWall:
            wall->setWallType(MyWall::RedWall);
            break;
        case BlueWater:
            wall->setWallType(MyWall::BlueWater);
            break;
        }
        addToGroup(wall);
        switch(i)
        {
        case 0:
            wall->setPos(-25,-25);
            break;
        case 1:
            wall->setPos(0,-25);
            break;
        case 2:
            wall->setPos(-25,0);
            break;
        case 3:
            wall->setPos(0,0);
            break;
        }
    }
}

void WallBox::clearBroup()
{
    QList<QGraphicsItem*>list=childItems();
    foreach(QGraphicsItem*item,list)
    {
        removeFromGroup(item);
    }
}
