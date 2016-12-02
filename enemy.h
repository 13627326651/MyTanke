#ifndef ENEMY_H
#define ENEMY_H

#include <phonon>
#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
//敌人的坦克
class Tankes : public QGraphicsObject
{
    Q_OBJECT
public:
    enum BulletType{Standard,RedSord};
    explicit Tankes(int step=5, int duration=50,
                    int interval=1000, int bulletSpeed=100);
    //设置边界矩形
    QRectF boundingRect() const{return QRectF(-19,-19,38,38);}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/enemy1U.gif"));
        painter->drawPixmap(-19,-19,38,38,pixmap);
    }
    void setLiveVal(int val){ mLiveValue=val;}
    bool maybeCollide(QPointF);
    bool isAlive(){return !mPauseMove;}
    void pauseMove(){mPauseMove=true;}
    void resumeMove(){mPauseMove=false;}
    void pauseShoot(){mPauseShoot=true;}
    void resumeShoot(){mPauseShoot=false;}
    void setBulletType(BulletType type){mBulletType=type;}
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
    int mLiveValue;
    Phonon::MediaObject *shootOverSound;
    Phonon::MediaObject *beShootSound;
    QPropertyAnimation *mAniBeShoot;
    QPropertyAnimation *mAniMoving;
    QGraphicsPixmapItem *blastPix;
    BulletType mBulletType;
};

class Enemy2 :public Tankes
{
public:
    explicit Enemy2(int step=5, int duration=50,
                    int interval=1000, int bulletSpeed=100)
        :Tankes(step,duration,interval,bulletSpeed){
         setLiveVal(2);
    };
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/enemy2U.gif"));
        painter->drawPixmap(-19,-19,38,38,pixmap);
    }

};


class Enemy3 :public Tankes
{
public:
    explicit Enemy3(int step=5, int duration=50,
                    int interval=1000, int bulletSpeed=100)
        :Tankes(step,duration,interval,bulletSpeed){
        setLiveVal(3);
    };
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/enemy3U.gif"));
        painter->drawPixmap(-19,-19,38,38,pixmap);
    }
};
#endif // ENEMY_H
