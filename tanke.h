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
    explicit Tanke();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void shoot();
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
    explicit Tankes();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void shoot();
    bool isColliding();
public slots:
    void moving();
    void checkHurt();
//protected:
   // void keyPressEvent(QKeyEvent *event);
private:
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
    QPainterPath shape() const;    
public slots:
    void timeout();
private:
    qreal mSpeed;
    QTimer *mTimer;
    Direction mDirection;
    QPointF mCurrentPos;
};

#endif // TANKE_H
