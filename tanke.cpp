#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QTime>
#include <QKeyEvent>
#include "mywall.h"
#include <QDebug>

#define SOURCEPATH tr("../MyTanke/music/")

#define qd qDebug()<<
//我的坦克

Tanke::Tanke(int step, int duration,int bulletSpeed)
{
    mPause=false;
    mStep=step;
    mDuration=duration;
    mSpeed=bulletSpeed;
    mSrcRotation=rotation();
    setFlag(QGraphicsItem::ItemIsFocusable);
    //音效初始化
    //射击音效
    shootSound=new Phonon::MediaObject(this);
    connect(shootSound,SIGNAL(finished()),shootSound,SLOT(stop()));
    shootSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("startshoot.mp3")));
    Phonon::AudioOutput *shootOutput=new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(shootSound,shootOutput);
    //被射中音效
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("lose.mp3")));
    Phonon::AudioOutput *shootOverOutput=new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(shootOverSound,shootOverOutput);
}

void Tanke::shoot()
{
    int rt=rotation();
    MyBullet *bullet=new MyBullet;
    //connect(bullet,SIGNAL(sgHaveShoot()),this,SLOT(slotBeShoot()));
    bullet->setSpeed(mSpeed);
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }
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
             if(items->boundingRect().width()==36&&items->boundingRect().height()==36)
                num--;
         }
     }
    scene()->removeItem(item);
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
   QPropertyAnimation *animation=new QPropertyAnimation(this,"rotation");
   animation->setStartValue(0);
   animation->setEndValue(1080);
   animation->setDuration(400);
   animation->start(QAbstractAnimation::DeleteWhenStopped);
   connect(animation,SIGNAL(finished()),this,SLOT(slotDestroy()));
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

    int beginStep=30;
    int beginDuration=450;
    int step=mStep;
    int duration=mDuration;
    QPropertyAnimation *animation=new QPropertyAnimation(this,"pos");
    animation->setStartValue(pos());
    animation->setDuration(duration);
    if(!event->isAutoRepeat())
    {
        step=beginStep;
        animation->setDuration(beginDuration);
    }
    QPointF temp;
    switch(event->key())
    {
    case Qt::Key_Up:
        setRotation(mSrcRotation);
        temp=pos()+QPointF(0,0-step);
        if(maybeCollide(temp))
            animation->deleteLater();
        else{
            animation->setEndValue(temp);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    case Qt::Key_Down:
        temp=pos()+QPointF(0,step);
        setRotation(mSrcRotation+180);
        if(maybeCollide(temp))
            animation->deleteLater();
        else{
            animation->setEndValue(temp);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    case Qt::Key_Left:
        temp=pos()+QPointF(0-step,0);
        setRotation(mSrcRotation-90);
        if(maybeCollide(temp))
            animation->deleteLater();
        else{
            animation->setEndValue(temp);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    case Qt::Key_Right:
        temp=pos()+QPointF(step,0);
        setRotation(mSrcRotation+90);
        if(maybeCollide(temp))
            animation->deleteLater();
        else{
            animation->setEndValue(temp);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    }

}




void Bullet::bulletShoot()
{
    //子弹步进制不要超过50，因为场景边际矩形宽度为50,固定子弹速度20
   int step=20;
   QPropertyAnimation *animation=new QPropertyAnimation(this,"pos");
   animation->setDuration(mSpeed);
   animation->setStartValue(pos());
   connect(animation,SIGNAL(finished()),this,SLOT(bulletShoot()));
   switch(mDirection)
   {
   case UP:
       setRotation(mDirection-90);
        if(isColliding())
        {
            animation->deleteLater();
            deleteLater();
        }else
        {
            animation->setEndValue(pos()+QPointF(0,0-step));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
       break;
   case DOWN:
       setRotation(mDirection+90);
       if(isColliding())
       {
           animation->deleteLater();
           deleteLater();
       }else
       {
           animation->setEndValue(pos()+QPointF(0,step));
           animation->start(QAbstractAnimation::DeleteWhenStopped);
       }
      break;
   case LEFT:
       setRotation(mDirection+180);
       if(isColliding())
       {
           animation->deleteLater();
           deleteLater();
       }else
       {
           animation->setEndValue(pos()+QPointF(0-step,0));
           animation->start(QAbstractAnimation::DeleteWhenStopped);
       }
      break;
   case RIGHT:
       setRotation(mDirection);
       if(isColliding())
       {
           animation->deleteLater();
           deleteLater();
       }else
       {
           animation->setEndValue(pos()+QPointF(step,0));
           animation->start(QAbstractAnimation::DeleteWhenStopped);
       }
      break;
   }
}




Tankes::Tankes(int step,int duration,int interval,int bulletSpeed)
{
    mPauseMove=false;
    mPauseShoot=false;
    this->step=step;
    this->duration=duration;
    mBulletSpeed=bulletSpeed;
    mSrcRotation=rotation();
    //坦克初始化后就可以射击，可以通过暂停函数停止射击
    QTimer *shootTimer=new QTimer(this);
    connect(shootTimer,SIGNAL(timeout()),this,SLOT(shoot()));
    shootTimer->start(interval);
    //被击中音效
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("attackover.mp3")));
    Phonon::AudioOutput *shootOverOutput=new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(shootOverSound,shootOverOutput);
}


void Tankes::shoot()
{
    if(mPauseShoot)
        return;
    YourBullet *bullet=new YourBullet();
    bullet->setSpeed(mBulletSpeed);
    int rt=rotation();
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry());
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry());

    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry());
    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry());
    }
    bullet->bulletShoot();
}

void Tankes::slotBeShoot()
{
    pauseMove();
    pauseShoot();
    shootOverSound->play();
    QPropertyAnimation *animation=new QPropertyAnimation(this,"opacity");
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setDuration(400);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation,SIGNAL(finished()),this,SLOT(slotDestroy()));
}


bool Tankes::maybeCollide(QPointF endPos)
{
    QRectF rectF(boundingRect());
    QGraphicsRectItem *item=scene()->addRect(rectF);
    item->hide();
    item->setPos(endPos);
    QList<QGraphicsItem*>list=item->collidingItems();
    int num=list.count();
     if(list.count()>0)
     {
         foreach(QGraphicsItem *items,list)
         {
             if(items->boundingRect().width()==38&&items->boundingRect().height()==38)
                  num--;
             if(items->boundingRect().width()==11&&items->boundingRect().height()==5)
                 num--;
         }
     }
    scene()->removeItem(item);
    if(num>0)
        return true;
    else
        return false;
}


static const int turnCount=20;
static int record=0;
void Tankes::moving()
{
    if(mPauseMove)
        return;
    int index=qrand()%4;
    qreal direction[4]={mSrcRotation,mSrcRotation+180,mSrcRotation+90,mSrcRotation-90};
    if(record>turnCount)
    {
        setRotation(direction[index]);
        record=0;
    }
    record++;
    QPropertyAnimation *animation=new QPropertyAnimation(this,"pos");
    animation->setDuration(duration);
    animation->setStartValue(pos());
    connect(animation,SIGNAL(finished()),this,SLOT(moving()));
    qreal rt=rotation();
    if(rt==mSrcRotation)           //up
    {
        if(maybeCollide(pos()+QPointF(0,0-step)))
        {
           setRotation(direction[index]);
           animation->deleteLater();
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            animation->setEndValue(pos()+QPointF(0,0-step));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }else if(rt==mSrcRotation+180)      //down
    {
        if(maybeCollide(pos()+QPointF(0,step)))
        {
          setRotation(direction[index]);
          animation->deleteLater();
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            animation->setEndValue(pos()+QPointF(0,step));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }else if(rt==mSrcRotation-90)           //left
    {

        if(maybeCollide(pos()+QPointF(0-step,0)))
        {
           setRotation(direction[index]);
           animation->deleteLater();
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            animation->setEndValue(pos()+QPointF(0-step,0));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }else if(rt==mSrcRotation+90)           //right
    {
        if(maybeCollide(pos()+QPointF(step,0)))
        {
           setRotation(direction[index]);
           animation->deleteLater();
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            animation->setEndValue(pos()+QPointF(step,0));
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}


bool MyBullet::isColliding()
{
    int num=0;
    QList<QGraphicsItem*>list=collidingItems();
    num=list.count();
    if(num>0){
        foreach(QGraphicsItem *item,list){
            if(item->boundingRect().width()==38&&item->boundingRect().height()==38){
                Tankes *tankes=(Tankes*)item;
                if(!tankes->isAlive())
                      continue;
                tankes->slotBeShoot();
             }else if(item->boundingRect().width()==36&&item->boundingRect().height()==36){
                num--;
            }else if((item->boundingRect().width()==25&&
                      item->boundingRect().height()==25) ||
                      (item->boundingRect().width()==50&&
                      item->boundingRect().height()==50)){

                MyWall *wall=(MyWall*)item;
                wall->beShoot();
            }
        }
    }
    if(num>0)
        return true;
    else
        return false;
}


bool YourBullet::isColliding()
{
    QList<QGraphicsItem*>list=collidingItems();
    //根据碰撞的图型项的个数判断是否发生碰撞
    int num;
    num=list.count();
    if(num>0){
        foreach(QGraphicsItem *item,list){
            if(item->boundingRect().width()==36&&item->boundingRect().height()==36)
            {
                Tanke *tanke=(Tanke*)item;
                if(!tanke->isAlive())
                    continue;
                tanke->slotBeShoot();
            }else if(item->boundingRect().width()==38&&item->boundingRect().height()==38)
            {
                //检测到碰撞了敌方自己的坦克，num--
                num--;
            }else if(item->boundingRect().width()==25&&item->boundingRect().height()==25)
            {
                MyWall *wall=(MyWall*)item;
                wall->beShoot();
            }
        }
    }
    //num代表了碰撞的个数
    if(num>0)
        return true;
    else
      return false;
}


//发射死亡信号并销毁自己
void Tankes::slotDestroy()
{
    emit sgDestroy();
    deleteLater();
}



