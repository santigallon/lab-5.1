#include "gamewindow.h"
#include "simulation.h"
#include "particula.h"

#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QMessageBox>
#include <QtMath>
#include <QDebug>

// --------------------------------------------------
// CONSTRUCTOR
// --------------------------------------------------
GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupScene();

    // Crear simulación (usa la escena ya creada)
    m_sim = new Simulation(m_scene);

    // Timer de simulación: llama al slot step() de Simulation
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, m_sim, &Simulation::step);
    timer->start(16); // ~60 FPS

    // conectar señales de la simulación
    connect(m_sim, &Simulation::hitPlatform,
            this, [this, timer](int which, double damage){
                // leer vida actual desde el texto y restar damage
                if (which == 1) {
                    bool ok = false;
                    int v = lifeLeft->toPlainText().toInt(&ok);
                    if (!ok) v = 100;
                    int nv = qMax(0, v - qRound(damage));
                    lifeLeft->setPlainText(QString::number(nv));

                    // si llegó a 0 -> gana el jugador contrario (2)
                    if (nv == 0) {
                        QMessageBox::information(this, "Victoria",
                                                 QString("¡Jugador %1 ha ganado!").arg(2));
                        timer->stop();
                    }
                } else {
                    bool ok = false;
                    int v = lifeRight->toPlainText().toInt(&ok);
                    if (!ok) v = 100;
                    int nv = qMax(0, v - qRound(damage));
                    lifeRight->setPlainText(QString::number(nv));

                    if (nv == 0) {
                        QMessageBox::information(this, "Victoria",
                                                 QString("¡Jugador %1 ha ganado!").arg(1));
                        timer->stop();
                    }
                }
            });

    // Cuando la simulación indique que terminó el turno, cambiamos turno
    connect(m_sim, &Simulation::finishedTurn, this, &GameWindow::onNextTurnClicked);
}


// --------------------------------------------------
GameWindow::~GameWindow() {}


// --------------------------------------------------
// INTERFAZ (UI)
// --------------------------------------------------
void GameWindow::setupUI()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 800, 400);

    m_view = new QGraphicsView(m_scene);
    m_view->setRenderHint(QPainter::Antialiasing);

    // SPINBOXES
    m_angleSpin = new QDoubleSpinBox;
    m_angleSpin->setRange(0, 90);
    m_angleSpin->setValue(45);

    m_speedSpin = new QDoubleSpinBox;
    m_speedSpin->setRange(0, 2000);
    m_speedSpin->setValue(600);

    // BOTONES
    m_launchBtn = new QPushButton("Lanzar");
    m_nextBtn   = new QPushButton("Siguiente turno");

    // LABEL DE TURNO
    m_turnLabel = new QLabel("Turno: Jugador 1");

    // LAYOUT DE CONTROLES
    QHBoxLayout *controls = new QHBoxLayout;
    controls->addWidget(new QLabel("Ángulo:"));
    controls->addWidget(m_angleSpin);
    controls->addWidget(new QLabel("Velocidad:"));
    controls->addWidget(m_speedSpin);
    controls->addWidget(m_launchBtn);
    controls->addWidget(m_nextBtn);
    controls->addWidget(m_turnLabel);

    // LAYOUT PRINCIPAL
    QWidget *central = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    mainLayout->addWidget(m_view);
    mainLayout->addLayout(controls);

    setCentralWidget(central);

    // CONEXIONES
    connect(m_launchBtn, &QPushButton::clicked, this, &GameWindow::onLaunchClicked);
    connect(m_nextBtn,   &QPushButton::clicked, this, &GameWindow::onNextTurnClicked);
}


// --------------------------------------------------
// ESCENA (VISUAL COMO EL PDF)
// --------------------------------------------------
void GameWindow::setupScene()
{
    // PLATAFORMAS (150x50 tal como definiste en otras implementaciones)
    platformLeft  = m_scene->addRect(50, 250, 150, 50, QPen(), QBrush(Qt::lightGray));
    platformRight = m_scene->addRect(600, 250, 150, 50, QPen(), QBrush(Qt::lightGray));

    // VIDA (texto sobre la plataforma)
    lifeLeft = m_scene->addText("100");
    lifeLeft->setPos(110, 255);

    lifeRight = m_scene->addText("100");
    lifeRight->setPos(660, 255);

    // PERSONAJES debajo de las plataformas
    playerLeft  = m_scene->addEllipse(110, 300, 30, 30, QPen(), QBrush(Qt::blue));
    playerRight = m_scene->addEllipse(660, 300, 30, 30, QPen(), QBrush(Qt::red));

    // CATAPULTAS (representación simple)
    catapultLeft  = m_scene->addRect(50, 220, 40, 10, QPen(), QBrush(Qt::black));
    catapultRight = m_scene->addRect(710, 220, 40, 10, QPen(), QBrush(Qt::black));
}


// --------------------------------------------------
// LANZAMIENTO: delega en Simulation::launch(...)
// --------------------------------------------------
void GameWindow::onLaunchClicked()
{
    double angleDeg = m_angleSpin->value();
    double speed    = m_speedSpin->value();

    QPointF spawn;
    if (m_currentPlayer == 1) {
        // un poco encima/adelante de la catapulta izquierda
        spawn = QPointF(90, 220);
    } else {
        // catapulta derecha
        spawn = QPointF(710, 220);
    }

    // delegamos en Simulation (usa la firma que está en tu header)
    m_sim->launch(angleDeg, speed, spawn, m_currentPlayer);

    qDebug("Jugador %d lanzó: ángulo=%.2f velocidad=%.2f", m_currentPlayer, angleDeg, speed);
}


// --------------------------------------------------
// CAMBIO DE TURNO (manualmente o por finishedTurn)
// --------------------------------------------------
void GameWindow::onNextTurnClicked()
{
    m_currentPlayer = (m_currentPlayer == 1 ? 2 : 1);
    m_turnLabel->setText(QString("Turno: Jugador %1").arg(m_currentPlayer));
    qDebug("Turno cambiado a %d", m_currentPlayer);
}
