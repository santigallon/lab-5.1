#include "particula.h"
#include <QBrush>

Particula::Particula(qreal x, qreal y, qreal vx_, qreal vy_)
    : vx(vx_), vy(vy_)
{
    setRect(-5, -5, 10, 10);
    setBrush(QBrush(Qt::cyan));
    setPos(x, y);
}

void Particula::advancePhys(qreal dt)
{
    // gravedad
    vy += 9.8 * dt;

    // actualizar posición
    moveBy(vx * dt, vy * dt);

    // detectar impacto con el suelo
    if (y() > 380) {
        emit impacto(pos());
    }
}
