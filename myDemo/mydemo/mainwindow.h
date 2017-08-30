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
enum workMode
{
    returnVoyage,   //返航
    circle,         //盘旋
    cruise,         //巡航
    hover,          //悬停
    freedom         //自主
};

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void map_mouseMoveEvent(QMouseEvent *event);
protected:
    QSettings   *m_conf;
private slots:
    void on_cacheMapBtn_clicked();

    void on_returnVoyageBtn_clicked();

    void on_circleBtn_clicked();

    void on_freedomBtn_clicked();

    void on_cruiseBtn_clicked();

    void on_hoverBtn_clicked();

    void on_mainPageBtn_clicked(bool checked);

private:
    void setCurWorkMode(workMode mode);
    void hideAllWorkModeWgt();
private:
    Ui::MainWindow *ui;
    MapWidget *m_pMapWgt;
    WayPointWgt m_wayPointWgt;

    mapcontrol::UAVItem         *m_uav;

};

#endif // MAINWINDOW_H
