#ifndef MYWALL_H
#define MYWALL_H

#include <QGraphicsObject>
#include <phonon>

class MyWall : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Shape{RedWall,WhiteWall,BlueWater};
    explicit MyWall(Shape shape=RedWall,int liveValue=2);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setWallType(Shape shape);
    int getLiveValue();
    void beShoot();
signals:
    
public slots:
    void slotDestroy();
private:
    Shape mShape;
    int mLifeVal;
    Phonon::MediaObject *shootOverSound;
};

#endif // MYWALL_H
