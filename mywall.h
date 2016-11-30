#ifndef MYWALL_H
#define MYWALL_H

#include <QGraphicsObject>
#include <phonon>

//ʵ�ֵ�������(0,0,25*25)
class MyWall : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Shape{RedWall,WhiteWall,BlueWater,GreenGrass};
    explicit MyWall(Shape shape=RedWall,int liveValue=2);
    QRectF boundingRect() const{
       return QRectF(0,0,25,25);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setWallType(Shape shape){mShape=shape;}
    int getLiveValue(){ return mLifeVal;}
    void beShoot();
    void setLiveValue(int liveVal){mLifeVal=liveVal;}
public slots:
    void slotDestroy(){hide();}
protected:
    Shape mShape;
    int mLifeVal;
    Phonon::MediaObject *shootOverSound;
};

//ʵ�ֵ�������(0,0,50*50)
class MyWall4 :public MyWall
{
    Q_OBJECT
public:
    QRectF boundingRect() const{
        return QRectF(0,0,50,50);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
};


//ʵ��(0,0,50*50)�ĺ��ӣ��������ĸ�С��Ŀ(0,0,25*25)
class WallBox : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:
    enum WallType{WhiteWall,RedWall,BlueWater,GreenGrass};
    explicit WallBox(WallType type=RedWall,int liveVal=2):mWallType(type){
        mLiveVal=liveVal;
    }
    QRectF boundingRect() const{
        return QRectF(-25,-25,50,50);
    }
    virtual void initBox();
    void setWallType(WallType type){
        mWallType=type;
    }
    void setLiveVal(int liveVal){
        mLiveVal=liveVal;
    }
    void clearBroup();
signals:

public slots:
protected:
    int mLiveVal;
    WallType mWallType;
};

//ʵ��(0,0,50*50)�ĺ���,����ֻ��һ����Ŀ
class WallBox4:public WallBox
{
    Q_OBJECT
public:
    explicit WallBox4(WallType type=RedWall,int liveVal=2)
            : WallBox(type,liveVal){}
    void initBox();
};

#endif // MYWALL_H
