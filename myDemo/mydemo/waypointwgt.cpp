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
