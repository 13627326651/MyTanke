#include "tankeview.h"
#include <QTime>
#include <QKeyEvent>
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <phonon>
#include "mywall.h"
#include "wallgroup.h"

#define SOURCEPATH tr("../MyTanke/music/")
#define qd qDebug()<<

static int tankeCount;
static int tankeNum;
static int gateNum=1;

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
    infoDisplay=scene->addRect(0,0,100,300,QPen(),QBrush(Qt::lightGray));
    infoDisplay->setPos(400,-300);
    infoDisplay1=scene->addRect(0,0,100,297,QPen(),QBrush(Qt::lightGray));
    infoDisplay1->setPos(400,0);
    //初始化右边坦克信息
    initTankeInfo(10);
    //创建产生敌人的计时器
    createTimer=new QTimer(this);
    connect(createTimer,SIGNAL(timeout()),this,SLOT(slotCreateTanke()));
    //创建我的坦克
    mMyTanke=new Tanke();
    scene->addItem(mMyTanke);
    mMyTanke->setPos(0,280);
    mMyTanke->pause();
    connect(mMyTanke,SIGNAL(sgDestroy()),this,SLOT(slotUpdateTanke()));
    //初始化背景音乐
    before=new Phonon::MediaObject;
    running=new Phonon::MediaObject;
    Phonon::AudioOutput *audioBefore=new Phonon::AudioOutput(Phonon::MusicCategory,0);
    Phonon::AudioOutput *audioRunning=new Phonon::AudioOutput(Phonon::MusicCategory,0);
    Phonon::createPath(before,audioBefore);
    Phonon::createPath(running,audioRunning);
    before->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("tankeBefore.wav")));
    //前景音乐一停止就开始真正的游戏
    connect(before,SIGNAL(finished()),this,SLOT(startGame()));
    running->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("tankeRunning.wav")));
    //显示游戏界面
    displayMenu();
}

void TankeView::createTanke(int count)
{
//    实例化tankes后一定要将其加入场景中，因为tanke类中有访问场景，不然内存访问出错
    QPointF points[3]={QPointF(-380,-280),QPointF(0,-280),QPointF(380,-280)};
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(int i=0;i<count;i++)
    {
        int index=qrand()%3;
        Tankes *tanke=new Tankes;
        scene->addItem(tanke);
        tanke->setPos(points[index]);
        connect(tanke,SIGNAL(sgDestroy()),this,SLOT(slotUpdateTankes()));

        QPropertyAnimation *animation=new QPropertyAnimation(tanke,"scale");
        animation->setDuration(2000);
        animation->setStartValue(0.0);
        animation->setEndValue(1.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        connect(animation,SIGNAL(finished()),tanke,SLOT(moving()));
    }

}

void TankeView::initTankeInfo(int cnt)
{
    for(int i=0;i<cnt;i++)
    {
        QGraphicsPixmapItem *item=scene->addPixmap(tr(":/images/tankeIcon.png"));
        item->setParentItem(infoDisplay);
        item->setPos(20,25+i*25);
    }
    for(int i=0;i<cnt;i++)
    {
        QGraphicsPixmapItem *item=scene->addPixmap(tr(":/images/tankeIcon.png"));
        item->setParentItem(infoDisplay);
        item->setPos(60,25+i*25);
    }

    for(int i=0;i<3;i++)
    {
        QGraphicsPixmapItem *item=scene->addPixmap(tr(":/images/tankeIcon01.png"));
        item->setParentItem(infoDisplay1);
        item->setPos(20,150+i*25);
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
    menu->setZValue(2);

    startBtn=new QPushButton(tr("开始"));
    connect(startBtn,SIGNAL(clicked()),this,SLOT(nextGate()));
    QGraphicsProxyWidget *start=scene->addWidget(startBtn);
    start->setPos(0,0);
    start->setZValue(2);

    mTextItem=new QGraphicsTextItem(0,scene);
    mTextItem->setHtml(tr("<font color=black>坦克大战</font>"));
    mTextItem->setZValue(2);
    mTextItem->setFont(QFont("Times",30,QFont::Bold));
    mTextItem->setPos(-40,-150);

}

void TankeView::slotCreateTanke()
{
    if(tankeNum<=tankeCount)
    {
        createTanke(tankeNum);
        tankeCount-=tankeNum;
    }
    if(tankeNum>tankeCount&&tankeCount>0)
    {
        createTanke(tankeCount);
        tankeCount=0;
        createTimer->stop();
    }
}

void TankeView::startGame()
{ 
    before->stop();
    menuWidget->hide();
    mTextItem->hide();
    addTankes(6,20);
    mMyTanke->resume();
    mMyTanke->setPos(0,280);
}

void TankeView::slotReplay()
{
    running->stop();
    running->play();
}

void TankeView::gameOver()
{
    cleanScene();
    createTimer->stop();

    running->stop();
    menuWidget->show();

    startBtn->setText(tr("结束"));
    disconnect(startBtn,SIGNAL(clicked()),this,SLOT(nextGate()));
    connect(startBtn,SIGNAL(clicked()),this,SLOT(close()));
    startBtn->show();

    mTextItem->setFont(QFont("Times",30,QFont::Bold));
    mTextItem->setHtml(tr("<font color=black>游戏结束</font>"));
    mTextItem->setPos(-40,-150);
    mTextItem->show();
}


void TankeView::addTankes(int TankeNum,int TankeCount)
{
    tankeNum=TankeNum;
    tankeCount=TankeCount;
    createTanke(tankeNum);
    tankeCount-=tankeNum;
    createTimer->start(10000);
}

void TankeView::addGate(int gateNum)
{
    //添加关卡
    WallGroup *gate;
    switch(gateNum)
    {
    case 1:
        gate=new GateOne();
        break;
    case 2:
        gate=new GateTwo();
        break;
    default:
        gate=new GateOne();
    }
    scene->addItem(gate);
    gate->setPos(-400,-300);
    gate->clearBroup();
    scene->removeItem(gate);
    gate->deleteLater();
}

void TankeView::nextGate()
{
   // before->play();
    startBtn->hide();
    menuWidget->show();
    mTextItem->setHtml(tr("<font color=black>第%1关</font>").arg(gateNum));
    mTextItem->setFont(QFont("Times",10,QFont::Bold));
    mTextItem->setPos(0,0);
    mTextItem->show();
    //重置右边面板信息
    QList<QGraphicsItem*>list=infoDisplay->childItems();
    foreach(QGraphicsItem*item,list)
    {
        if(!item->isVisible())
            item->show();
    }
    //清理场景
    cleanScene();
    //添加关卡
    addGate(gateNum);
    gateNum++;
    QTimer::singleShot(1000,this,SLOT(startGame()));
}



void TankeView::slotUpdateTanke()
{
    QList<QGraphicsItem*>list=infoDisplay1->childItems();
    bool isOver=true;
    foreach(QGraphicsItem*item,list)
    {
        if(item->isVisible()){
            item->hide();
            isOver=false;
            break;
        }
    }
    if(isOver)
        gameOver();
}

void TankeView::slotUpdateTankes()
{
    QList<QGraphicsItem*>list=infoDisplay->childItems();
    for(int i=0;i<list.count();i++)
    {
        QGraphicsItem*item=list.at(i);
        if(item->isVisible())
        {
            item->hide();
            if(i==list.count()-1)
                QTimer::singleShot(3000,this,SLOT(nextGate()));
                //nextGate();
            break;
        }
    }
}

void TankeView::cleanScene()
{
    //清理场景,设置我为hide
    mMyTanke->hide();
    QList<QGraphicsItem*>listBarrers=scene->items(-397,-297,794,594);
    foreach(QGraphicsItem*item,listBarrers)
    {
        if(item->zValue()>1)
            continue;
//        QGraphicsObject *object=(QGraphicsObject*)item;//并不是每个场景中的物体都是这个类的
//        object->deleteLater();
        delete item;
    }
    //清理完再显示出来
    mMyTanke->show();
}

void TankeView::keyPressEvent(QKeyEvent *event)
{
    mMyTanke->setFocus();
    if(!mMyTanke->isVisible())
    {
        if(event->key()==Qt::Key_Return)
        {
            mMyTanke->setPos(0,270);
            mMyTanke->show();
            mMyTanke->resume();
        }
    }
    QGraphicsView::keyPressEvent(event);
}






