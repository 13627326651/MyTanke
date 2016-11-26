#include <QApplication>
#include "tanke.h"
#include <QGraphicsScene>
#include <QGraphicsView>


int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    Tanke *tanke=new Tanke;
    Tankes *tankes=new Tankes;
    Tankes *tankes1=new Tankes;
    Tankes *tankes2=new Tankes;
    tanke->setFocus();
    QGraphicsScene scene;
    scene.setSceneRect(-200,-200,400,400);
    scene.addItem(tanke);
    scene.addItem(tankes);
    scene.addItem(tankes1);
    scene.addItem(tankes2);
 //   QTimer::singleShot(2000,tankes,SLOT(deleteLater()));
    tankes->setPos(-180,-180);
    tankes1->setPos(0,-180);
    tankes2->setPos(180,-180);
    scene.addLine(-200,-200,200,-200);//top
    scene.addLine(-200,-200,-200,200);//left
    scene.addLine(200,-200,200,200);//right
    scene.addLine(-200,200,200,200);//bottom
    QGraphicsView view;
    view.setMinimumSize(405,405);
    view.setMaximumSize(405,405);
    view.setScene(&scene);
    view.show();
    return app.exec();
}

