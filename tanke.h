#ifndef TANKE_H
#define TANKE_H

#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
//我的坦克
class Tanke : public QGraphicsObject
{
    Q_OBJECT
public:
    enum CollideType{WALL,BULLET,NOTHING};
    explicit Tanke();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void shoot();
    CollideType isColliding();
    void moveLength(int length, int step, int key);
public slots:
    void checkHurt();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    qreal mSrcRotation;
};



//敌人的坦克
class Tankes : public QGraphicsObject
{
    Q_OBJECT
public:
    enum CollideType{WALL,BULLET,NOTHING};
    explicit Tankes();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    CollideType isColliding();

public slots:
    void moving();
    void checkHurt();
    void shoot();
//protected:
   // void keyPressEvent(QKeyEvent *event);
private:
    bool isHurt;
    qreal mSrcRotation;
};





//子弹
class Bullet : public QGraphicsObject
{
    Q_OBJECT
public:
    //方向信息
    enum Direction{UP,DOWN,LEFT,RIGHT};
    explicit Bullet();
    //设置方向
    void setDirection(Direction direction);
    //设置速度
    void setSpeed(const qreal speed);
    //开始发射子弹
    void bulletShoot();
    //是否发生碰撞
    bool isColliding();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
    void timeout();
protected:
    qreal mSpeed;
    QTimer *mTimer;
    Direction mDirection;
    QPointF mCurrentPos;
};


class MyBullet :public Bullet
{
    Q_OBJECT
public :
    QRectF boundingRect() const;
};

#endif // TANKE_H
