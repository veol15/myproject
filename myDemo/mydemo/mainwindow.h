#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "MapWidget.h"
#include "waypointwgt.h"
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
private slots:
    void on_cacheMapBtn_clicked();

private:
    Ui::MainWindow *ui;
    MapWidget *map;
    WayPointWgt m_wayPointWgt;

    mapcontrol::UAVItem         *m_uav;

};

#endif // MAINWINDOW_H
