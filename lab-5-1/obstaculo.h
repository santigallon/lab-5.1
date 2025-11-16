#ifndef OBSTACULO_H
#define OBSTACULO_H

class obstaculo
{
public:
    obstaculo(double x, double y, double w, double h);
    double x() const; double y() const; double w() const; double h() const;
private:
    double m_x, m_y, m_w, m_h;
};

#endif // OBSTACULO_H
