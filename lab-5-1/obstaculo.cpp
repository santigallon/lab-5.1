#include "obstaculo.h"
#include <QBrush>

obstaculo::obstaculo(double x, double y, double w, double h)
    : m_x(x), m_y(y), m_w(w), m_h(h)
{
    // definimos rect local en (0,0) y ponemos la posición en scene con setPos
    setRect(0,0,m_w,m_h);
    setPos(m_x, m_y);
    setBrush(Qt::darkGray);
}

double obstaculo::x() const { return m_x; }
double obstaculo::y() const { return m_y; }
double obstaculo::w() const { return m_w; }
double obstaculo::h() const { return m_h; }
