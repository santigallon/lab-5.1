#ifndef VECTORDES_H
#define VECTORDES_H

#include <cmath>

struct vectordes {
    double x, y;
    vectordes(): x(0), y(0) {}
    vectordes(double _x, double _y): x(_x), y(_y) {}
    vectordes operator+(const vectordes &o) const { return vectordes(x+o.x, y+o.y); }
    vectordes operator-(const vectordes &o) const { return vectordes(x-o.x, y-o.y); }
    vectordes operator*(double s) const { return vectordes(x*s, y*s); }
    vectordes operator/(double s) const { return vectordes(x/s, y/s); }
};

inline double punto(const vectordes &a, const vectordes &b){ return a.x*b.x + a.y*b.y; }
inline double longitud(const vectordes &v){ return std::sqrt(v.x*v.x + v.y*v.y); }

#endif // VECTORDES_H
