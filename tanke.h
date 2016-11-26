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



//���˵�̹��
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
