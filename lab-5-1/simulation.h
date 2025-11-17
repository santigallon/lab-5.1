#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <QGraphicsScene>
#include "particula.h"
#include "obstaculo.h"

class Simulation {
public:
    // Ahora recibe la scene para dibujar automáticamente los items
    Simulation(QGraphicsScene *scene, double w, double h);

    void agregarParticula(particula *p);
    void agregarObstaculo(obstaculo *o);

    void setGravedad(double g);
    void setRestitucion(double r);

    void paso(double dt);

private:
    QGraphicsScene *m_scene;
    double ancho, alto;
    double gravedad = 0.0;
    double restitucion = 0.8; // rebote más energético

    std::vector<particula*> m_particulas;
    std::vector<obstaculo*> m_obstaculos;

    void manejarPared(particula *p);
    void manejarObstaculo(particula *p, const obstaculo &o);
    void manejarColisionesEntreParticulas();
};

#endif
