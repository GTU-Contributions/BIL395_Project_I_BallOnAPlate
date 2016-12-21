#include "drawer.h"
#include <QGraphicsSceneMouseEvent>

Drawer::Drawer(QObject * parent):QGraphicsScene(parent)
{
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);
}

void Drawer::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(mouseEvent->button() == Qt::LeftButton && !finishPath){
        if(closeToStartPoint){
            closeToStartPoint = false;
            finishPath = true;
        }
        else{
            QPointF point = mouseEvent->scenePos();
            point = collision(point.x(), point.y());
            path.append(point);
        }
    }
    else if(mouseEvent->button() == Qt::RightButton){
        path.clear();
        closeToStartPoint = false;
        finishPath = false;
    }

    draw();
    QString coordinates = QString("X: ") + QString::number(positionToCoordinateX(mouseEvent->scenePos().x())) + QString(" Y: ") + QString::number(positionToCoordinateX(mouseEvent->scenePos().y()));
    this->addText(coordinates);

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Drawer::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!finishPath && path.size() > 1){
        qreal dx = mouseEvent->scenePos().x() - path.at(0).x();
        qreal dy = mouseEvent->scenePos().y() - path.at(0).y();
        if(qSqrt(dx*dx+dy*dy) < MAX_RADIUS)
            closeToStartPoint = true;
        else
            closeToStartPoint = false;
    }

    draw();
    QString coordinates = QString("X: ") + QString::number(positionToCoordinateX(mouseEvent->scenePos().x())) + QString(" Y: ") + QString::number(positionToCoordinateX(mouseEvent->scenePos().y()));
    this->addText(coordinates);

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Drawer::draw(){
    this->clear();
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);

    if(!path.empty()){
        QPen linePen(Qt::black);
        linePen.setWidth(POINT_RADIUS);
        linePen.setJoinStyle(Qt::RoundJoin);
        QPen pointPen(Qt::red);
        QBrush pointBrush(Qt::red);
        QPen startPointPen(Qt::green);
        QBrush startPointBrush(Qt::green);

        for(int i=1;i<path.size();++i)
            this->addLine(QLineF(path.at(i-1), path.at(i)), linePen);

        if(finishPath)
            this->addLine(QLineF(path.at(path.size()-1), path.at(0)), linePen);

        for(int i=1;i<path.size();++i)
            this->addEllipse(path.at(i).x()-POINT_RADIUS, path.at(i).y()-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, pointPen, pointBrush);

        if(closeToStartPoint)
            this->addEllipse(path.at(0).x()-MAX_RADIUS, path.at(0).y()-MAX_RADIUS, 2*MAX_RADIUS, 2*MAX_RADIUS, startPointPen, startPointBrush);
        else
            this->addEllipse(path.at(0).x()-POINT_RADIUS, path.at(0).y()-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, startPointPen, startPointBrush);
    }
}

qreal Drawer::collisionX(qreal x)
{
    if(x-MAX_RADIUS < LEFT_BORDER)
        return LEFT_BORDER + MAX_RADIUS;

    if(x+MAX_RADIUS > RIGHT_BORDER)
        return RIGHT_BORDER - MAX_RADIUS;

    return x;
}

qreal Drawer::collisionY(qreal y)
{
    if(y-MAX_RADIUS < TOP_BORDER)
        return TOP_BORDER + MAX_RADIUS;

    if(y+MAX_RADIUS > BOTTOM_BORDER)
        return BOTTOM_BORDER - MAX_RADIUS;

    return y;
}

QPointF Drawer::collision(QPointF &point)
{
    return collision(point.x(), point.y());
}

QPointF Drawer::collision(qreal x, qreal y)
{
    return QPointF(collisionX(x), collisionY(y));
}

int Drawer::positionToCoordinateX(qreal x)
{
    const qreal X_MAX = 1000;
    return LEFT_BORDER + x*X_MAX/(RIGHT_BORDER-LEFT_BORDER);
}

int Drawer::positionToCoordinateY(qreal y)
{
    const qreal Y_MAX = 1000;
    return TOP_BORDER + y*Y_MAX/(BOTTOM_BORDER-TOP_BORDER);
}

QPoint Drawer::positionToCoordinate(QPointF &point){
    return QPoint(positionToCoordinateX(point.x()), positionToCoordinateY(point.y()));
}

