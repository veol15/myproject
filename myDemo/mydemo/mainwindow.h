#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "MapWidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void map_mouseMoveEvent(QMouseEvent *event);
protected:
    QSettings   *m_conf;
private:
    Ui::MainWindow *ui;
    MapWidget *map;

    mapcontrol::UAVItem         *m_uav;

};

#endif // MAINWINDOW_H
