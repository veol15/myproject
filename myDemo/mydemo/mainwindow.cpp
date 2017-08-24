#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    map = new MapWidget;
    map->SetShowUAV(true);
    map->SetShowHome(true);

    ui->mapWgtLayoutH->addWidget(map);
}

MainWindow::~MainWindow()
{
    delete ui;
}
