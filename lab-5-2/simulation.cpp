#include "simulation.h"
#include "particula.h"

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QtMath>
#include <QTimer>

Simulation::Simulation(QGraphicsScene *scene)
    : m_scene(scene)
{
    // buscamos las plataformas por su posición (como en tu escena)
    for (auto *item : scene->items()) {
        auto r = dynamic_cast<QGraphicsRectItem*>(item);
        if (!r) continue;

        if (r->rect().width() == 150 && r->rect().height() == 50) {
            if (r->x() < 200) platformLeft = r;
            else platformRight = r;
        }
    }

    // timer del simulador
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Simulation::step);
    timer->start(16); // ~60 FPS
}

void Simulation::launch(double angle_deg, double speed, QPointF start, int currentPlayer)
{
    if (m_particle != nullptr)
        return;

    double angle_rad = qDegreesToRadians(angle_deg);

    double vx = speed * qCos(angle_rad);
    double vy = -speed * qSin(angle_rad);  // hacia arriba

    // Si es jugador 2, invertimos el movimiento
    if (currentPlayer == 2) vx = -vx;

    m_particle = new Particula(start.x(), start.y(), vx, vy);
    m_scene->addItem(m_particle);

    connect(m_particle, &Particula::impacto, this, [this](QPointF p){
        emit finishedTurn();
    });
}

void Simulation::step()
{
    if (!m_particle) return;

    m_particle->advancePhys(0.016);

    // CHEQUEO DE COLISIONES CON LAS PLATAFORMAS
    auto coll = m_particle->collidingItems();
    for (auto *c : coll) {

        if (c == platformLeft || c == platformRight) {

            double velocidad = qSqrt(m_particle->vx*m_particle->vx +
                                     m_particle->vy*m_particle->vy);

            double daño = factor_constante * m_particle->mass * velocidad;

            if (c == platformLeft) emit hitPlatform(1, daño);
            else emit hitPlatform(2, daño);

            m_scene->removeItem(m_particle);
            delete m_particle;
            m_particle = nullptr;

            emit finishedTurn();
            return;
        }
    }

    // si sale del mapa
    if (m_particle->x() < -50 || m_particle->x() > 850) {
        m_scene->removeItem(m_particle);
        delete m_particle;
        m_particle = nullptr;

        emit finishedTurn();
    }
}
