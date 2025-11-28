#include "obstaculo.h"
#include <QBrush>

Obstaculo::Obstaculo(qreal x, qreal y, qreal w, qreal h, qreal resistance, int owner)
    : QGraphicsRectItem(0,0,w,h), m_resistance(resistance), m_owner(owner)
{
    setPos(x, y);
    setBrush(QBrush(Qt::darkGray));
}

void Obstaculo::applyDamage(qreal d){
    m_resistance -= d;
    if(m_resistance <= 0){
        setBrush(QBrush(Qt::red));
        setOpacity(0.4);
    }
}
