#ifndef PARTICULA_H
#define PARTICULA_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QPointF>

class Particula : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Particula(qreal x, qreal y, qreal vx, qreal vy);

    void advancePhys(qreal dt);

    qreal vx, vy;
    qreal mass = 1.0;

signals:
    void impacto(QPointF pos);
};

#endif // PARTICULA_H
