#include "mywall.h"
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#define qd qDebug()<<

/*当需要添加物体素材时，只要在MyWall,WallBox,WallGroup类中添加枚举变量，
 *分别在函数MyWall::paint(),MyWall4::paint,
 *WallBox::initBox(),WallBox4::initBox()
 *WallGroup::addBarrierLine(),WallGroup::addBarrierLine4()
 *中添加一个选择行即可
 *
 *
 */


#define SOURCEPATH tr("../MyTanke/music/")
MyWall::MyWall(Shape shape, int liveValue)
{
    mShape=shape;
    mLifeVal=liveValue;
    //初始化毁灭音效
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("walldown.wav")));
    Phonon::AudioOutput *audioOutput=new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(shootOverSound,audioOutput);
}



void MyWall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    switch(mShape)
    {
    case RedWall:
        pixmap.load(tr(":/images/redWall.gif"));
        break;
    case WhiteWall:
        pixmap.load(tr(":/images/whiteWall.png"));
        break;
    case BlueWater :
        pixmap.load(tr(":/images/blueWater.gif"));
        break;
    case GreenGrass :
        pixmap.load(tr(":/images/greenGrass.gif"));
        break;
    }
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawPixmap(0,0,25,25,pixmap);
}


void MyWall::beShoot()
{
    QPropertyAnimation *animation=new QPropertyAnimation(this,"scale");
    animation->setDuration(100);
    animation->setStartValue(1.0);
    mLifeVal--;
    if(mLifeVal>0)
    {
        if(mLifeVal==1){
            QGraphicsBlurEffect *blur=new QGraphicsBlurEffect;
            setGraphicsEffect(blur);
            animation->setEndValue(0.9);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }else{
            animation->deleteLater();
        }
        shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("attackover.mp3")));
        shootOverSound->play();
    }else if(mLifeVal==0)
    {
        shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("walldown.wav")));
        shootOverSound->play();
        animation->setEndValue(1.1);
        connect(animation,SIGNAL(finished()),this,SLOT(slotDestroy()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void WallBox::initBox()
{
    for(int i=0;i<4;i++)
    {
        MyWall *wall=new MyWall();
        wall->setLiveValue(mLiveVal);
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
        case GreenGrass:
            wall->setWallType(MyWall::GreenGrass);
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


void MyWall4::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    switch(mShape)
    {
    case RedWall:
        pixmap.load(tr(":/images/redWall4.gif"));
        break;
    case WhiteWall:
        pixmap.load(tr(":/images/whiteWall4.gif"));
        break;
    case BlueWater :
        pixmap.load(tr(":/images/blueWater.gif"));
        break;
    case GreenGrass :
        pixmap.load(tr(":/images/greenGrass.gif"));
        break;
    }
    QPen pen(Qt::red);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawPixmap(0,0,50,50,pixmap);
}


void WallBox4::initBox()
{
    MyWall *wall=new MyWall4();
    wall->setLiveValue(mLiveVal);
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
    case GreenGrass:
        wall->setWallType(MyWall::GreenGrass);
        break;
    }
    addToGroup(wall);
    wall->setPos(-25,-25);
}
