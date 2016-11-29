#include <QApplication>
#include <QPen>
#include "tanke.h"
#include "tankeview.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextCodec>

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    TankeView *view=new TankeView;
    view->show();
    return app.exec();
}

