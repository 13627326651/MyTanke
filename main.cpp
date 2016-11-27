#include <QApplication>
#include <QPen>
#include "tanke.h"
#include "myview.h"
#include <QGraphicsScene>
#include <QGraphicsView>


int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
//    Tanke *tanke=new Tanke;
//    Tankes *tankes=new Tankes;
//    Tankes *tankes1=new Tankes;
//    Tankes *tankes2=new Tankes;
//    tanke->setFocus();
//    QGraphicsScene scene;
//    scene.setSceneRect(-200,-200,400,400);
//    scene.addItem(tanke);
//    scene.addItem(tankes);
//    scene.addItem(tankes1);
//    scene.addItem(tankes2);
// //   QTimer::singleShot(2000,tankes,SLOT(deleteLater()));
//    tankes->setPos(-180,-180);
//    tankes1->setPos(0,-180);
//    tankes2->setPos(180,-180);
//    QPen pen;
//    pen.setWidth(5);
//    scene.addLine(-200,-200,200,-200,pen);//top
//    scene.addLine(-200,-200,-200,200,pen);//left
//    scene.addLine(200,-200,200,200,pen);//right
//    scene.addLine(-200,200,200,200,pen);//bottom
//    QGraphicsView view;
//    view.setMinimumSize(800,600);
//    view.setMaximumSize(800,600);
//    view.setScene(&scene);
//    view.show();
    MyView *view=new MyView;
    view->show();
    return app.exec();
}

