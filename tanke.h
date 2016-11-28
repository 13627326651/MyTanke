#ifndef TANKE_H
#define TANKE_H

#include <phonon>
#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
//我的坦克
class Tanke : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Tanke(int step=20, int duration=100, int bulletSpeed=100);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void shoot();
    void moveLength(int length, int step, int key);
    void setAlive(bool);
    bool isAlive();
public slots:
    bool maybeCollide(QPointF);
    void toHide();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    int mStep;
    int mDuration;
    int mSpeed;
    bool mIsAlive;
    qreal mSrcRotation;
    Phonon::MediaObject *shootSound;
};



//敌人的坦克
class Tankes : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Tankes(int step=5, int duration=50, int interval=1000, int bulletSpeed=100);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool maybeCollide(QPointF);
    void setAlive(bool);
    bool isAlive();
public slots:
    void moving();
    void shoot();
private:
    bool isHurt;
    qreal mSrcRotation;
    bool mIsAlive;
    int step;
    int duration;
    int mBulletSpeed;

};





//子弹
class Bullet : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Direction{UP,DOWN,LEFT,RIGHT};
    explicit Bullet(int speed=100);
    void setDirection(Direction direction);
    void setSpeed(const qreal speed);
    virtual bool isColliding()=0;
//    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
     void bulletShoot();
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
    bool isColliding();
};

class YourBullet :public Bullet
{
    Q_OBJECT
public:
    QRectF boundingRect() const;
    bool isColliding();
};

#endif // TANKE_H
