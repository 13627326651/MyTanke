#include "tankeview.h"
#include <QKeyEvent>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <phonon>
#define qd qDebug()<<

TankeView::TankeView()
{
    initView();
}

void TankeView::initView()
{
    setRenderHint(QPainter::Antialiasing);
    // 设置缓存背景，这样可以加快渲染速度
    setCacheMode(CacheBackground);
    int viewWidth=803+100;
    int viewHeight=603;
    setMinimumSize(viewWidth,viewHeight);
    setMaximumSize(viewWidth,viewHeight);
    setBackgroundBrush(Qt::black);
    scene=new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(-400,-300,900,600);
    //在场景外添加边界矩形，防止子弹射出场景
    topLine=scene->addRect(0,0,900,50);
    topLine->setPos(-450,-350);
    leftLine=scene->addRect(0,0,50,700);
    leftLine->setPos(-450,-350);
    bottomLine=scene->addRect(0,0,900,50);
    bottomLine->setPos(-450,300);
    rightLine=scene->addRect(0,0,50,700);
    rightLine->setPos(400,-350);
    infoDisplay=scene->addRect(0,0,100,600,QPen(),QBrush(Qt::lightGray));
    infoDisplay->setPos(400,-300);
    displayMenu();
}

void TankeView::createTanke()
{

//    实例化tankes后一定要将其加入场景中，因为tanke类中有访问场景，不然内存访问出错。
    Tankes *tankesArr=new Tankes[20];
    for(int i=0;i<20;i++)
    {
        if(i<9)
        {
            scene->addItem(&tankesArr[i]);
            tankesArr[i].setPos(-380,-280);
            tankesArr[i].moving();
        }else if(i<15)
        {
            scene->addItem(&tankesArr[i]);
            tankesArr[i].setPos(0,-280);
            tankesArr[i].moving();
        }else if(i<20)
        {
            scene->addItem(&tankesArr[i]);
            tankesArr[i].setPos(380,-280);
            tankesArr[i].moving();
        }
    }

//    Tankes *tankes=new Tankes;
//    scene()->addItem(tankes);
//    tankes->setPos(-380,-280);
//    tankes->moving();
    mMyTanke=new Tanke(5,50,100);
    scene->addItem(mMyTanke);
    mMyTanke->setPos(0,270);
}

void TankeView::initTankeInfo()
{
    for(int i=0;i<10;i++)
    {
        QGraphicsPixmapItem *item=scene->addPixmap(tr(":/images/tankeIcon.png"));
        item->setParentItem(infoDisplay);
        item->setPos(20,25+i*25);
    }
    for(int i=0;i<10;i++)
    {
        QGraphicsPixmapItem *item=scene->addPixmap(tr(":/images/tankeIcon.png"));
        item->setParentItem(infoDisplay);
        item->setPos(60,25+i*25);
    }

    for(int i=0;i<3;i++)
    {
        QGraphicsPixmapItem *item=scene->addPixmap(tr(":/images/tankeIcon01.png"));
        item->setParentItem(infoDisplay);
        item->setPos(20,450+i*25);
    }
}

void TankeView::displayMenu()
{
    menuWidget=new QWidget;
    menuWidget->resize(903,603);
    menuWidget->setAutoFillBackground(true);
    menuWidget->setPalette(QPalette(Qt::gray));
    QGraphicsProxyWidget *menu=scene->addWidget(menuWidget);
    menu->setPos(-400,-300);
    menu->setZValue(1);

    startBtn=new QPushButton(tr("开始"));
    connect(startBtn,SIGNAL(clicked()),this,SLOT(beginGame()));
    QGraphicsProxyWidget *start=scene->addWidget(startBtn);
    start->setPos(0,0);
    start->setZValue(1);

    mTextItem=new QGraphicsTextItem(0,scene);
    mTextItem->setHtml(tr("<font color=black>坦克大战</font>"));
    mTextItem->setZValue(1);
    mTextItem->setFont(QFont("Times",30,QFont::Bold));
    mTextItem->setPos(-40,-150);

    before=new Phonon::MediaObject;
    running=new Phonon::MediaObject;
    Phonon::AudioOutput *audioBefore=new Phonon::AudioOutput(Phonon::MusicCategory,0);
    Phonon::AudioOutput *audioRunning=new Phonon::AudioOutput(Phonon::MusicCategory,0);
    Phonon::createPath(before,audioBefore);
    Phonon::createPath(running,audioRunning);
    before->setCurrentSource(Phonon::MediaSource(tr("../MyTanke/tankeBefore.wav")));
    running->setCurrentSource(Phonon::MediaSource(tr("../MyTanke/tankeRunning.wav")));
    before->play();
}

void TankeView::beginGame()
{
    before->stop();
    running->play();
    connect(running,SIGNAL(finished()),this,SLOT(slotReplay()));

    menuWidget->hide();
    startBtn->hide();
    mTextItem->hide();

    initTankeInfo();
    addBarrier();
    createTanke();
}

void TankeView::slotReplay()
{
    running->enqueue(running->currentSource());
}

void TankeView::keyPressEvent(QKeyEvent *event)
{
//    mMyTanke->setFocus();
//    if(!mMyTanke->isVisible())
//    {
//        if(event->key()==Qt::Key_Return)
//        {
//            mMyTanke->show();
//            mMyTanke->setAlive(true);
//        }
//    }
    QGraphicsView::keyPressEvent(event);
}

void TankeView::addBarrier()
{

    //上部分
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(-340,-260));
    addBarrierLine(REDWALL,Qt::Vertical,6,QPointF(-190,-260));
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(-75,-260));
    addBarrierLine(WHITEWALL,Qt::Horizontal,1,QPointF(-25,-135));
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(25,-260));
    addBarrierLine(REDWALL,Qt::Vertical,6,QPointF(140,-260));
    addBarrierLine(REDWALL,Qt::Vertical,5,QPointF(290,-260));
    //中间部分
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(-75,-35));
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(25,-35));

    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(-400,-35));
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(350,-35));

    addBarrierLine(REDWALL,Qt::Horizontal,3,QPointF(150,0));
    addBarrierLine(REDWALL,Qt::Horizontal,3,QPointF(-300,0));
    //下面部分
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(-300,80));
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(-190,80));
    addBarrierLine(REDWALL,Qt::Vertical,3,QPointF(23,40));
    addBarrierLine(REDWALL,Qt::Vertical,1,QPointF(-25,70));
    addBarrierLine(REDWALL,Qt::Vertical,3,QPointF(-73,40));
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(140,80));
    addBarrierLine(REDWALL,Qt::Vertical,4,QPointF(250,80));

}

void TankeView::addBarrierLine(TankeView::BarrierType type, Qt::Orientation orientation, const int count,const QPointF position)
{

    qreal x=position.x();
    qreal y=position.y();
    if(orientation==Qt::Horizontal)
    {
        for(int i=0;i<count;i++)
        {
            QPixmap pixmap;
            QGraphicsPixmapItem *pixmapItem;
            switch(type)
            {
            case BLUEWATER:
                pixmap.load(tr(":/images/blueWater.png"));
                pixmapItem=scene->addPixmap(pixmap);
                pixmapItem->setPos(x+i*pixmapItem->boundingRect().width(),y);
                break;
            case WHITEWALL:
                pixmap.load(tr(":/images/whiteWall.png"));
                pixmapItem=scene->addPixmap(pixmap);
                pixmapItem->setPos(x+i*pixmapItem->boundingRect().width(),y);
                break;
            case REDWALL:
                pixmap.load(tr(":/images/redWall.png"));
                pixmapItem=scene->addPixmap(pixmap);
                pixmapItem->setPos(x+i*pixmapItem->boundingRect().width(),y);
                break;
            }
        }
    }else if(orientation==Qt::Vertical)
    {
        for(int i=0;i<count;i++)
        {
            QPixmap pixmap;
            QGraphicsPixmapItem *pixmapItem;
            switch(type)
            {
            case BLUEWATER:
                pixmap.load(tr(":/images/blueWater.png"));
                pixmapItem=scene->addPixmap(pixmap);
                pixmapItem->setPos(x,y+i*pixmapItem->boundingRect().height());
                break;
            case WHITEWALL:
                pixmap.load(tr(":/images/whiteWall.png"));
                pixmapItem=scene->addPixmap(pixmap);
                pixmapItem->setPos(x,y+i*pixmapItem->boundingRect().height());
                break;
            case REDWALL:
                pixmap.load(tr(":/images/redWall.png"));
                pixmapItem=scene->addPixmap(pixmap);
                pixmapItem->setPos(x,y+i*pixmapItem->boundingRect().height());
                break;
            }
        }
    }

}


