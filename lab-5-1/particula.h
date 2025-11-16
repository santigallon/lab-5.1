#ifndef PARTICULA_H
#define PARTICULA_H

#include <cmath>
#include <iostream>
#include "vectordes.h"

class particula
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
private:
    vectordes m_posicion;
    vectordes m_velocidad;
    double m_masa;
    double m_radio;
};

#endif // PARTICULA_H
