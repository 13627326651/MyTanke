#ifndef TANKE_H
#define TANKE_H

#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
//�ҵ�̹��
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



//���˵�̹��
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





//�ӵ�
class Bullet : public QGraphicsObject
{
    Q_OBJECT
public:
    //������Ϣ
    enum Direction{UP,DOWN,LEFT,RIGHT};
    explicit Bullet();
    //���÷���
    void setDirection(Direction direction);
    //�����ٶ�
    void setSpeed(const qreal speed);
    //��ʼ�����ӵ�
    void bulletShoot();
    //�Ƿ�����ײ
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
