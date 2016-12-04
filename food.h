#ifndef FOOD_H
#define FOOD_H
#include <QGraphicsObject>
#include <QPainter>
#include <phonon>


class Food : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Food();
    QRectF boundingRect() const{
        return QRectF(-20,-20,40,40);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QPixmap pixmap(tr(":/images/star.gif"));
        painter->drawPixmap(-20,-20,40,40,pixmap);
    }
    void checkCollide();
public slots:
    void startShinning();
private:
    QTimer *shinning;
    int isShow;
    Phonon::MediaObject *eatSound;
    bool liveVal;
};


#endif // FOOD_H
