#ifndef WPEDITDLG_H
#define WPEDITDLG_H

#include <QDialog>
#include <opmapwidget.h>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QCheckBox>
using namespace mapcontrol;

class WPEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit WPEditDlg(QWidget *parent = 0);

    void setupUi(void);


    int setWaypoints(int idx,
                     QMap<int, mapcontrol::WayPointItem*> *wpMap,
                     int heightAltitude = 1);
    int setWaypoints_(int idx,
                      QMap<int, mapcontrol::WayPointItem*> *wpMap,
                      int heightAltitude);

    int setTableItem(int ri, int ci, QString s);
    int updateWaypoints(void);

    void setReferenceAltitude(double alt);
    double getReferenceAltitude(void);

protected slots:
    void act_cbAllWaypoints_clicked(bool s);
    void act_cbHeightAltitude_clicked(bool s);

private:
    QVBoxLayout         *verticalLayout;
    QTableWidget        *tableWaypoints;
    QCheckBox           *cbAllWaypoints;
    QCheckBox           *cbHeightAltitude;
    QDialogButtonBox    *buttonBox;

    QMap<int, mapcontrol::WayPointItem*>    *m_wpMap;       // waypoints map
    int                                     m_wpIdx;        // active waypoint index

    int                 m_bHeightAltitude;                  // height or altitude
    double              m_referenceAltitude;                // home altitude (reference)

    QColor              clCL1, clCL2;
    QColor              clB1, clB2;
    int                 fontSize;
    int                 rowHeight;
};

#endif // WPEDITDLG_H
