#include "particula.h"
#include <cmath>
#include <QBrush>

particula::particula(double x, double y, double radio, double masa, const vectordes &vel)
    : m_posicion(x,y), m_velocidad(vel), m_masa(masa), m_radio(radio)
{
    // dibujamos el círculo centrado en la posición (usamos rect local y setPos como centro)
    setRect(-m_radio, -m_radio, m_radio*2, m_radio*2);
    setPos(m_posicion.x, m_posicion.y);
    setBrush(QBrush(Qt::cyan));
}

particula::~particula() {}

vectordes particula::posicion() const { return m_posicion; }
vectordes particula::velocidad() const { return m_velocidad; }
double particula::masa() const { return m_masa; }
double particula::radio() const { return m_radio; }

void particula::paso(double dt){
    m_posicion = m_posicion + m_velocidad * dt;
    // actualizar posición gráfica (posicion representa el centro)
    setPos(m_posicion.x, m_posicion.y);
}

void particula::aplicarImpulso(const vectordes &impulso){
    m_velocidad = m_velocidad + impulso / m_masa;
}

bool particula::colicionaCon(const particula &other) const {
    double dx = m_posicion.x - other.m_posicion.x;
    double dy = m_posicion.y - other.m_posicion.y;
    double dist2 = dx*dx + dy*dy;
    double R = m_radio + other.m_radio;
    return dist2 <= R*R;
}

bool particula::colicionaConRectangulo(double rx, double ry, double rw, double rh) const {
    // closest point in rect to particle center
    double cx = std::max(rx, std::min(m_posicion.x, rx+rw));
    double cy = std::max(ry, std::min(m_posicion.y, ry+rh));
    double dx = m_posicion.x - cx;
    double dy = m_posicion.y - cy;
    return dx*dx + dy*dy <= m_radio*m_radio;
}

void particula::absorber(particula &other){
    // conservación del momento lineal
    vectordes totalMomento = m_velocidad * m_masa + other.m_velocidad * other.m_masa;
    double newMass = m_masa + other.m_masa;
    vectordes newVel = totalMomento / newMass;

    // actualización de radio asumiendo área proporcional a masa
    double area1 = M_PI * m_radio * m_radio;
    double area2 = M_PI * other.m_radio * other.m_radio;
    double newArea = area1 + area2;
    double newRadius = std::sqrt(newArea / M_PI);

    m_masa = newMass;
    m_velocidad = newVel;
    m_radio = newRadius;

    // actualizar representación gráfica (rect local)
    setRect(-m_radio, -m_radio, m_radio*2, m_radio*2);
    setPos(m_posicion.x, m_posicion.y);
}

void particula::setPosicion(const vectordes &p){
    m_posicion = p;
    setPos(m_posicion.x, m_posicion.y); // actualiza el dibujo
}

void particula::setVelocidad(const vectordes &v){
    m_velocidad = v;
}
