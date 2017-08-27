#ifndef WAYOPWGT_H
#define WAYOPWGT_H

#include <QWidget>

namespace Ui {
class WayOPWgt;
}

class WayOPWgt : public QWidget
{
    Q_OBJECT

public:
    explicit WayOPWgt(QWidget *parent = 0);
    ~WayOPWgt();

private:
    Ui::WayOPWgt *ui;
};

#endif // WAYOPWGT_H
