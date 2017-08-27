#include "wayopwgt.h"
#include "ui_wayopwgt.h"

WayOPWgt::WayOPWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WayOPWgt)
{
    ui->setupUi(this);
}

WayOPWgt::~WayOPWgt()
{
    delete ui;
}
