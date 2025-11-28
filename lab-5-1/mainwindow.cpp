#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

#include "simulation.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Creamos la scene y la view y las insertamos en el centralWidget
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,800,600);

    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    // Layout dentro del centralWidget
    QWidget *c = ui->centralwidget;
    QVBoxLayout *lay = new QVBoxLayout(c);
    lay->setContentsMargins(0,0,0,0);
    lay->addWidget(view);

    // Botones de control simples
    QWidget *controls = new QWidget;
    QHBoxLayout *h = new QHBoxLayout(controls);
    QPushButton *startBtn = new QPushButton("Iniciar");
    QPushButton *stopBtn  = new QPushButton("Detener");
    h->addWidget(startBtn);
    h->addWidget(stopBtn);
    lay->addWidget(controls);

    // Crear simulación — ahora recibe la scene y tamaño
    Simulation *sim = new Simulation(scene, 800, 600);
    sim->setGravedad(0.0);
    sim->setRestitucion(0.8);

    // Añadir partículas (ejemplo)
    sim->agregarParticula(new particula(100,100,10,2.0, vectordes(120,30)));
    sim->agregarParticula(new particula(200,150,12,3.0, vectordes(-80,10)));
    sim->agregarParticula(new particula(400,300,8,1.2, vectordes(0,-50)));
    sim->agregarParticula(new particula(600,100,15,4.0, vectordes(-60,40)));

    // Añadir obstáculos
    sim->agregarObstaculo(new obstaculo(300,250,50,80));
    sim->agregarObstaculo(new obstaculo(500,120,100,40));
    sim->agregarObstaculo(new obstaculo(150,400,60,60));
    sim->agregarObstaculo(new obstaculo(650,350,80,50));

    // Timer para pasos de simulación
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [sim](){ sim->paso(0.001); });

    connect(startBtn, &QPushButton::clicked, [timer](){ timer->start(16); });
    connect(stopBtn,  &QPushButton::clicked, [timer](){ timer->stop(); });

    // opcional: comenzar automáticamente
    // timer->start(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}
