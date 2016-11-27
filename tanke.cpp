#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

#define qd qDebug()<<
//我的坦克

Tanke::Tanke()
{
    mSrcRotation=rotation();
    setFlag(QGraphicsItem::ItemIsFocusable);
    QTimer *checkTimer=new QTimer(this);
    connect(checkTimer,SIGNAL(timeout()),this,SLOT(checkHurt()));
    checkTimer->start(50);
}

QRectF Tanke::boundingRect() const
{
    return QRectF(-16,-20,32,40);
}

void Tanke::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/tanke.png"));
    painter->drawPixmap(-15,-19,30,38,pixmap);
}


void Tanke::shoot()
{
    int rt=rotation();
    MyBullet *bullet=new MyBullet;
    bullet->setSpeed(50);
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()-30);
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()+30);

    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()-30,pos().ry());

    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()+30,pos().ry());
    }
    bullet->bulletShoot();

}

Tanke::CollideType Tanke::isColliding()
{
    QList<QGraphicsItem*>list=collidingItems();
     if(list.count()>0)
     {
         foreach(QGraphicsItem *item,list)
         {
             if(item->boundingRect().width()==10&&item->boundingRect().height()==4)
                return Tanke::BULLET;
             else
                 return Tanke::WALL;
         }
     }
     return Tanke::NOTHING;
}

void Tanke::moveLength(int length, int step,int key)
{
    for(int i=0;i<length/step;i++)
    {
        switch(key)
        {
        case Qt::Key_Up:
            setRotation(mSrcRotation);
            moveBy(0,0-step);
            if(isColliding()==Tanke::WALL)
            {
                moveBy(0,step);
                break;
            }
            break;
        case Qt::Key_Down:
            setRotation(mSrcRotation+180);
            moveBy(0,step);
            if(isColliding()==Tanke::WALL)
            {
                moveBy(0,0-step);
                break;
            }
            break;
        case Qt::Key_Left:
            setRotation(mSrcRotation-90);
            moveBy(0-step,0);
            if(isColliding()==Tanke::WALL)
            {
                moveBy(step,0);
                break;
            }
            break;
        case Qt::Key_Right:
            setRotation(mSrcRotation+90);
            moveBy(step,0);
            if(isColliding()==Tanke::WALL)
            {
                moveBy(0-step,0);
                break;
            }
            break;
        default:
            break;
        }
    }
}

void Tanke::checkHurt()
{
    if(isColliding()==Tanke::BULLET)
        hide();
}

void Tanke::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat()&&event->key()==Qt::Key_Space)
        return;
    if(event->key()==Qt::Key_Space)
        shoot();
    else
        moveLength(15,3,event->key());


}


//子弹
Bullet::Bullet()
{
    mTimer=new QTimer(this);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(timeout()));
}

void Bullet::setDirection(Bullet::Direction direction)
{
    mDirection=direction;
}

QRectF Bullet::boundingRect() const
{
    return QRectF(-5,-2,10,4);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/bullet.png"));
    painter->drawPixmap(-5,-2,10,4,pixmap);
}


void Bullet::bulletShoot()
{
        switch(mDirection)
        {
        case UP:
            rotate(-90);
            break;
        case DOWN:
            rotate(90);
            break;
        case LEFT:
            rotate(180);
            break;
        }
        mTimer->start(mSpeed);
}

bool Bullet::isColliding()
{
//     QList<QGraphicsItem*>list=collidingItems();
//     if(list.count()>0)
//     {
//         foreach(QGraphicsItem *item,list)
//         {
//             if(item->boundingRect().width()==30&&item->boundingRect().height()==38)
//                    return true;
//         }
//     }
//   return false;
     QList<QGraphicsItem*>list=collidingItems();
     if(list.count()>0)
         return true;
     else
         return false;
}

void Bullet::setSpeed(const qreal speed)
{
    mSpeed=speed;
}


//设置子弹移动的距离和消失的时间，可以根据子弹的消失长短体现子弹的威力，比如100时，一个子弹可以
//消灭紧挨着的两个敌人
void Bullet::timeout()
{
    switch(mDirection)
    {
    case UP:
         moveBy(0,-15);
         if(isColliding())
             QTimer::singleShot(50,this,SLOT(deleteLater()));
         //hide();

        break;
    case DOWN:
        moveBy(0,15);
        if(isColliding())
           QTimer::singleShot(50,this,SLOT(deleteLater()));
        //hide();
        break;
    case LEFT:
        moveBy(-15,0);
        if(isColliding())
           QTimer::singleShot(50,this,SLOT(deleteLater()));
        //hide();
        break;
    case RIGHT:
        moveBy(15,0);
        if(isColliding())
           QTimer::singleShot(50,this,SLOT(deleteLater()));
        //hide();
        break;
    }
}



//敌人的坦克
QRectF Tankes::boundingRect() const
{
    return QRectF(-15,-19,30,38);
}

void Tankes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/tanke.png"));
    painter->drawPixmap(-15,-19,30,38,pixmap);
}


Tankes::Tankes()
{
    mSrcRotation=rotation();
    QTimer *shootTimer=new QTimer(this);
    QTimer *checkTimer=new QTimer(this);
    connect(shootTimer,SIGNAL(timeout()),this,SLOT(shoot()));
    connect(checkTimer,SIGNAL(timeout()),this,SLOT(checkHurt()));
    shootTimer->start(1000);
    checkTimer->start(50);
    moving();
}


void Tankes::shoot()
{
    int rt=rotation();
    Bullet *bullet=new Bullet();
    bullet->setSpeed(100);
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()-30);
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()+30);

    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()-30,pos().ry());

    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()+30,pos().ry());
    }
    bullet->bulletShoot();
}

Tankes::CollideType Tankes::isColliding()
{
   CollideType type=Tankes::NOTHING;

   QList<QGraphicsItem*>list=collidingItems();
    if(list.count()>0)
    {
        foreach(QGraphicsItem *item,list)
        {
            if(item->boundingRect().width()==11&&item->boundingRect().height()==5)
            {
                qd item->boundingRect().width();
               type=Tankes::BULLET;
            }
            else if(item->boundingRect().width()==30&&item->boundingRect().height()==38)
                type=Tankes::NOTHING;
            else
                type=Tankes::WALL;
            if(type==Tankes::WALL||type==Tankes::BULLET)
                return type;
        }
    }
    return type;
}

void Tankes::moving()
{
    qreal rt=rotation();
    if(rt==mSrcRotation)           //up
    {
        moveBy(0,-10);
        if(isColliding()==Tankes::WALL)
        {
            moveBy(0,10);
            int temp=qrand()%2;
            if(temp)
                setRotation(mSrcRotation+90);
            else
                setRotation(mSrcRotation-90);
        }
        QTimer::singleShot(100,this,SLOT(moving()));
    }else if(rt==mSrcRotation+180)      //down
    {
        moveBy(0,10);
        if(isColliding()==Tankes::WALL)
        {
            moveBy(0,-10);
            int temp=qrand()%2;
            if(temp)
                setRotation(mSrcRotation+90);
            else
                setRotation(mSrcRotation-90);
        }
        QTimer::singleShot(100,this,SLOT(moving()));
    }else if(rt==mSrcRotation-90)           //left
    {
        moveBy(-10,0);
        if(isColliding()==Tankes::WALL)
        {
            moveBy(10,0);
            int temp=qrand()%2;
            if(temp)
                setRotation(mSrcRotation);
            else
                setRotation(mSrcRotation+180);
        }
        QTimer::singleShot(100,this,SLOT(moving()));
    }else if(rt==mSrcRotation+90)           //right
    {
        moveBy(10,0);
        if(isColliding()==Tankes::WALL)
        {
            moveBy(-10,0);
            int temp=qrand()%2;
            if(temp)
                setRotation(mSrcRotation);
            else
                setRotation(mSrcRotation+180);
        }
        QTimer::singleShot(100,this,SLOT(moving()));
    }
}

void Tankes::checkHurt()
{
    if(isColliding()==Tankes::BULLET)
        deleteLater();
}


QRectF MyBullet::boundingRect() const
{
    return QRectF(-5,-2,11,5);
}
