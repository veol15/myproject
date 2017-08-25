#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    m_conf = new QSettings("./data/myDemo.ini", QSettings::IniFormat);
    ui->setupUi(this);

    map = new MapWidget;
    ui->mapWgtLayoutH->addWidget(map);

    map->SetShowUAV(true);
    map->SetShowHome(true);

    m_uav = map->AddUAV(0);
    connect(map, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(map_mouseMoveEvent(QMouseEvent*)));

    double  lat, lng;
    int     zoom;

    lat  = m_conf->value("mapWidget_home_lat", 30).toDouble();
    lng  = m_conf->value("mapWidget_home_lng", 104).toDouble();
    zoom = m_conf->value("lastZoom", 3).toInt();

    internals::PointLatLng p(lat, lng);
    map->SetCurrentPosition(p);
    map->SetZoom(zoom);
    map->setConf(m_conf);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::map_mouseMoveEvent(QMouseEvent *event)
{
    if( 1 )
    {
        internals::PointLatLng p;
        p = map->currentMousePosition();
        if( m_uav != NULL )
            m_uav->SetUAVPos(p, 460);
    }
}
