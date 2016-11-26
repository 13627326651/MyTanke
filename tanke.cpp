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
    QTimer *check=new QTimer(this);
    connect(check,SIGNAL(timeout()),this,SLOT(checkHurt()));
    check->start(50);
}

QRectF Tanke::boundingRect() const
{
    qreal penWidth=2;
    return QRectF(-15-penWidth/2,-19-penWidth/2,30+penWidth,38+penWidth);
}

void Tanke::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap(tr(":/images/tanke.png"));
    painter->drawPixmap(-15,-19,30,38,pixmap);
}

QPainterPath Tanke::shape() const
{
    QPainterPath path;
    path.addRect(-15,-18,30,38);
    return path;
}

void Tanke::shoot()
{
    int rt=rotation();
    Bullet *bullet=new Bullet();
    bullet->setSpeed(50);
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()-25);
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()+25);

    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()-25,pos().ry());

    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()+25,pos().ry());
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

void Tanke::checkHurt()
{
    if(isColliding()==Tanke::BULLET)
        deleteLater();
}

void Tanke::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        setRotation(mSrcRotation);
        moveBy(0,-10);
        if(isColliding()==Tanke::WALL)
            moveBy(0,10);
        break;
    case Qt::Key_Down:
        setRotation(mSrcRotation+180);
        moveBy(0,10);
        if(isColliding()==Tanke::WALL)
            moveBy(0,-10);
        break;
    case Qt::Key_Left:
        setRotation(mSrcRotation-90);
        moveBy(-10,0);
        if(isColliding()==Tanke::WALL)
            moveBy(10,0);
        break;
    case Qt::Key_Right:
        setRotation(mSrcRotation+90);
        moveBy(10,0);
        if(isColliding()==Tanke::WALL)
            moveBy(-10,0);
        break;
    case Qt::Key_Space:
        shoot();
        break;
    default:
        break;
    }
}


//子弹
Bullet::Bullet()
{
    mTimer=new QTimer;
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

QPainterPath Bullet::shape() const
{
    QPainterPath path;
    path.addRect(-5,-2,10,4);
    return path;
//        QPainterPath path;
//        path.addRect(-2,-2,4,4);
//        return path;
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

void Bullet::timeout()
{
    switch(mDirection)
    {
    case UP:
         moveBy(0,-20);
         if(isColliding())
             QTimer::singleShot(40,this,SLOT(deleteLater()));
        break;
    case DOWN:
        moveBy(0,20);
        if(isColliding())
           QTimer::singleShot(40,this,SLOT(deleteLater()));
        break;
    case LEFT:
        moveBy(-20,0);
        if(isColliding())
           QTimer::singleShot(40,this,SLOT(deleteLater()));
        break;
    case RIGHT:
        moveBy(20,0);
        if(isColliding())
           QTimer::singleShot(40,this,SLOT(deleteLater()));
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
    isHurt=false;
    mSrcRotation=rotation();
    //QTimer *timer=new QTimer(this);
    QTimer *checkTimer=new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(moving()));
    connect(checkTimer,SIGNAL(timeout()),this,SLOT(checkHurt()));
    //timer->start(100);
    moving();
    checkTimer->start(50);
}


QPainterPath Tankes::shape() const
{
    QPainterPath path;
    path.addRect(-15,-18,30,38);
    return path;
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
        bullet->setPos(pos().rx(),pos().ry()-25);
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()+25);

    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()-25,pos().ry());

    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx()+25,pos().ry());
    }
    bullet->bulletShoot();
}

Tankes::CollideType Tankes::isColliding()
{
        QList<QGraphicsItem*>list=collidingItems();
         if(list.count()>0)
         {
             foreach(QGraphicsItem *item,list)
             {
                 if(item->boundingRect().width()==10&&item->boundingRect().height()==4)
                    return Tankes::BULLET;
                 else
                    return Tankes::WALL;
             }
         }
         return Tankes::NOTHING;
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
