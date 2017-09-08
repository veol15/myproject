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

void WayPointWgt::setWPItem(WayPointItemWgt *wgt)
{
    int n = ui->waypointItemWgtLayout->count();
    ui->waypointItemWgtLayout->insertWidget(n-1,wgt);
}
