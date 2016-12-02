#ifndef TANKE_H
#define TANKE_H

#include <phonon>
#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
//�ҵ�̹��
class Tanke : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Tanke(int step=5, int duration=50, int bulletSpeed=100);
    QRectF boundingRect() const{
        return QRectF(-18,-18,36,36);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/tanke.gif"));
        painter->drawPixmap(-18,-18,36,36,pixmap);
    }
    void shoot();
    bool isAlive(){
        return !mPause;
    }
    void pause(){mPause=true;}
    void resume(){mPause=false;}
    void setBeginSpeed(int step,int duration){
        beginDuration=duration;
        beginStep=step;
    }

signals:
    void sgDestroy();
public slots:
    bool maybeCollide(QPointF);
    void slotDestroy();
    void slotBeShoot();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    bool mPause;
    int mStep;
    int mDuration;
    int mSpeed;
    qreal mSrcRotation;
    Phonon::MediaObject *shootSound;
    Phonon::MediaObject *shootOverSound;
    QPropertyAnimation *mAniBeShoot;
    int beginDuration;
    int beginStep;
};


//���˵�̹��
class Tankes : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Tankes(int step=5, int duration=50,
                    int interval=1000, int bulletSpeed=100);
    //���ñ߽����
    QRectF boundingRect() const{return QRectF(-19,-19,38,38);}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/enemy.gif"));
        painter->drawPixmap(-19,-19,38,38,pixmap);
    }

    bool maybeCollide(QPointF);
    bool isAlive(){return !mPauseMove;}
    void pauseMove(){mPauseMove=true;}
    void resumeMove(){mPauseMove=false;}
    void pauseShoot(){mPauseShoot=true;}
    void resumeShoot(){mPauseShoot=false;}
public slots:
    void slotDestroy();
signals:
    void sgDestroy();
public slots:
    void moving();
    void shoot();
    void slotBeShoot();
private:
    bool mPauseMove;
    bool mPauseShoot;
    qreal mSrcRotation;
    int step;
    int duration;
    int mBulletSpeed;
    Phonon::MediaObject *shootOverSound;
    Phonon::MediaObject *beShootSound;
    QPropertyAnimation *mAniBeShoot;
    QPropertyAnimation *mAniMoving;

};


//�ӵ�
class Bullet : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Direction{UP,DOWN,LEFT,RIGHT};
    explicit Bullet(int speed=100): mSpeed(speed){
        animation=new QPropertyAnimation(this,"pos");
        connect(animation,SIGNAL(finished()),this,SLOT(bulletShoot()));
    }
    void setDirection(Direction direction){
        mDirection=direction;
    }
    //�����ӵ��ٶ�
    void setSpeed(const qreal speed){
        mSpeed=speed;
    }
    virtual bool isColliding()=0;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/bullet.png"));
        painter->drawPixmap(-5,-2,10,4,pixmap);
    }
public slots:
     void bulletShoot();
protected:
    qreal mSpeed;
    QTimer *mTimer;
    Direction mDirection;
    QPointF mCurrentPos;
    QPropertyAnimation *animation;
};


class MyBullet :public Bullet
{
    Q_OBJECT
public :
    QRectF boundingRect() const{return QRectF(-5,-2,10,4);}
    bool isColliding();
};

class YourBullet :public Bullet
{
    Q_OBJECT
public:
    QRectF boundingRect() const{ return QRectF(-5,-2,11,5);}
    bool isColliding();
};

#endif // TANKE_H
