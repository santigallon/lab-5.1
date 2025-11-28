#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>

class Obstaculo : public QGraphicsRectItem {
public:
    Obstaculo(qreal x, qreal y, qreal w, qreal h, qreal resistance, int owner);

    qreal resistance() const { return m_resistance; }
    void applyDamage(qreal d);
    bool destroyed() const { return m_resistance <= 0; }

    int owner() const { return m_owner; }

    bool notified() const { return m_notified; }
    void setNotified(bool v) { m_notified = v; }

private:
    qreal m_resistance;
    int m_owner;
    bool m_notified = false;
};

#endif // OBSTACULO_H
