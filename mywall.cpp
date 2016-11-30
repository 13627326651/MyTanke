#include "mywall.h"
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
    return QRectF(-25,-25,50,50);
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
    painter->drawPixmap(-25,-25,50,50,pixmap);
}

void MyWall::setWallType(MyWall::Shape shape)
{
    mShape=shape;
}

int MyWall::getLiveValue()
{
    return mLifeVal;
}

void MyWall::beShoot()
{
    shootOverSound->play();
    QGraphicsBlurEffect *blur=new QGraphicsBlurEffect;
    setGraphicsEffect(blur);

    QPropertyAnimation *animation=new QPropertyAnimation(this,"scale");
    animation->setDuration(100);
    animation->setStartValue(1.0);

    if(mLifeVal==2)
    {
        animation->setEndValue(0.9);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
       mLifeVal--;

    }else if(mLifeVal==1)
    {
        animation->setEndValue(0.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        mLifeVal--;

    }else
    {
        animation->deleteLater();
        slotDestroy();
    }
}

void MyWall::slotDestroy()
{
    hide();
}
