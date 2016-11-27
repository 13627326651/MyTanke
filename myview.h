#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include "tanke.h"

class MyView : public QGraphicsView
{
public:
    enum BarrierType{BLUEWATER,WHITEWALL,REDWALL};
    MyView();
    void initView();
    void createTanke();
protected:
    void keyPressEvent(QKeyEvent *event);
    void addBarrier();
    void addBarrierLine(BarrierType type,Qt::Orientation orientation,const int count,const QPointF position);
private:
    QGraphicsLineItem *topLine;
    QGraphicsLineItem *bottomLine;
    QGraphicsLineItem *leftLine;
    QGraphicsLineItem *rightLine;
private:
    Tanke *mMyTanke;
};

#endif // MYVIEW_H
