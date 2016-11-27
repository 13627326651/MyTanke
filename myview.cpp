#include "myview.h"
#include <QPixmap>
#include <QGraphicsScene>

MyView::MyView()
{
    initView();
}

void MyView::initView()
{
    setMinimumSize(800,600);
    setMaximumSize(800,600);
    setBackgroundBrush(Qt::black);
    QGraphicsScene *scene=new QGraphicsScene(this);
    scene->setSceneRect(-395,-295,790,590);
    topLine=scene->addLine(-395,-295,395,-295);
    leftLine=scene->addLine(-395,-295,-395,295);
    bottomLine=scene->addLine(-395,295,395,295);
    rightLine=scene->addLine(395,-295,395,295);
    setScene(scene);
    addBarrier();
    createTanke();

}

void MyView::createTanke()
{
    Tankes *tankesArr=new Tankes[20];
    for(int i=0;i<20;i++)
    {
        if(i<6)
        {
            scene()->addItem(&tankesArr[i]);
            tankesArr[i].setPos(-375,-275);

        }else if(i<12)
        {
            scene()->addItem(&tankesArr[i]);
            tankesArr[i].setPos(0,-275);
        }else if(i<20)
        {
            scene()->addItem(&tankesArr[i]);
            tankesArr[i].setPos(375,-275);
        }
    }
    mMyTanke=new Tanke;
    scene()->addItem(mMyTanke);
    mMyTanke->setPos(0,275);
}

void MyView::keyPressEvent(QKeyEvent *event)
{
    mMyTanke->setFocus();
    QGraphicsView::keyPressEvent(event);
}

void MyView::addBarrier()
{
    QPixmap *pixmapArr=new QPixmap[10];
    QGraphicsPixmapItem *pixmapItemArr[10];
    for(int i=0;i<10;i++)
    {
        pixmapArr[i].load(tr(":/images/whiteWall.png"));
    }
    pixmapItemArr[0]=scene()->addPixmap(pixmapArr[0]);
    pixmapItemArr[0]->setPos(-350,-210);
}
