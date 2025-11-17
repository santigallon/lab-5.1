#ifndef PARTICULA_H
#define PARTICULA_H

#include <QGraphicsEllipseItem>
#include "vectordes.h"

class particula : public QGraphicsEllipseItem
{
public:
    particula(double x, double y, double radio, double masa, const vectordes &velocidad);

    ~particula();

    vectordes posicion() const;
    vectordes velocidad() const;
    double masa() const;
    double radio() const;

    void paso(double dt);
    void aplicarImpulso(const vectordes &impulso);
    bool colicionaCon(const particula &other) const;
    bool colicionaConRectangulo(double rx, double ry, double rw, double rh) const;

    void absorber(particula &other);
    void setPosicion(const vectordes &p);
    void setVelocidad(const vectordes &v);

private:
    vectordes m_posicion;
    vectordes m_velocidad;
    double m_masa;
    double m_radio;
};

#endif // PARTICULA_H
