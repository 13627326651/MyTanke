#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

#define qd qDebug()<<

Tanke::Tanke()
{
    mSrcRotation=rotation();
    setFlag(QGraphicsItem::ItemIsFocusable);
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
    //bullet->setPos(pos());
    bullet->setSpeed(50);
    if(rt==mSrcRotation)
    {
        //Bullet *bullet1=new Bullet(100,Bullet::UP,pos());
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos().rx(),pos().ry()-25);
        //bullet1->setPos(pos());
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
       // bullet->setPos(pos().rx(),pos().ry()+80);
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
//    scene()->addItem(bullet);
//    bullet->setPos(pos());
    bullet->bulletShoot();
//    switch(rotation())
//    {
//    case mSrcRotation:
//        Bullet *bullet1=new Bullet(100,Bullet::UP,pos());
//        scene()->addItem(bullet1);
//        bullet1->setPos(pos());
//        break;
//    case mSrcRotation+180:
//        Bullet *bullet2=new Bullet(100,Bullet::DOWN,pos());
//        scene()->addItem(bullet2);
//        bullet2->setPos(pos());
//        break;
//    case mSrcRotation-90:
//        Bullet *bullet3=new Bullet(100,Bullet::LEFT,pos());
//        scene()->addItem(bullet3);
//        bullet3->setPos(pos());
//        break;
//    case mSrcRotation+90:
//        Bullet *bullet4=new Bullet(100,Bullet::RIGHT,pos());
//        scene()->addItem(bullet4);
//        bullet4->setPos(pos());
//        break;
//    }
    //bullet->moveBullet();
    //bullet->setPos(this->pos());
}

void Tanke::keyPressEvent(QKeyEvent *event)
{
    qd event->key();
    switch(event->key())
    {
    case Qt::Key_Up:
        setRotation(mSrcRotation);
        moveBy(0,-10);
        break;
    case Qt::Key_Down:
        setRotation(mSrcRotation+180);
        moveBy(0,10);
        break;
    case Qt::Key_Left:
        setRotation(mSrcRotation-90);
        moveBy(-10,0);
        break;
    case Qt::Key_Right:
        setRotation(mSrcRotation+90);
        moveBy(10,0);
        break;
    case Qt::Key_Space:
        shoot();
        break;
    default:
        break;
    }
}



Bullet::Bullet()
{
//    mSpeed=speed;
  //  rotate(-90);
//    mDirection=direction;
//    switch(mDirection)
//    {
//    case UP:
//        rotate(-90);
//        break;
//    case DOWN:
//        rotate(90);
//        break;
//    case LEFT:
//        rotate(180);
//        break;
//    }
    mTimer=new QTimer;
    connect(mTimer,SIGNAL(timeout()),this,SLOT(timeout()));
    //    mTimer->start(mSpeed);
}

void Bullet::setDirection(Bullet::Direction direction)
{
    mDirection=direction;
}

QRectF Bullet::boundingRect() const
{
    qreal penWidth=1;
    return QRectF(-5-penWidth/2,-2-penWidth,10+penWidth,4+penWidth);
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
}

void Bullet::bulletShoot()
{
    mCurrentPos=pos();
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
    QList<QGraphicsItem*>list=collidingItems();
    if(list.count()>0)
        return true;
    else return false;
}

void Bullet::setSpeed(const qreal speed)
{
    mSpeed=speed;
}

//void Bullet::moveBullet()
//{
//    mTimer->start(mSpeed);
//}

void Bullet::timeout()
{
    qreal x=mCurrentPos.rx();
    qreal y=mCurrentPos.ry();
    switch(mDirection)
    {
    case UP:
        //setPos(mCurrentPos.rx+mCurrentPos.ry+20);
        //mCurrentPos.setY(mCurrentPos.ry()+20);
        setPos(x,y-20);
        if(isColliding())
            deleteLater();
        mCurrentPos.setY(y-20);
        break;
    case DOWN:
        //setPos(0,currentPos);
        setPos(x,y+20);
        if(isColliding())
            deleteLater();
        mCurrentPos.setY(y+20);
        break;
    case LEFT:
        //setPos(-currentPos,0);
        setPos(x-20,y);
        if(isColliding())
            deleteLater();
        mCurrentPos.setX(x-20);
        break;
    case RIGHT:
        //setPos(currentPos,0);
        setPos(x+20,y);
        if(isColliding())
            deleteLater();
        mCurrentPos.setX(x+20);
        break;
    }
    //moveBy(0,-20);
//    setPos(0,-currentPos);
//    qd pos();
}
