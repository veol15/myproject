#ifndef WAYPOINTWGT_H
#define WAYPOINTWGT_H

#include <QWidget>

namespace Ui {
class WayPointWgt;
}

class WayPointWgt : public QWidget
{
    Q_OBJECT

public:
    explicit WayPointWgt(QWidget *parent = 0);
    ~WayPointWgt();

private:
    Ui::WayPointWgt *ui;
};

#endif // WAYPOINTWGT_H
