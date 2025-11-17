#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsScene>
#include <vector>
#include "particula.h"
#include "obstaculo.h"

class Simulation
{
public:
    Simulation(QGraphicsScene *scene);

    void setBounds(double width, double height);
    void setGravity(double g);
    void setRestitucionObstaculo(double e);

    void agregarParticula(particula *p);
    void agregarObstaculo(obstaculo *o);

    void paso(double dt);

private:
    QGraphicsScene *m_scene;
    double m_w = 800;
    double m_h = 600;
    double m_g = 0;
    double m_e = 0;

    std::vector<particula*> m_particulas;
    std::vector<obstaculo*> m_obstaculos;

    void manejarPared(particula &p);
    void manejarObstaculo(particula &p, obstaculo &o);
    void manejarColisionesParticulas();
};

#endif // SIMULATION_H
