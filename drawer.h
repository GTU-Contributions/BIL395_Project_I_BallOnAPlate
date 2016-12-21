#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>
#include <QList>
#include <QtGui>
#include <QtCore>

class Drawer:public QGraphicsScene
{
public:
    Drawer(QObject * parent = 0);
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
    void draw();
    const qreal LEFT_BORDER = 0;
    const qreal RIGHT_BORDER = 400;
    const qreal TOP_BORDER = 0;
    const qreal BOTTOM_BORDER = 400;
    const qreal POINT_RADIUS = 10;
    const qreal MAX_RADIUS = 20; //MAX_RADIUS >= 2*POINT_RADIUS
private:
    qreal collisionX(qreal x);
    qreal collisionY(qreal y);
    QPointF collision(QPointF &point);
    QPointF collision(qreal x, qreal y);

    int positionToCoordinateX(qreal x);
    int positionToCoordinateY(qreal y);
    QPoint positionToCoordinate(QPointF &point);

    bool closeToStartPoint = false;
    bool finishPath = false;
    QList<QPointF> path;

};

#endif // DRAWER_H
