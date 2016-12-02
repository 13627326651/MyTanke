#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QTime>
#include <QKeyEvent>
#include "mywall.h"
#include <QDebug>

#define SOURCEPATH tr("../MyTanke/music/")

#define qd qDebug()<<
//我的坦克

Tanke::Tanke(int step, int duration,int bulletSpeed)
{
    mPause=false;
    mStep=step;
    mDuration=duration;
    mSpeed=bulletSpeed;
    beginStep=30;
    beginDuration=450;
    mSrcRotation=rotation();
    setFlag(QGraphicsItem::ItemIsFocusable);
    //射击音效
    shootSound=new Phonon::MediaObject(this);
    connect(shootSound,SIGNAL(finished()),shootSound,SLOT(stop()));
    shootSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("startshoot.mp3")));
    Phonon::AudioOutput *shootOutput=new Phonon::AudioOutput(Phonon::VideoCategory,this);
    Phonon::createPath(shootSound,shootOutput);
    //被射中音效
    shootOverSound=new Phonon::MediaObject(this);
    connect(shootOverSound,SIGNAL(finished()),shootOverSound,SLOT(stop()));
    shootOverSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("lose.mp3")));
    Phonon::AudioOutput *shootOverOutput=new Phonon::AudioOutput(Phonon::VideoCategory,this);
    Phonon::createPath(shootOverSound,shootOverOutput);
    //设置被击中的动画
    mAniBeShoot=new QPropertyAnimation(this,"rotation");
    connect(mAniBeShoot,SIGNAL(finished()),this,SLOT(slotDestroy()));
    //移动动画

}

void Tanke::shoot()
{
    int rt=rotation();
    RedSord *bullet=new RedSord;
    bullet->setSpeed(mSpeed);
    if(rt==mSrcRotation){
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }else if(rt==mSrcRotation+180){
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }else if(rt==mSrcRotation-90){
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }else if(rt==mSrcRotation+90){
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        bullet->setPos(pos());
    }
    bullet->bulletShoot();
}



bool Tanke::maybeCollide(QPointF endPos)
{
    QRectF rectF(boundingRect());
    QGraphicsRectItem *item=scene()->addRect(rectF);
    item->hide();
    item->setPos(endPos);
    QList<QGraphicsItem*>list=item->collidingItems();
    int num=list.count();
    if(list.count()>0) {
         foreach(QGraphicsItem *items,list){
             qreal x=items->boundingRect().width();
             qreal y=items->boundingRect().height();
             if(x==36&&y==36)
                  num--;
             if(x==10&&y==4)
                  num--;
             if(x==25&&y==25||x==50&&y==50)
             {
                 MyWall *wall=(MyWall*)items;
                 if(wall->getShape()==MyWall::GreenGrass)
                 {
                     wall->setZValue(1);
                     num--;
                 }
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

void Tanke::slotDestroy()
{   
    emit sgDestroy();
    hide();
}

void Tanke::slotBeShoot()
{
   pause();
   shootOverSound->play();

   mAniBeShoot->setStartValue(0);
   mAniBeShoot->setEndValue(1080);
   mAniBeShoot->setDuration(400);
   mAniBeShoot->start();
}

void Tanke::keyPressEvent(QKeyEvent *event)
{
    if(mPause)
        return;
    if(event->isAutoRepeat()&&event->key()==Qt::Key_Space)
        return;
    if(event->key()==Qt::Key_Space)
    {
        shootSound->play();
        shoot();
        return;
    }
    QPropertyAnimation *mAniMoving=new QPropertyAnimation(this,"pos");

    int step=mStep;
    mAniMoving->setStartValue(pos());
    mAniMoving->setDuration(mDuration);
    if(!event->isAutoRepeat())
    {
        step=beginStep;
        mAniMoving->setDuration(beginDuration);
    }
    QPointF temp;
    switch(event->key())
    {
    case Qt::Key_Up:
        temp=pos()+QPointF(0,0-step);
        setRotation(mSrcRotation);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    case Qt::Key_Down:
        temp=pos()+QPointF(0,step);
        setRotation(mSrcRotation+180);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    case Qt::Key_Left:
        temp=pos()+QPointF(0-step,0);
        setRotation(mSrcRotation-90);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    case Qt::Key_Right:
        temp=pos()+QPointF(step,0);
        setRotation(mSrcRotation+90);
        if(!maybeCollide(temp))
        {
            mAniMoving->setEndValue(temp);
            mAniMoving->start(QPropertyAnimation::DeleteWhenStopped);
        }else
            mAniMoving->deleteLater();
        break;
    }


}




void Bullet::bulletShoot()
{
   //子弹步进制不要超过50，因为场景边际矩形宽度为50,固定子弹速度20
   int step=20;
   animation->setDuration(mSpeed);
   animation->setStartValue(pos());
   switch(mDirection)
   {
   case UP:
       setRotation(-90);
        if(isColliding())
            deleteLater();
        else
        {
            animation->setEndValue(pos()+QPointF(0,0-step));
            animation->start();
        }
       break;
   case DOWN:
       setRotation(90);
       if(isColliding())
           deleteLater();
      else
       {
           animation->setEndValue(pos()+QPointF(0,step));
           animation->start();
       }
      break;
   case LEFT:
       setRotation(180);
       if(isColliding())
           deleteLater();
       else
       {
           animation->setEndValue(pos()+QPointF(0-step,0));
           animation->start();
       }
      break;
   case RIGHT:
       setRotation(0);
       if(isColliding())
           deleteLater();
       else
       {
           animation->setEndValue(pos()+QPointF(step,0));
           animation->start();
       }
      break;
   }
}




Tankes::Tankes(int step,int duration,int interval,int bulletSpeed)
{
    mPauseMove=false;
    mPauseShoot=false;
    this->step=step;
    this->duration=duration;
    mBulletSpeed=bulletSpeed;
    mSrcRotation=rotation();
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
    YourBullet *bullet=new YourBullet();
    bullet->setSpeed(mBulletSpeed);
    int rt=rotation();
    if(rt==mSrcRotation)
    {
        bullet->setDirection(Bullet::UP);
        scene()->addItem(bullet);
        //bullet->setPos(pos().rx(),pos().ry());
        bullet->setPos(pos());
    }else if(rt==mSrcRotation+180)
    {
        bullet->setDirection(Bullet::DOWN);
        scene()->addItem(bullet);
        //bullet->setPos(pos().rx(),pos().ry());
        bullet->setPos(pos());
    }else if(rt==mSrcRotation-90)
    {
        bullet->setDirection(Bullet::LEFT);
        scene()->addItem(bullet);
        //bullet->setPos(pos().rx(),pos().ry());
        bullet->setPos(pos());
    }else if(rt==mSrcRotation+90)
    {
        bullet->setDirection(Bullet::RIGHT);
        scene()->addItem(bullet);
        //bullet->setPos(pos().rx(),pos().ry());
        bullet->setPos(pos());
    }
    bullet->bulletShoot();
}

void Tankes::slotBeShoot()
{
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
    int num=list.count();
     if(list.count()>0)
     {
         foreach(QGraphicsItem *items,list)
         {
             qreal x=items->boundingRect().width();
             qreal y=items->boundingRect().height();
             if(x==38&&y==38)
                  num--;
             if(x==11&&y==5)
                  num--;
             if(x==25&&y==25||x==50&&y==50)
             {
                 MyWall *wall=(MyWall*)items;
                 if(wall->getShape()==MyWall::GreenGrass){
                    wall->setZValue(1);
                    num--;
                 }
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


bool MyBullet::isColliding()
{
    int num=0;
    QList<QGraphicsItem*>list=collidingItems();
    num=list.count();
    if(num>0){
        foreach(QGraphicsItem *item,list){
            qreal x=item->boundingRect().width();
            qreal y=item->boundingRect().height();

            if(x==38&&y==38){
                Tankes *tankes=(Tankes*)item;
                if(!tankes->isAlive())
                      continue;
                tankes->slotBeShoot();
             }else if(x==36&&y==36){
                num--;
            }else if((x==25&&y==25) || (x==50&&y==50)){
                MyWall *wall=(MyWall*)item;
                if(wall->getShape()==MyWall::GreenGrass)
                {
                    num--;
                    wall->setZValue(1);
                    continue;
                }
                wall->beShoot();
            }
        }
    }
    if(num>0)
        return true;
    else
        return false;
}


bool YourBullet::isColliding()
{
    QList<QGraphicsItem*>list=collidingItems();
    //根据碰撞的图型项的个数判断是否发生碰撞
    int num;
    num=list.count();
    if(num>0){
        foreach(QGraphicsItem *item,list){
            qreal x=item->boundingRect().width();
            qreal y=item->boundingRect().height();
            if(x==36&&y==36)
            {
                Tanke *tanke=(Tanke*)item;
                if(!tanke->isAlive())
                    continue;
                tanke->slotBeShoot();
            }else if(x==38&&y==38)
            {
                //检测到碰撞了敌方自己的坦克，num--
                num--;
            }else if(x==25&&y==25||x==50&&y==50)
            {
                MyWall *wall=(MyWall*)item;
                if(wall->getShape()==MyWall::GreenGrass)
                {
                    num--;
                    wall->setZValue(1);
                    continue;
                }
                wall->beShoot();
            }
        }
    }
    //num代表了碰撞的个数
    if(num>0)
        return true;
    else
      return false;
}


//发射死亡信号并销毁自己
void Tankes::slotDestroy()
{
    emit sgDestroy();
    deleteLater();
}





bool RedSord::isColliding()
{
    int num=0;
    QList<QGraphicsItem*>list=collidingItems();
    num=list.count();
    if(num>0){
        foreach(QGraphicsItem *item,list){
            qreal x=item->boundingRect().width();
            qreal y=item->boundingRect().height();

            if(x==38&&y==38){
                Tankes *tankes=(Tankes*)item;
                if(!tankes->isAlive())
                      continue;
                tankes->slotBeShoot();
             }else if(x==36&&y==36){
                num--;
            }else if((x==25&&y==25) || (x==50&&y==50)){
                MyWall *wall=(MyWall*)item;
                if(wall->getShape()==MyWall::GreenGrass)
                {
                    num--;
                    wall->setZValue(1);
                    continue;
                }
                wall->beShoot();
            }
        }
    }
    if(num>0)
        return true;
    else
        return false;
}

void RedSord::bulletShoot()
{
    switch(mDirection)
    {
    case UP:
        setRotation(-90);
        break;
    case DOWN:
        setRotation(90);
       break;
    case LEFT:
        setRotation(180);
       break;
    case RIGHT:
        setRotation(0);
       break;
    }
    isColliding();
    QTimer::singleShot(100,this,SLOT(deleteLater()));
}
