#ifndef SIMULATION_H
#define SIMULATION_H

#include <QObject>
#include <QList>

class QGraphicsScene;
class Particula;
class QGraphicsRectItem;

class Simulation : public QObject
{
    Q_OBJECT

public:
    Simulation(QGraphicsScene *scene);

    void launch(double angle_deg, double speed, QPointF start, int currentPlayer);

signals:
    void hitPlatform(int which, double damage);
    void finishedTurn();

public slots:
    void step();

private:
    QGraphicsScene *m_scene;
    Particula *m_particle = nullptr;

    QGraphicsRectItem *platformLeft;
    QGraphicsRectItem *platformRight;

    double factor_constante = 0.15;   // lo define el estudiante
};

#endif // SIMULATION_H
