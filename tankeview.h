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
    void createTanke();
    void initTankeInfo();
    void displayMenu();
public slots:
    void beginGame();
    void slotReplay();
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

    QGraphicsTextItem *mTextItem;
    QWidget *menuWidget;
    QPushButton *startBtn;
private:
    Tanke *mMyTanke;
};

#endif // TANKEVIEW_H
