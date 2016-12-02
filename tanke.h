#ifndef TANKE_H
#define TANKE_H

#include <phonon>
#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>
//ÎÒµÄÌ¹¿Ë
class Tanke : public QGraphicsObject
{
    Q_OBJECT
public:
    enum BulletType{Standard,RedSord};
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
    void setBulletType(BulletType type){
        mBulletType=type;
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
    BulletType mBulletType;
};

#endif // TANKE_H

