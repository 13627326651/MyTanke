#ifndef TANKEVIEW_H
#define TANKEVIEW_H

#include <QPushButton>
#include <QGraphicsView>
#include "tanke.h"
class TankeView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TankeView();
    enum BarrierType{BLUEWATER,WHITEWALL,REDWALL};
    void initView();
    void createTanke(int count);
    void initTankeInfo(int cnt);
    void displayMenu();
    void gameOver();
    void initMusic();
    void resetTankeInfo();
    void addTankes(int TankeNum, int TankeCount);
    void addGate(int gateNum);
public slots:
    void nextGate();
    void slotCreateTanke();
    void startGame();
    void slotReplay();
    void slotUpdateTanke();
    void slotUpdateTankes();
    void cleanScene();
protected:
    void keyPressEvent(QKeyEvent *event);
    void addBarrier();
    void addBarrierLine(BarrierType type,Qt::Orientation orientation,const int count,const QPointF position);
private:
    QGraphicsScene *scene;
    QGraphicsRectItem *topLine;
    QGraphicsRectItem *bottomLine;
    QGraphicsRectItem *leftLine;
    QGraphicsRectItem *rightLine;
    QGraphicsRectItem *infoDisplay;
    QGraphicsRectItem *infoDisplay1;

    Phonon::MediaObject *before;
    Phonon::MediaObject *running;
    Phonon::MediaObject *end;

    QGraphicsTextItem *mTextItem;
    QWidget *menuWidget;
    QPushButton *startBtn;

    Tanke *mMyTanke;
    QTimer *createTimer;
};

#endif // TANKEVIEW_H
