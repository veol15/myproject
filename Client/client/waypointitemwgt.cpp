#include "waypointitemwgt.h"
#include "ui_waypointitemwgt.h"

WayPointItemWgt::WayPointItemWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WayPointItemWgt)
{
    ui->setupUi(this);
}

WayPointItemWgt::~WayPointItemWgt()
{
    delete ui;
}

void WayPointItemWgt::setTitle(const QString &title)
{
    m_title = title;
    ui->label->setText(m_title);
}

void WayPointItemWgt::setInfo(mapcontrol::WayPointItem *wpItem)
{
    ui->latLabel->setText(QString::number(wpItem->Coord().Lat()));
    ui->lngLabel->setText(QString::number(wpItem->Coord().Lng()));
    ui->altLabel->setText(QString::number(wpItem->Altitude()));
    setTitle(QString("航点%1").arg(wpItem->Number()));
}

void WayPointItemWgt::on_actionCB_currentIndexChanged(int index)
{
    ui->actSWgt->setCurrentIndex(index);
}
