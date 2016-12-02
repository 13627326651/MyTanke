#include <QApplication>
#include <QPen>
#include "tanke.h"
#include "tankeview.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTextCodec>
#include <QTimer>

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    TankeView *view=new TankeView;
    view->show();
//    QTimer::singleShot(20000,view,SLOT(deleteLater()));
//    QWidget *widget=new QWidget;
//    QTimer::singleShot(1000,widget,SLOT(show()));
    return app.exec();
}

