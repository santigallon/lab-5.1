#include "simulation.h"
#include <cmath>

Simulation::Simulation(QGraphicsScene *scene, double w, double h)
    : m_scene(scene), ancho(w), alto(h)
{}

void Simulation::agregarParticula(particula *p) {
    m_particulas.push_back(p);
    if (m_scene) m_scene->addItem(p);
}

void Simulation::agregarObstaculo(obstaculo *o) {
    m_obstaculos.push_back(o);
    if (m_scene) m_scene->addItem(o);
}

void Simulation::setGravedad(double g) {
    gravedad = g;
}

void Simulation::setRestitucion(double r) {
    restitucion = r;
}

void Simulation::paso(double dt)
{
    // 1. Actualizar movimiento
    for (auto p : m_particulas) {
        if (gravedad != 0)
            p->aplicarImpulso(vectordes(0, gravedad * p->masa() * dt));

        p->paso(dt);
    }

    // 2. Colisiones con paredes
    for (auto p : m_particulas)
        manejarPared(p);

    // 3. Colisiones con obstáculos
    for (auto p : m_particulas)
        for (auto o : m_obstaculos)
            manejarObstaculo(p, *o);

    // 4. Colisiones entre partículas
    manejarColisionesEntreParticulas();
}

void Simulation::manejarPared(particula *p)
{
    vectordes pos = p->posicion();
    vectordes vel = p->velocidad();
    double r = p->radio();

    bool cambio = false;

    if (pos.x - r < 0) {
        pos.x = r;
        vel.x = -vel.x * restitucion;
        cambio = true;
    }
    if (pos.x + r > ancho) {
        pos.x = ancho - r;
        vel.x = -vel.x * restitucion;
        cambio = true;
    }
    if (pos.y - r < 0) {
        pos.y = r;
        vel.y = -vel.y * restitucion;
        cambio = true;
    }
    if (pos.y + r > alto) {
        pos.y = alto - r;
        vel.y = -vel.y * restitucion;
        cambio = true;
    }

    if (cambio) {
        // aplicar corrección inmediata: fijar posición y velocidad
        p->setPosicion(pos);
        p->setVelocidad(vel);
    }
}

void Simulation::manejarObstaculo(particula *p, const obstaculo &o)
{
    double px = p->posicion().x;
    double py = p->posicion().y;
    double r  = p->radio();

    double rx = o.x();
    double ry = o.y();
    double rw = o.w();
    double rh = o.h();

    // Si no hay colisión, salir
    if (!p->colicionaConRectangulo(rx, ry, rw, rh))
        return;

    vectordes vel = p->velocidad();

    // Calcular penetración en cada lado
    double penLeft   = (px + r) - rx;
    double penRight  = (rx + rw) - (px - r);
    double penTop    = (py + r) - ry;
    double penBottom = (ry + rh) - (py - r);

    // Elegir menor penetración para saber cara de colisión
    double minPen = std::min(std::min(penLeft, penRight),
                             std::min(penTop, penBottom));

    vectordes newPos = p->posicion();

    if (minPen == penLeft) {
        // Golpeó por la izquierda
        newPos.x = rx - r;
        vel.x = -vel.x * restitucion;
    }
    else if (minPen == penRight) {
        // Golpeó por la derecha
        newPos.x = rx + rw + r;
        vel.x = -vel.x * restitucion;
    }
    else if (minPen == penTop) {
        // Golpeó por arriba
        newPos.y = ry - r;
        vel.y = -vel.y * restitucion;
    }
    else { // penBottom
        // Golpeó por abajo
        newPos.y = ry + rh + r;
        vel.y = -vel.y * restitucion;
    }

    // Aplicar corrección de posición y velocidad
    p->setPosicion(newPos);
    p->setVelocidad(vel);
}

void Simulation::manejarColisionesEntreParticulas()
{
    for (size_t i = 0; i < m_particulas.size(); ++i) {
        for (size_t jj = i + 1; jj < m_particulas.size(); ++jj) {

            particula *a = m_particulas[i];
            particula *b = m_particulas[jj];

            if (!a->colicionaCon(*b)) continue;

            vectordes va = a->velocidad();
            vectordes vb = b->velocidad();

            // Conservación del momento + restitución
            vectordes normal = vectordes(b->posicion().x - a->posicion().x,
                                         b->posicion().y - a->posicion().y);

            double dist = longitud(normal);
            if (dist == 0) continue;

            normal = normal / dist;

            double vRel = punto(vb - va, normal);
            // si se separan, no aplicar
            if (vRel >= 0) continue;

            double e = restitucion;
            double invMass = 1.0 / a->masa() + 1.0 / b->masa();

            double impulsoMagnitud = -(1 + e) * vRel / invMass;

            vectordes impulso = normal * impulsoMagnitud;

            a->aplicarImpulso(impulso * -1);
            b->aplicarImpulso(impulso);

            // separar ligeramente si están muy penetradas (corrección posicional mínima)
            double overlap = a->radio() + b->radio() - dist;
            if (overlap > 0) {
                vectordes correction = normal * (overlap / (a->masa() + b->masa())); // simple
                a->setPosicion(vectordes(a->posicion().x - correction.x * b->masa(),
                                         a->posicion().y - correction.y * b->masa()));
                b->setPosicion(vectordes(b->posicion().x + correction.x * a->masa(),
                                         b->posicion().y + correction.y * a->masa()));
            }
        }
    }
}
