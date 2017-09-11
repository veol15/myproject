#include "videowgt.h"
#include "ui_videowgt.h"

VideoWgt::VideoWgt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoWgt)
{
    ui->setupUi(this);
}

VideoWgt::~VideoWgt()
{
    delete ui;
}
