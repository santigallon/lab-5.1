#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class QPushButton;
class QLabel;
class QDoubleSpinBox;

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void onLaunchClicked();
    void onNextTurnClicked();

private:
    void setupScene();
    void setupUI();

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;

    QPushButton *m_launchBtn;
    QPushButton *m_nextBtn;
    QDoubleSpinBox *m_angleSpin;
    QDoubleSpinBox *m_speedSpin;
    QLabel *m_turnLabel;

    int m_currentPlayer = 1;

    // NUEVA LÍNEA OBLIGATORIA
    Simulation *m_sim;

    // Items gráficos
    QGraphicsRectItem *platformLeft;
    QGraphicsRectItem *platformRight;
    QGraphicsTextItem *lifeLeft;
    QGraphicsTextItem *lifeRight;
    QGraphicsEllipseItem *playerLeft;
    QGraphicsEllipseItem *playerRight;
    QGraphicsRectItem *catapultLeft;
    QGraphicsRectItem *catapultRight;
};

#endif // GAMEWINDOW_H
