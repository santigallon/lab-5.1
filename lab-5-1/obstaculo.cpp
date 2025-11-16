#include "obstaculo.h"


obstaculo::obstaculo(double x, double y, double w, double h)
    : m_x(x), m_y(y), m_w(w), m_h(h) {}

double obstaculo::x() const { return m_x; }
double obstaculo::y() const { return m_y; }
double obstaculo::w() const { return m_w; }
double obstaculo::h() const { return m_h; }
