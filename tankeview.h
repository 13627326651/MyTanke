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
    void initTankeInfo();
    void displayMenu();
    void gameOver();
    void initMusic();
public slots:
    void slotCreateTanke();
    void beginGame();
    void slotReplay();
    void slotUpdateTanke();
    void slotUpdateTankes();
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

    Phonon::MediaObject *before;
    Phonon::MediaObject *running;
    Phonon::MediaObject *end;

    QGraphicsTextItem *mTextItem;
    QWidget *menuWidget;
    QPushButton *startBtn;
private:
    Tanke *mMyTanke;
};

#endif // TANKEVIEW_H
