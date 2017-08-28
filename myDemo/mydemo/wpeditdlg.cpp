#include "wpeditdlg.h"
#include <QHeaderView>

WPEditDlg::WPEditDlg(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("Waypoints Edit");

    m_wpMap  = NULL;
    m_wpIdx  = 0;

    clCL1 = QColor(0x00, 0x00, 0xFF);
    clCL2 = QColor(0x00, 0x00, 0x00);
    clB1  = QColor(0xFF, 0xFF, 0xFF);
    clB2  = QColor(0xE8, 0xE8, 0xE8);

    fontSize  = 10;
    rowHeight = 24;

    m_bHeightAltitude = 1;
    m_referenceAltitude = 440.0;

    setupUi();
}

int WPEditDlg::setWaypoints(int idx, QMap<int, mapcontrol::WayPointItem*> *wpMap,
                                      int heightAltitude)
{
    m_wpIdx  = idx;
    m_wpMap  = wpMap;
    m_bHeightAltitude = heightAltitude;

    // set height/altitude checkbox
    if( heightAltitude ) {
        cbHeightAltitude->setCheckState(Qt::Checked);
    } else {
        cbHeightAltitude->setCheckState(Qt::Unchecked);
    }

    // if edit all waypoints then disable checkbox of all waypoints
    if( m_wpIdx < 0 ) cbAllWaypoints->setDisabled(true);

    // set table items
    setWaypoints_(idx, wpMap, heightAltitude);

    return 0;
}

int WPEditDlg::setWaypoints_(int idx,
                                       QMap<int, mapcontrol::WayPointItem*> *wpMap,
                                       int heightAltitude)
{
    if( heightAltitude ) {
        tableWaypoints->horizontalHeaderItem(1)->setText("Height");
    } else {
        tableWaypoints->horizontalHeaderItem(1)->setText("Altitude");
    }

    // clear old contents
    tableWaypoints->clearContents();

    // insert items
    double h;

    if( idx >= 0 ) {
        cbAllWaypoints->setCheckState(Qt::Unchecked);
        tableWaypoints->setRowCount(1);

        mapcontrol::WayPointItem *item;
        item = wpMap->value(idx);

        if( heightAltitude ) h = item->Altitude() - m_referenceAltitude;
        else                 h = item->Altitude();

        setTableItem(0, 0, QString("%1").arg(item->Number()));
        setTableItem(0, 1, QString("%1").arg(h));
        setTableItem(0, 2, QString("%1").arg(item->Heading()));

        tableWaypoints->item(0, 0)->setFlags(Qt::ItemIsSelectable);
        tableWaypoints->item(0, 2)->setToolTip("0:North, 90:East, 180:South, 270:West");
    } else {
        cbAllWaypoints->setCheckState(Qt::Checked);

        QList<int> ids = wpMap->keys();
        int        ri = 0;

        tableWaypoints->setRowCount(ids.size());

        foreach(int i, ids) {
            mapcontrol::WayPointItem *item;
            item = wpMap->value(i);

            if( heightAltitude ) h = item->Altitude() - m_referenceAltitude;
            else                 h = item->Altitude();

            setTableItem(ri, 0, QString("%1").arg(item->Number()));
            setTableItem(ri, 1, QString("%1").arg(h));
            setTableItem(ri, 2, QString("%1").arg(item->Heading()));

            tableWaypoints->item(ri, 0)->setFlags(Qt::ItemIsSelectable);
            tableWaypoints->item(ri, 2)->setToolTip("0:North, 90:East, 180:South, 270:West");

            ri++;
        }
    }

    return 0;
}

int WPEditDlg::setTableItem(int ri, int ci, QString s)
{
    if( tableWaypoints->item(ri, ci) != NULL ) {
        tableWaypoints->item(ri, ci)->setText(s);
    } else {
        QTableWidgetItem* item = new QTableWidgetItem();
        item->setText(s);

        item->setTextColor(clCL2);
        if( ri % 2 == 0 ) item->setBackgroundColor(clB1);
        else              item->setBackgroundColor(clB2);

        item->setFont(QFont("", fontSize));

        tableWaypoints->setItem(ri, ci, item);
        tableWaypoints->setRowHeight(ri, rowHeight);
    }

    return 0;
}

int WPEditDlg::updateWaypoints(void)
{
    int     i, n;
    int     heightAltitude;

    int     idx;
    double  alt, heading;

    if( cbHeightAltitude->checkState() == Qt::Checked )
        heightAltitude = 1;
    else
        heightAltitude = 0;

    n = tableWaypoints->rowCount();
    for(i=0; i<n; i++) {
        idx     = tableWaypoints->item(i, 0)->text().toInt();
        alt     = tableWaypoints->item(i, 1)->text().toDouble();
        heading = tableWaypoints->item(i, 2)->text().toDouble();

        if( heightAltitude ) alt += m_referenceAltitude;

        m_wpMap->value(idx)->SetAltitude(alt);
        m_wpMap->value(idx)->SetHeading(heading);
    }

    return 0;
}

void WPEditDlg::setReferenceAltitude(double alt)
{
    m_referenceAltitude = alt;
}

double WPEditDlg::getReferenceAltitude(void)
{
    return m_referenceAltitude;
}

void WPEditDlg::setupUi(void)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("Waypoint_EditDialog"));
    this->resize(695, 439);

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    // waypoint edit table
    tableWaypoints = new QTableWidget(this);
    tableWaypoints->setObjectName(QString::fromUtf8("tableWaypoints"));
    tableWaypoints->verticalHeader()->hide();
    //tableWaypoints->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    tableWaypoints->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    tableWaypoints->setColumnCount(3);
    QStringList slHeader;
    slHeader.append("Index");
    slHeader.append("Altitude");
    slHeader.append("Heading");
    tableWaypoints->setHorizontalHeaderLabels(slHeader);

    // all waypoint checkbox
    cbAllWaypoints = new QCheckBox(this);
    cbAllWaypoints->setObjectName(QString::fromUtf8("cbAllWaypoints"));
    cbAllWaypoints->setText("All Waypoints");
    connect(cbAllWaypoints, SIGNAL(clicked(bool)),
            this, SLOT(act_cbAllWaypoints_clicked(bool)));

    // height / altitude checkbox
    cbHeightAltitude = new QCheckBox(this);
    cbHeightAltitude->setObjectName(QString::fromUtf8("cbHeightAltitude"));
    cbHeightAltitude->setText("Height / Altitude");
    connect(cbHeightAltitude, SIGNAL(clicked(bool)),
            this, SLOT(act_cbHeightAltitude_clicked(bool)));

    // ok/cancle buttonbox
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    verticalLayout->addWidget(tableWaypoints);
    verticalLayout->addWidget(cbAllWaypoints);
    verticalLayout->addWidget(cbHeightAltitude);
    verticalLayout->addWidget(buttonBox);


    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);
}

void WPEditDlg::act_cbAllWaypoints_clicked(bool s)
{
    if( cbAllWaypoints->checkState() == Qt::Checked ) {
        setWaypoints_(-1, m_wpMap, m_bHeightAltitude);
    } else{
        setWaypoints_(m_wpIdx, m_wpMap, m_bHeightAltitude);
    }
}

void WPEditDlg::act_cbHeightAltitude_clicked(bool s)
{
    if( cbHeightAltitude->checkState() == Qt::Checked ) {
        m_bHeightAltitude = 1;
    } else {
        m_bHeightAltitude = 0;
    }

    if( cbAllWaypoints->checkState() == Qt::Checked ) {
        setWaypoints_(-1, m_wpMap, m_bHeightAltitude);
    } else {
        setWaypoints_(m_wpIdx, m_wpMap, m_bHeightAltitude);
    }
}
