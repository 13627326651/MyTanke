#ifndef BOXGROUP_H
#define BOXGROUP_H

#include <QObject>
#include <QGraphicsItemGroup>
class WallGroup : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit WallGroup();
signals:
    
public slots:
    
};

#endif // BOXGROUP_H
