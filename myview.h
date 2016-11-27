#ifndef MYVIEW_H
#define MYVIEW_H
#include <QGraphicsView>
#include "tanke.h"

class MyView : public QGraphicsView
{
public:
    MyView();
    void initView();
    void createTanke();
protected:
    void keyPressEvent(QKeyEvent *event);
    void addBarrier();
private:
    QGraphicsLineItem *topLine;
    QGraphicsLineItem *bottomLine;
    QGraphicsLineItem *leftLine;
    QGraphicsLineItem *rightLine;
private:
    Tanke *mMyTanke;
};

#endif // MYVIEW_H
