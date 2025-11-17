#include "simulation.h"
#include <cmath>

Simulation::Simulation(QGraphicsScene *scene)
    : m_scene(scene)
{}

void Simulation::setBounds(double width, double height){ m_w = width; m_h = height; }
void Simulation::setGravity(double g){ m_g = g; }
void Simulation::setRestitucionObstaculo(double e){ m_e = e; }

void Simulation::agregarParticula(particula *p){
    m_particulas.push_back(p);
    m_scene->addItem(p);
}

void Simulation::agregarObstaculo(obstaculo *o){
    m_obstaculos.push_back(o);
    m_scene->addItem(o);
}

void Simulation::paso(double dt){
    // aplicar gravedad y mover
    for(auto p : m_particulas){
        if(m_g != 0.0){
            p->aplicarImpulso(vectordes(0, p->masa() * m_g * dt));
        }
        p->paso(dt);
    }

    // colisiones contra paredes
    for(auto p : m_particulas) manejarPared(*p);

    // colisiones contra obstáculos
    for(auto p : m_particulas){
        for(auto o : m_obstaculos) manejarObstaculo(*p, *o);
    }

    // colisiones entre partículas (merging)
    manejarColisionesParticulas();
}

void Simulation::manejarPared(particula &p){
    vectordes pos = p.posicion();
    vectordes vel = p.velocidad();
    double r = p.radio();

    if(pos.x - r < 0){ pos.x = r; vel.x = -vel.x; }
    if(pos.x + r > m_w){ pos.x = m_w - r; vel.x = -vel.x; }
    if(pos.y - r < 0){ pos.y = r; vel.y = -vel.y; }
    if(pos.y + r > m_h){ pos.y = m_h - r; vel.y = -vel.y; }


    vectordes dv = vel - p.velocidad();
    p.aplicarImpulso(dv * p.masa());

    p.aplicarImpulso(vectordes(0,0));
}

void Simulation::manejarObstaculo(particula &p, obstaculo &o){
    if(!p.colicionaConRectangulo(o.x(), o.y(), o.w(), o.h())) return;

    vectordes vel = p.velocidad();

    double left = std::abs(p.posicion().x - o.x());
    double right = std::abs(p.posicion().x - (o.x()+o.w()));
    double top = std::abs(p.posicion().y - o.y());
    double bottom = std::abs(p.posicion().y - (o.y()+o.h()));

    double mn = std::min(std::min(left,right), std::min(top,bottom));

    if(mn == left || mn == right) vel.x = -m_e * vel.x;
    else vel.y = -m_e * vel.y;

    vectordes dv = vel - p.velocidad();
    p.aplicarImpulso(dv * p.masa());
}

void Simulation::manejarColisionesParticulas(){
    for(size_t i=0;i<m_particulas.size();++i){
        for(size_t j=i+1;j<m_particulas.size();++j){
            if(m_particulas[i]->colicionaCon(*m_particulas[j])){
                m_particulas[i]->absorber(*m_particulas[j]);
                m_scene->removeItem(m_particulas[j]);
                delete m_particulas[j];
                m_particulas.erase(m_particulas.begin() + j);
                --j;
            }
        }
    }
}
