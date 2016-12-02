#ifndef BULLET_H
#define BULLET_H

#include <phonon>
#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>

//标准子弹，可以根据需要重写paint()，boundingRect(),bulletShoot()函数实现不同的子弹
class Bullet : public QGraphicsObject
{
    Q_OBJECT
public:
    enum User{Enemy,Friend};
    enum Direction{UP,DOWN,LEFT,RIGHT};
    explicit Bullet(int speed=100): mSpeed(speed){
        animation=new QPropertyAnimation(this,"pos");
        connect(animation,SIGNAL(finished()),this,SLOT(bulletShoot()));
    }
    QRectF boundingRect() const{
            return QRectF(-5,-2,10,4);
        }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/bullet.png"));
        painter->drawPixmap(-5,-2,10,4,pixmap);
    }
    bool isColliding();
    void setDirection(Direction direction){
        mDirection=direction;
    }
    //设置子弹速度
    void setSpeed(const qreal speed){
        mSpeed=speed;
    }
    void setUser(User user){
        mUser=user;
    }
public slots:
     virtual void bulletShoot();
protected:
    qreal mSpeed;
    QTimer *mTimer;
    Direction mDirection;
    QPointF mCurrentPos;
    QPropertyAnimation *animation;
    User mUser;
};


//红色激光
class RedSordBullet : public Bullet
{
public:
    QRectF boundingRect() const{
        return QRectF(0,0,800,2);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        painter->setPen(QPen(Qt::white));
        painter->drawLine(0,0,800,0);
    }
    void bulletShoot();
};



#endif // BULLET_H
