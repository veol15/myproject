#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    m_conf = new QSettings(QCoreApplication::applicationDirPath() + "/data/myDemo.ini", QSettings::IniFormat);
    ui->setupUi(this);

    m_pMapWgt = new MapWidget(this);

    m_pMapWgt->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->hLayout2->insertWidget(0,m_pMapWgt);
    ui->hLayout2->insertWidget(1,&m_wayPointWgt);

    m_pMapWgt->SetShowUAV(true);
    m_pMapWgt->SetShowHome(true);
    m_pMapWgt->SetUseOpenGL(false);

    m_uav = m_pMapWgt->AddUAV(0);
    connect(m_pMapWgt, SIGNAL(mouseMove(QMouseEvent*)),
            this, SLOT(map_mouseMoveEvent(QMouseEvent*)));

    double  lat;
    double  lng;
    int     zoom;

    lat  = m_conf->value("mapWidget_home_lat", 30).toDouble();
    lng  = m_conf->value("mapWidget_home_lng", 104).toDouble();
    zoom = m_conf->value("lastZoom", 3).toInt();

    internals::PointLatLng p(lat, lng);
    m_pMapWgt->SetCurrentPosition(p);
    m_pMapWgt->SetZoom(zoom);
    m_pMapWgt->setConf(m_conf);

    setCurWorkMode(returnVoyage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::map_mouseMoveEvent(QMouseEvent *event)
{
    if(0)
    {
        internals::PointLatLng p;
        p = m_pMapWgt->currentMousePosition();
        if( m_uav != NULL )
            m_uav->SetUAVPos(p, 460);
    }
}

void MainWindow::on_cacheMapBtn_clicked()
{
    internals::RectLatLng  rect;
    rect = m_pMapWgt->SelectedArea();
    if( rect.IsEmpty() )
    {
        QMessageBox msgbox(QMessageBox::Warning,
                           "Warning", "Please select an area first!");
        msgbox.exec();
        return;
    }
    m_pMapWgt->RipMap();
}

void MainWindow::on_returnVoyageBtn_clicked()
{
    setCurWorkMode(returnVoyage);
}

void MainWindow::on_circleBtn_clicked()
{
    setCurWorkMode(circle);
}

void MainWindow::on_freedomBtn_clicked()
{
    setCurWorkMode(freedom);
}

void MainWindow::on_cruiseBtn_clicked()
{
    setCurWorkMode(cruise);
}

void MainWindow::on_hoverBtn_clicked()
{
    setCurWorkMode(hover);
}

void MainWindow::setCurWorkMode(MainWindow::workMode mode)
{
    switch (mode)
    {
    case returnVoyage:
    {
        hideAllWorkModeWgt();
        ui->returnVoyageWgt->setVisible(true);
    }
        break;
    case circle:
    {
        hideAllWorkModeWgt();
        ui->circleWgt->setVisible(true);
    }
        break;
    case cruise:
    {
        hideAllWorkModeWgt();
        ui->cruiseWgt->setVisible(true);
    }
        break;
    case hover:
    {
        hideAllWorkModeWgt();
        ui->hoverWgt->setVisible(true);
    }
        break;
    case freedom:
    {
        hideAllWorkModeWgt();
        ui->freedomWgt->setVisible(true);
    }
        break;
    default:
        break;
    }
}

void MainWindow::hideAllWorkModeWgt()
{
    ui->returnVoyageWgt->setVisible(false);
    ui->circleWgt->setVisible(false);
    ui->cruiseWgt->setVisible(false);
    ui->hoverWgt->setVisible(false);
    ui->freedomWgt->setVisible(false);
}

void MainWindow::on_mainPageBtn_clicked(bool checked)
{
    if (checked)
    {
        ui->bottomWgt->setVisible(false);
        //m_pMapWgt->setVisible(true);
        ui->mainPageBtn->setText(tr("航路规划"));
    }
    else
    {
        ui->bottomWgt->setVisible(true);
        //m_pMapWgt->setVisible(false);
        ui->mainPageBtn->setText(tr("主界面"));
    }
}
