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
}

void WayPointItemWgt::on_actionCB_currentIndexChanged(int index)
{
    switch (index)
    {
    case mapcontrol::WayPointItem::wayPoint:

        break;
    case mapcontrol::WayPointItem::changeAltitude:

        break;
    case mapcontrol::WayPointItem::cycle:

        break;
    case mapcontrol::WayPointItem::returnVoyage:

        break;
    case mapcontrol::WayPointItem::jump:

        break;
    default:
        break;
    }
}
