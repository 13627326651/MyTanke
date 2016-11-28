#include <QApplication>
#include <QPen>
#include "tanke.h"
#include "myview.h"
#include <QGraphicsScene>
#include <QGraphicsView>


int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    MyView *view=new MyView;
    view->show();
    return app.exec();
}

