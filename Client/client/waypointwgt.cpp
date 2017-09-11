#include "waypointwgt.h"
#include "ui_waypointwgt.h"

WayPointWgt::WayPointWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WayPointWgt)
{
    ui->setupUi(this);
}

WayPointWgt::~WayPointWgt()
{
    delete ui;
}

void WayPointWgt::addWPItem(WayPointItemWgt *wgt)
{
    int n = ui->waypointItemWgtLayout->count();
    ui->waypointItemWgtLayout->insertWidget(n-1,wgt);
}

void WayPointWgt::delWPItem(int num)
{
    QLayoutItem *item = ui->waypointItemWgtLayout->itemAt(num);
    if (item == nullptr)
        return;
    QWidget *wgt = item->widget();
    if (wgt == nullptr)
        return;
    ui->waypointItemWgtLayout->removeWidget(wgt);
    delete wgt;
}

void WayPointWgt::clearWPItem()
{
    int n = ui->waypointItemWgtLayout->count();
    for (int i=0; i<n-1;++i)
    {
        QLayoutItem *item = ui->waypointItemWgtLayout->itemAt(0);
        ui->waypointItemWgtLayout->removeItem(item);
        delete item->widget();
    }
}
