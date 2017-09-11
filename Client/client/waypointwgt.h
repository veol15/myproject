#ifndef WAYPOINTWGT_H
#define WAYPOINTWGT_H

#include <QWidget>
#include "waypointitemwgt.h"
namespace Ui {
class WayPointWgt;
}

class WayPointWgt : public QWidget
{
    Q_OBJECT

public:
    explicit WayPointWgt(QWidget *parent = 0);
    ~WayPointWgt();

    void addWPItem(WayPointItemWgt *wgt);
    void delWPItem(int num);
    void clearWPItem();

private:
    Ui::WayPointWgt *ui;
};

#endif // WAYPOINTWGT_H
