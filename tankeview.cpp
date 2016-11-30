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

static int tankeCount=20;
static const int tankeNum=6;

TankeView::TankeView()
{
    initView();
}

void TankeView::initView()
{
    setRenderHint(QPainter::Antialiasing);
    // ���û��汳�����������Լӿ���Ⱦ�ٶ�
    setCacheMode(CacheBackground);
    int viewWidth=803+100;
    int viewHeight=603;
    setMinimumSize(viewWidth,viewHeight);
    setMaximumSize(viewWidth,viewHeight);
    setBackgroundBrush(Qt::blue);
    scene=new QGraphicsScene(this);
    setScene(scene);
    scene->setSceneRect(-400,-300,900,600);
    //�ڳ�������ӱ߽���Σ���ֹ�ӵ��������
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
    //��ʼ���ұ�̹����Ϣ
    initTankeInfo();
    //�ڳ������������
    addBarrier();
    //��ʾ��Ϸ����
    displayMenu();
    //��ʼ����������
    initMusic();
   //��ʼ���ҵ�̹�ˣ����ú�λ�ã���ʼ������ͣ״̬
    mMyTanke=new Tanke(5,50,100);
    scene->addItem(mMyTanke);
    mMyTanke->setPos(0,270);
    mMyTanke->pause();
    connect(mMyTanke,SIGNAL(sgDestroy()),this,SLOT(slotUpdateTanke()));
}

void TankeView::createTanke(int count)
{
//    ʵ����tankes��һ��Ҫ������볡���У���Ϊtanke�����з��ʳ�������Ȼ�ڴ���ʳ���
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

    startBtn=new QPushButton(tr("��ʼ"));
    connect(startBtn,SIGNAL(clicked()),this,SLOT(beginGame()));
    QGraphicsProxyWidget *start=scene->addWidget(startBtn);
    start->setPos(0,0);
    start->setZValue(1);

    mTextItem=new QGraphicsTextItem(0,scene);
    mTextItem->setHtml(tr("<font color=black>̹�˴�ս</font>"));
    mTextItem->setZValue(1);
    mTextItem->setFont(QFont("Times",30,QFont::Bold));
    mTextItem->setPos(-40,-150);
}

void TankeView::slotCreateTanke()
{
    if(tankeNum<tankeCount)
    {
        createTanke(tankeNum);
        tankeCount-=tankeNum;
    }
    if(tankeNum>tankeCount&&tankeCount>0)
    {
        createTanke(tankeCount);
        tankeCount=0;
    }
}

void TankeView::beginGame()
{
    before->stop();
    //running->play();
    mMyTanke->resume();
    connect(running,SIGNAL(finished()),this,SLOT(slotReplay()));

    menuWidget->hide();
    startBtn->hide();
    mTextItem->hide();

    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotCreateTanke()));
    timer->start(30000);

    createTanke(tankeNum);
    tankeCount-=tankeNum;
}

void TankeView::slotReplay()
{
    running->stop();
    running->play();
}

void TankeView::gameOver()
{
    running->stop();
    menuWidget->show();

   startBtn->setText(tr("����"));
   disconnect(startBtn,SIGNAL(clicked()),this,SLOT(beginGame()));
   connect(startBtn,SIGNAL(clicked()),this,SLOT(close()));
   startBtn->show();

    mTextItem->setHtml(tr("<font color=black>��Ϸ����</font>"));
    mTextItem->show();
}

void TankeView::initMusic()
{
    before=new Phonon::MediaObject;
    running=new Phonon::MediaObject;
    Phonon::AudioOutput *audioBefore=new Phonon::AudioOutput(Phonon::MusicCategory,0);
    Phonon::AudioOutput *audioRunning=new Phonon::AudioOutput(Phonon::MusicCategory,0);
    Phonon::createPath(before,audioBefore);
    Phonon::createPath(running,audioRunning);
    before->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("tankeBefore.wav")));
    running->setCurrentSource(Phonon::MediaSource(SOURCEPATH+tr("tankeRunning.wav")));
    before->play();
}

void TankeView::slotUpdateTanke()
{
    QList<QGraphicsItem*>list=scene->itemAt(450,-295)->childItems();
    int i;
    for(i=20;i<23;i++)
    {
        QGraphicsItem*item=list.at(i);
        if(item->isVisible())
        {
            item->hide();
            break;
        }
    }
    if(i==23)
        gameOver();
}

void TankeView::slotUpdateTankes()
{
    QList<QGraphicsItem*>list=scene->itemAt(450,-295)->childItems();
    for(int i=0;i<20;i++)
    {
        QGraphicsItem*item=list.at(i);
        if(item->isVisible())
        {
            item->hide();
            if(i==19)
                gameOver();
            break;
        }
    }
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


void TankeView::addBarrier()
{
    WallGroup *gateOne=new GateOne();
    scene->addItem(gateOne);
    gateOne->setPos(-400,-300);
    gateOne->clearBroup();
    scene->removeItem(gateOne);
}




