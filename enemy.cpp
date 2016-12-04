#include "enemy.h"
#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include "mywall.h"
#include <QDebug>
#include "bullet.h"

#define SOURCEPATH tr("../MyTanke/music/")

#define qd qDebug()<<
Tankes::Tankes(int step,int duration,int interval,int bulletSpeed)
{
    mPauseMove=false;
    mPauseShoot=false;
    this->step=step;
    this->duration=duration;
    mBulletSpeed=bulletSpeed;
    mSrcRotation=rotation();
    //默认子弹类型
    mBulletType=Standard;
    //生命力
    mLiveValue=1;
    //坦克初始化后就可以射击，可以通过暂停函数停止射击
    QTimer *shootTimer=new QTimer(this);
    connect(shootTimer,SIGNAL(timeout()),this,SLOT(shoot()));
    shootTimer->start(interval);
    //被击中音效
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("blast.wav")));
    Phonon::AudioOutput *shootOverOutput=new Phonon::AudioOutput(Phonon::VideoCategory,this);
    Phonon::createPath(shootOverSound,shootOverOutput);

    blastPix=new QGraphicsPixmapItem();
    blastPix->setPixmap(QPixmap(tr(":/images/blast7min.gif")));//不要用blast6min.gof
  //  scene()->addItem(blastPix);
    blastPix->setParentItem(this);
    blastPix->setPos(-30,-23);
    blastPix->hide();
    //被击中动画
//    mAniBeShoot=new QPropertyAnimation(this,"opacity");
//    connect(mAniBeShoot,SIGNAL(finished()),this,SLOT(slotDestroy()));
    //移动动画
    mAniMoving=new QPropertyAnimation(this,"pos");
    connect(mAniMoving,SIGNAL(finished()),this,SLOT(moving()));
}


void Tankes::shoot()
{
    if(mPauseShoot)
        return;
    Bullet *bullet;
    switch(mBulletType)
    {
    case Standard:
        bullet=new Bullet;
        bullet->setSpeed(mBulletSpeed);
        break;
    case RedSord:
        bullet=new RedSordBullet;
    }
    bullet->setUser(Bullet::Enemy);

    int rt=rotation();
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos()+QPointF(0,-20));
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos()+QPointF(0,20));
    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos()+QPointF(-20,0));
    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos()+QPointF(20,0));
    }
    bullet->bulletShoot();
}

void Tankes::slotBeShoot()
{
    mLiveValue--;
    if(mLiveValue>0)
        return;
    pauseMove();
    pauseShoot();
    shootOverSound->play();

    blastPix->show();
    QTimer::singleShot(500,this,SLOT(slotDestroy()));
}


bool Tankes::maybeCollide(QPointF endPos)
{
    QRectF rectF(boundingRect());
    QGraphicsRectItem *item=scene()->addRect(rectF);//item一定要记得释放内存
    item->hide();
    item->setPos(endPos);
    QList<QGraphicsItem*>list=item->collidingItems();
    int num=0;
     if(list.count()>0)
     {
         foreach(QGraphicsItem *items,list)
         {
             qreal x=items->boundingRect().width();
             qreal y=items->boundingRect().height();
             if(x==36&&y==36)
                  num++;
             if((x==900&&y==50)||(x==50&&y==700))
                 num++;
             if(x==25&&y==25||x==50&&y==50)
             {
                 MyWall *wall=(MyWall*)items;
                 if(wall->getShape()==MyWall::GreenGrass)
                    wall->setZValue(1);
                 num++;
             }
         }
     }
    scene()->removeItem(item);
    delete item;
    if(num>0)
        return true;
    else
        return false;
}


static const int turnCount=20;
static int record=0;
void Tankes::moving()
{
    if(mPauseMove)
        return;
    int index=qrand()%4;
    qreal direction[4]={mSrcRotation,mSrcRotation+180,mSrcRotation+90,mSrcRotation-90};
    //移动turnCount次进行转向
    if(record>turnCount)
    {
        setRotation(direction[index]);
        record=0;
    }
    record++;
    mAniMoving->setDuration(duration);
    mAniMoving->setStartValue(pos());

    qreal rt=rotation();
    if(rt==mSrcRotation)           //up
    {
        if(maybeCollide(pos()+QPointF(0,0-step)))           //可能存在内存泄漏
        {
           setRotation(direction[index]);
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            mAniMoving->setEndValue(pos()+QPointF(0,0-step));
            mAniMoving->start();
        }
    }else if(rt==mSrcRotation+180)      //down
    {
        if(maybeCollide(pos()+QPointF(0,step)))
        {
          setRotation(direction[index]);
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            mAniMoving->setEndValue(pos()+QPointF(0,step));
            mAniMoving->start();
        }
    }else if(rt==mSrcRotation-90)           //left
    {

        if(maybeCollide(pos()+QPointF(0-step,0)))
        {
           setRotation(direction[index]);
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            mAniMoving->setEndValue(pos()+QPointF(0-step,0));
            mAniMoving->start();
        }
    }else if(rt==mSrcRotation+90)           //right
    {
        if(maybeCollide(pos()+QPointF(step,0)))
        {
           setRotation(direction[index]);
           QTimer::singleShot(10,this,SLOT(moving()));
        }else
        {
            mAniMoving->setEndValue(pos()+QPointF(step,0));
            mAniMoving->start();
        }
    }
}

//发射死亡信号并销毁自己
void Tankes::slotDestroy()
{
    emit sgDestroy();
    deleteLater();
}
