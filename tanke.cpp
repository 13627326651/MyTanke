#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QTime>
#include <QKeyEvent>
#include <QDebug>

#define qd qDebug()<<
static const QString SOURCEPATH=QObject::tr("../MyTanke/");
//我的坦克

Tanke::Tanke(int step, int duration,int bulletSpeed)
{
    mStep=step;
    mDuration=duration;
    mSpeed=bulletSpeed;
    mIsAlive=true;
    mSrcRotation=rotation();
    setFlag(QGraphicsItem::ItemIsFocusable);
    //音效初始化
    shootSound=new Phonon::MediaObject(this);
    connect(shootSound,SIGNAL(finished()),shootSound,SLOT(stop()));
    shootSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("shoot02.mp3")));
    Phonon::AudioOutput *audioOutput=new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(shootSound,audioOutput);
}

QRectF Tanke::boundingRect() const
{
    return QRectF(-18,-18,36,36);
}

void Tanke::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/tanke01.png"));
    painter->drawPixmap(-18,-18,36,36,pixmap);
}


void Tanke::shoot()
{
    if(!mIsAlive)
        return;
    int rt=rotation();
    MyBullet *bullet=new MyBullet;
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


void Tanke::setAlive(bool alive)
{
    mIsAlive=alive;
}

bool Tanke::isAlive()
{
    return mIsAlive;
}



bool Tanke::maybeCollide(QPointF endPos)
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

void Tanke::toHide()
{
    hide();
}

void Tanke::keyPressEvent(QKeyEvent *event)
{
    setFocus();
    if(!mIsAlive&&event->key()==Qt::Key_Return)
    {
        mIsAlive=true;
        setPos(0,270);
        show();
    }
    if(!mIsAlive)
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


//子弹
Bullet::Bullet(int speed)
{
        mSpeed=speed;
}

void Bullet::setDirection(Bullet::Direction direction)
{
    mDirection=direction;
}


void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/bullet.png"));
    painter->drawPixmap(-5,-2,10,4,pixmap);
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
           // hide();
            animation->deleteLater();
            //QTimer::singleShot(mSpeed,this,SLOT(deleteLater()));
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
          // hide();
           animation->deleteLater();
           //QTimer::singleShot(mSpeed,this,SLOT(deleteLater()));
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
          // hide();
           animation->deleteLater();
           //QTimer::singleShot(mSpeed,this,SLOT(deleteLater()));
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
          // hide();
           animation->deleteLater();
           //QTimer::singleShot(mSpeed,this,SLOT(deleteLater()));
           deleteLater();
       }else
       {
           animation->setEndValue(pos()+QPointF(step,0));
           animation->start(QAbstractAnimation::DeleteWhenStopped);
       }
      break;
   }
}


void Bullet::setSpeed(const qreal speed)
{
    mSpeed=speed;
}

//敌人的坦克
QRectF Tankes::boundingRect() const
{
    return QRectF(-19,-19,38,38);
}

void Tankes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/tanke.png"));
    painter->drawPixmap(-19,-19,38,38,pixmap);
}


Tankes::Tankes(int step,int duration,int interval,int bulletSpeed)
{
    this->step=step;
    this->duration=duration;
    mBulletSpeed=bulletSpeed;
    mIsAlive=true;
    mSrcRotation=rotation();
    QTimer *shootTimer=new QTimer(this);
    connect(shootTimer,SIGNAL(timeout()),this,SLOT(shoot()));
    shootTimer->start(interval);
}


void Tankes::shoot()
{
    if(!mIsAlive)
        return;
    int rt=rotation();
    YourBullet *bullet=new YourBullet();
    bullet->setSpeed(mBulletSpeed);
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

void Tankes::setAlive(bool alive)
{
    mIsAlive=alive;
}



void Tankes::moving()
{
    if(!mIsAlive)
        return;
    int index=qrand()%4;
    qreal direction[4]={mSrcRotation,mSrcRotation+180,mSrcRotation+90,mSrcRotation-90};
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
    if(num>0)
    {
        foreach(QGraphicsItem *item,list)
        {
            if(item->boundingRect().width()==38&&item->boundingRect().height()==38)
               {
                Tankes *tankes=(Tankes*)item;
                if(!tankes->isAlive())
                    continue;
                tankes->setAlive(false);
                QGraphicsBlurEffect *blur=new QGraphicsBlurEffect;
                tankes->setGraphicsEffect(blur);
                QPropertyAnimation *animation=new QPropertyAnimation(tankes,"opacity");
                animation->setStartValue(1.0);
                animation->setEndValue(0.0);
                animation->setDuration(1000);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation,SIGNAL(finished()),tankes,SLOT(slotDestroy()));

             }else if(item->boundingRect().width()==36&&item->boundingRect().height()==36)
            {
                num--;
            }else if(item->boundingRect().width()==49&&item->boundingRect().height()==35)
            {
                item->hide();
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
    if(num>0)
    {
        foreach(QGraphicsItem *item,list)
        {
            //检测到碰撞了我方坦克，并设置我方坦克死亡
            if(item->boundingRect().width()==36&&item->boundingRect().height()==36)
            {
                Tanke *tanke=(Tanke*)item;
                if(!tanke->isAlive())
                   continue;
                tanke->setAlive(false);
                QPropertyAnimation *animation=new QPropertyAnimation(tanke,"rotation");
                animation->setStartValue(0);
                animation->setEndValue(1080);
                animation->setDuration(400);
                animation->start(QAbstractAnimation::DeleteWhenStopped);
                connect(animation,SIGNAL(finished()),tanke,SLOT(toHide()));
            }else if(item->boundingRect().width()==38&&item->boundingRect().height()==38)
            {
                //检测到碰撞了敌方自己的坦克，num--
                num--;
            }else if(item->boundingRect().width()==49&&item->boundingRect().height()==35)
            {

                item->hide();
            }
        }
    }
    //num代表了碰撞的个数
    if(num>0)
        return true;
    else
      return false;
}

//判断敌方坦克是否存活
bool Tankes::isAlive()
{
    return mIsAlive;
}
//发射死亡信号并销毁自己
void Tankes::slotDestroy()
{
   QList<QGraphicsItem*>list=scene()->itemAt(450,-295)->childItems();
   foreach(QGraphicsItem*item,list)
   {
       if(item->isVisible())
       {
           item->hide();
           break;
       }
   }

   deleteLater();
}

//我的子弹大小
QRectF MyBullet::boundingRect() const
{
    return QRectF(-5,-2,10,4);
}
//敌方子弹大小
QRectF YourBullet::boundingRect() const
{
    return QRectF(-5,-2,11,5);
}
