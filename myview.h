#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include "tanke.h"

class MyView : public QGraphicsView
{
    Q_OBJECT
public:
    enum BarrierType{BLUEWATER,WHITEWALL,REDWALL};
    MyView();
    void initView();
    void createTanke();
    void initTankeInfo();
    void displayMenu();
protected:
    void keyPressEvent(QKeyEvent *event);
    void addBarrier();
    void addBarrierLine(BarrierType type,Qt::Orientation orientation,const int count,const QPointF position);

private:
    QGraphicsRectItem *topLine;
    QGraphicsRectItem *bottomLine;
    QGraphicsRectItem *leftLine;
    QGraphicsRectItem *rightLine;
    QGraphicsRectItem *infoDisplay;
    QWidget *menuWidget;
private:
    Tanke *mMyTanke;
};

#endif // MYVIEW_H
