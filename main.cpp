#include <QApplication>
#include "tanke.h"
#include <QGraphicsScene>
#include <QGraphicsView>


int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    Tanke tanke;
    tanke.setFocus();
    QGraphicsScene scene;
    scene.setSceneRect(-100,-100,200,200);
    scene.addItem(&tanke);
    scene.addLine(-100,-100,100,-100);//top
    scene.addLine(-100,-100,-100,100);//left
    scene.addLine(100,-100,100,100);//right
    scene.addLine(-100,100,100,100);//bottom
    QGraphicsView view;
    view.setScene(&scene);
    view.show();
    return app.exec();
}

