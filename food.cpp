#include "food.h"
#include "enemy.h"
#include "tanke.h"
#include <QTimer>

#define SOURCEPATH tr("../MyTanke/music/")

Food::Food()
{
    liveVal=true;
    shinning=new QTimer(this);
    connect(shinning,SIGNAL(timeout()),this,SLOT(startShinning()));
    shinning->start(400);
    isShow=0;
    eatSound=new Phonon::MediaObject(this);
    Phonon::AudioOutput *audio=new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(eatSound,audio);
    eatSound->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("eatsound.mp3")));
    connect(eatSound,SIGNAL(finished()),this,SLOT(deleteLater()));
}

void Food::checkCollide()
{
    QList<QGraphicsItem*>list=collidingItems();
    if(list.count()>0)
    {
        foreach(QGraphicsItem*item,list)
        {
            qreal x=item->boundingRect().width();
            qreal y=item->boundingRect().height();
            if(x==38&&y==38&&liveVal){
                liveVal=false;
                eatSound->play();
                Tankes *tankes=(Tankes*)item;
                tankes->addStar();
            }else if(y==36&&x==36&&liveVal){
                liveVal=false;
                eatSound->play();
                Tanke*tanke=(Tanke*)item;
                tanke->addStar();
            }
        }
    }
}

void Food::startShinning()
{
    isShow++;
    if(isShow%2)
    {
        show();
    }else{
        hide();
    }
    checkCollide();
}
