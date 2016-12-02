#include "bullet.h"
#include "tanke.h"
#include <QPointF>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include "mywall.h"
#include "enemy.h"
#include <QDebug>

bool Bullet::isColliding()
{
    int num=0;
    QList<QGraphicsItem*>list=collidingItems();
    num=list.count();
    if(num>0){
        foreach(QGraphicsItem *item,list){
            qreal x=item->boundingRect().width();
            qreal y=item->boundingRect().height();

            if(x==38&&y==38){
                if(mUser==Enemy)
                {
                    num--;
                }else{
                    Tankes *tankes=(Tankes*)item;
                    if(!tankes->isAlive())
                          continue;
                    tankes->slotBeShoot();
                }
             }else if(x==36&&y==36){
                if(mUser==Friend)
                {
                    num--;
                }else {
                    Tanke *tanke=(Tanke*)item;
                    if(!tanke->isAlive())
                        continue;
                    tanke->slotBeShoot();
                }
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

void RedSordBullet::bulletShoot()
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


