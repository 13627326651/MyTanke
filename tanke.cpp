#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QTime>
#include <QKeyEvent>
#include "mywall.h"
#include "enemy.h"
#include <QDebug>
#include "bullet.h"

#define SOURCEPATH tr("../MyTanke/music/")

#define qd qDebug()<<
//我的坦克

Tanke::Tanke(int step, int duration,int bulletSpeed)
{
    mPause=false;
    mStep=step;
    mDuration=duration;
    mSpeed=bulletSpeed;
    beginStep=30;
    beginDuration=450;
    mSrcRotation=rotation();
    mBulletType=Standard;
    setFlag(QGraphicsItem::ItemIsFocusable);
    //射击音效
    shootSound=new Phonon::MediaObject(this);
    connect(shootSound,SIGNAL(finished()),shootSound,SLOT(stop()));
    shootSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("startshoot.mp3")));
    Phonon::AudioOutput *shootOutput=new Phonon::AudioOutput(Phonon::VideoCategory,this);
    Phonon::createPath(shootSound,shootOutput);
    //被射中音效
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("lose.mp3")));
    Phonon::AudioOutput *shootOverOutput=new Phonon::AudioOutput(Phonon::VideoCategory,this);
    Phonon::createPath(shootOverSound,shootOverOutput);
    //设置被击中的动画
    mAniBeShoot=new QPropertyAnimation(this,"rotation");
    connect(mAniBeShoot,SIGNAL(finished()),this,SLOT(slotDestroy()));
}

void Tanke::shoot()
{
    //选择子弹
    Bullet *bullet;
    switch(mBulletType)
    {
    case Standard:
        bullet=new Bullet;
        bullet->setSpeed(mSpeed);
        break;
    case RedSord:
        bullet=new RedSordBullet;
        break;
    }
    bullet->setUser(RedSordBullet::Friend);
    //设置子弹方向
    int rt=rotation();
    if(rt==mSrcRotation){
        bullet->setDirection(Bullet::UP);
    }else if(rt==mSrcRotation+180){
        bullet->setDirection(Bullet::DOWN);
    }else if(rt==mSrcRotation-90){
        bullet->setDirection(Bullet::LEFT);
    }else if(rt==mSrcRotation+90){
        bullet->setDirection(Bullet::RIGHT);
    }
    scene()->addItem(bullet);
    bullet->setPos(pos());
    //射击
    bullet->bulletShoot();
}



bool Tanke::maybeCollide(QPointF endPos)
{
    QRectF rectF(boundingRect());
    QGraphicsRectItem *item=scene()->addRect(rectF);
    item->hide();
    item->setPos(endPos);
    QList<QGraphicsItem*>list=item->collidingItems();
    int num=list.count();
    if(list.count()>0) {
         foreach(QGraphicsItem *items,list){
             qreal x=items->boundingRect().width();
             qreal y=items->boundingRect().height();
             if(x==36&&y==36)
                  num--;
             //if(x==10&&y==4)
               //   num--;
             if(x==25&&y==25||x==50&&y==50)
             {
                 MyWall *wall=(MyWall*)items;
                 if(wall->getShape()==MyWall::GreenGrass)
                 {
                     wall->setZValue(1);
                     num--;
                 }
             }
         }
     }
    scene()->removeItem(item);
    delete item;
    if(num>0)
        return true;
    else
        return false;
}

void Tanke::slotDestroy()
{   
    emit sgDestroy();
    hide();
}

void Tanke::slotBeShoot()
{
   pause();
   shootOverSound->play();

   mAniBeShoot->setStartValue(0);
   mAniBeShoot->setEndValue(1080);
   mAniBeShoot->setDuration(400);
   mAniBeShoot->start();
}

void Tanke::keyPressEvent(QKeyEvent *event)
{
    if(mPause)
        return;
    if(event->isAutoRepeat()&&event->key()==Qt::Key_Space)
        return;
    if(event->key()==Qt::Key_Space)
    {
        shootSound->play();
        shoot();
        return;
    }
    QPropertyAnimation *mAniMoving=new QPropertyAnimation(this,"pos");

    int step=mStep;
    mAniMoving->setStartValue(pos());
    mAniMoving->setDuration(mDuration);
    if(!event->isAutoRepeat())
    {
        step=beginStep;
        mAniMoving->setDuration(beginDuration);
    }
    QPointF temp;
    switch(event->key())
    {
    case Qt::Key_Up:
        temp=pos()+QPointF(0,0-step);
        setRotation(mSrcRotation);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    case Qt::Key_Down:
        temp=pos()+QPointF(0,step);
        setRotation(mSrcRotation+180);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    case Qt::Key_Left:
        temp=pos()+QPointF(0-step,0);
        setRotation(mSrcRotation-90);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    case Qt::Key_Right:
        temp=pos()+QPointF(step,0);
        setRotation(mSrcRotation+90);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    }


}





