#ifndef WAYPOINTITEMWGT_H
#define WAYPOINTITEMWGT_H

#include <QWidget>
#include "waypointitem.h"
namespace Ui {
class WayPointItemWgt;
}

class WayPointItemWgt : public QWidget
{
    Q_OBJECT

public:
    explicit WayPointItemWgt(QWidget *parent = 0);
    ~WayPointItemWgt();

    void setTitle(const QString &title);
    void setInfo(mapcontrol::WayPointItem *wpItem);

private:
    Ui::WayPointItemWgt *ui;
    QString m_title;
};

#endif // WAYPOINTITEMWGT_H
