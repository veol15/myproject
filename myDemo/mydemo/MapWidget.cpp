
#include <QtCore>
#include <QtGui>
#include <QMenu>

#include "MapWidget.h"
#include <QHeaderView>
#include <QFileDialog>
#include <QInputDialog>

using namespace mapcontrol;

MapWidget::MapWidget(QWidget *parent) :
    mapcontrol::OPMapWidget(parent),
    m_pWayOPWgt(new WayOPWgt(this))
{
    m_conf = NULL;

    configuration->SetAccessMode(core::AccessMode::ServerAndCache);
    configuration->SetTileMemorySize(200);
    configuration->SetCacheLocation("./data/");

    SetZoom(4);
    SetMinZoom(4);
    SetMaxZoom(18);
    SetMapType(MapType::GoogleHybridChina);

    // set initial values
    m_bSelectArea = 0;
    m_pSelectArea1.SetLat(-9999);   m_pSelectArea1.SetLng(-9999);
    m_pSelectArea2.SetLat(-9999);   m_pSelectArea2.SetLng(-9999);

    m_homeShow = 1;
    m_homeAlt = 440;
    m_homePos.SetLat(-9999);        m_homePos.SetLng(-9999);
    m_homeSafearea = 100;

    m_flightHeight = 20;

    // setup menus
    setupMenu();
}

MapWidget::~MapWidget()
{

}

void MapWidget::setConf(QSettings *conf)
{
    m_conf = conf;

    if( m_conf == NULL ) return;

    // map type & access mode
    {
        MapType::Types              mapType;
        core::AccessMode::Types     accessMode;
        QString                     cacheLocation;

        // load settings
        accessMode    = (core::AccessMode::Types) m_conf->value("mapWidget_accessMode",
                                                                (int)(core::AccessMode::ServerAndCache)).toInt();
        mapType       = (MapType::Types) m_conf->value("mapWidget_mapType",
                                                       (int)(MapType::GoogleHybridChina)).toInt();
        cacheLocation = m_conf->value("mapWidget_cacheLocation", "./data/").toString();

        // set configurations
        configuration->SetAccessMode(accessMode);
        configuration->SetCacheLocation(cacheLocation);
        SetMapType(mapType);

        // set accessMode actions
        if( accessMode == core::AccessMode::ServerAndCache ) {
            m_actMapAccess_ServerAndCache->setChecked(true);
            m_actMapAccess_Cache->setChecked(false);
        } else if ( accessMode == core::AccessMode::CacheOnly ) {
            m_actMapAccess_ServerAndCache->setChecked(false);
            m_actMapAccess_Cache->setChecked(true);
        }
    }

    // home & safe area
    {
        m_homeShow = m_conf->value("mapWidget_home_show", m_homeShow).toInt();
        m_homePos.SetLat(m_conf->value("mapWidget_home_lat", m_homePos.Lat()).toDouble());
        m_homePos.SetLng(m_conf->value("mapWidget_home_lng", m_homePos.Lng()).toDouble());
        m_homeAlt = m_conf->value("mapWidget_home_alt", m_homeAlt).toDouble();
        m_homeSafearea = m_conf->value("mapWidget_home_safeArea", m_homeSafearea).toDouble();

        m_flightHeight = m_conf->value("mapWidget_flightHeight", m_flightHeight).toDouble();

        if( m_homeShow ) {
            this->SetShowHome(true);
            this->Home->SetCoord(m_homePos);
            this->Home->SetAltitude( (int)(m_homeAlt) );
            this->Home->SetSafeArea((int)(m_homeSafearea));

            m_actHome_ShowHide->setChecked(true);
        } else {
            m_actHome_ShowHide->setChecked(false);
        }
    }
}

void MapWidget::syncConf(void)
{
    if( m_conf == NULL ) return;

    // map type & access mode
    MapType::Types              mapType;
    core::AccessMode::Types     accessMode;

    mapType = GetMapType();
    accessMode = configuration->AccessMode();
    m_conf->setValue("mapWidget_mapType", (int)(mapType));
    m_conf->setValue("mapWidget_accessMode", (int)(accessMode));

    // home
    m_conf->setValue("mapWidget_home_show", m_homeShow);
    m_conf->setValue("mapWidget_home_lat", m_homePos.Lat());
    m_conf->setValue("mapWidget_home_lng", m_homePos.Lng());
    m_conf->setValue("mapWidget_home_alt", m_homeAlt);
    m_conf->setValue("mapWidget_home_safeArea", m_homeSafearea);

    m_conf->setValue("mapWidget_flightHeight", m_flightHeight);

    // sync to configure file
    m_conf->sync();
}

void MapWidget::setHome(internals::PointLatLng &p, double alt)
{
    m_homePos = p;
    m_homeAlt = alt;

    this->SetShowHome(true);
    this->Home->SetCoord(m_homePos);
    this->Home->SetAltitude( (int)(m_homeAlt) );
    this->Home->SetSafeArea((int)(m_homeSafearea));

    syncConf();

    ReloadMap();
}

void MapWidget::getHome(internals::PointLatLng &p, double &alt)
{
    p = m_homePos;
    alt = m_homeAlt;
}


int MapWidget::getWaypoints(AP_WPArray &wpa)
{
    QMap<int, mapcontrol::WayPointItem*> wpMap;

    // get waypoints
    wpMap  = WPAll();

    foreach(mapcontrol::WayPointItem* p, wpMap) {
        AP_WayPoint wp;

        wp.idx      = p->Number();
        wp.lng      = p->Coord().Lng();
        wp.lat      = p->Coord().Lat();
        wp.alt      = p->Altitude();
        wp.heading  = p->Heading();

        wpa.set(wp);
    }

    return 0;
}

int MapWidget::setWaypoints(AP_WPArray &wpa)
{
    return 0;
}

int MapWidget::setupMenu(void)
{
    // setup actions
    m_actMapType = new QAction(tr("Map Type"), this);
    connect(m_actMapType, SIGNAL(triggered()),
            this, SLOT(actMapType_SelectMap()));


    m_actMapAccess_ServerAndCache = new QAction(tr("ServerAndCache"), this);
    m_actMapAccess_ServerAndCache->setCheckable(true);
    m_actMapAccess_ServerAndCache->setChecked(true);
    connect(m_actMapAccess_ServerAndCache, SIGNAL(triggered()),
            this, SLOT(actMapAccess_ServerAndCache()));

    m_actMapAccess_Cache = new QAction(tr("Cache"), this);
    m_actMapAccess_Cache->setCheckable(true);
    m_actMapAccess_Cache->setChecked(false);
    connect(m_actMapAccess_Cache, SIGNAL(triggered()),
            this, SLOT(actMapAccess_Cache()));


    m_actWaypoint_add = new QAction(tr("Waypoint Add"), this);
    connect(m_actWaypoint_add, SIGNAL(triggered()),
            this, SLOT(actWaypoint_add()));
    m_actWaypoint_del = new QAction(tr("Waypoint Delete"), this);
    connect(m_actWaypoint_del, SIGNAL(triggered()),
            this, SLOT(actWaypoint_del()));
    m_actWaypoint_edit = new QAction(tr("Waypoint Edit"), this);
    connect(m_actWaypoint_edit, SIGNAL(triggered()),
            this, SLOT(actWaypoint_edit()));
    m_actWaypoint_clear = new QAction(tr("Waypoint Clear"), this);
    connect(m_actWaypoint_clear, SIGNAL(triggered()),
            this, SLOT(actWaypoint_clear()));
    m_actWaypoint_save = new QAction(tr("Waypoint Save"), this);
    connect(m_actWaypoint_save, SIGNAL(triggered()),
            this, SLOT(actWaypoint_save()));
    m_actWaypoint_load = new QAction(tr("Waypoint Load"), this);
    connect(m_actWaypoint_load, SIGNAL(triggered()),
            this, SLOT(actWaypoint_load()));

    m_actSelectArea_beg = new QAction(tr("SelectArea Begin"), this);
    connect(m_actSelectArea_beg, SIGNAL(triggered()),
            this, SLOT(actSelectArea_beg()));
    m_actSelectArea_end = new QAction(tr("SelectArea End"), this);
    connect(m_actSelectArea_end, SIGNAL(triggered()),
            this, SLOT(actSelectArea_end()));
    m_actSelectArea_clear = new QAction(tr("SelectArea Clear"), this);
    connect(m_actSelectArea_clear, SIGNAL(triggered()),
            this, SLOT(actSelectArea_clear()));

    m_actHome_Set = new QAction(tr("Set Home"), this);
    connect(m_actHome_Set, SIGNAL(triggered()),
            this, SLOT(actHome_Set()));
    m_actHome_Safearea = new QAction(tr("Set Home Safearea"), this);
    connect(m_actHome_Safearea, SIGNAL(triggered()),
            this, SLOT(actHome_Safearea()));
    m_actHome_ShowHide = new QAction(tr("Show/Hide Home"), this);
    m_actHome_ShowHide->setCheckable(true);
    m_actHome_ShowHide->setChecked(true);
    connect(m_actHome_ShowHide, SIGNAL(triggered()),
            this, SLOT(actHome_ShowHide()));

    // setup menu
    m_popupMenu = new QMenu("Menu");

    QMenu *menuAccessMode = m_popupMenu->addMenu("Access Type");
    m_popupMenu->addAction(m_actMapType);
    m_popupMenu->addSeparator();
    m_popupMenu->addAction(m_actWaypoint_add);
    m_popupMenu->addAction(m_actWaypoint_del);
    m_popupMenu->addAction(m_actWaypoint_edit);
    m_popupMenu->addAction(m_actWaypoint_clear);
    m_popupMenu->addAction(m_actWaypoint_save);
    m_popupMenu->addAction(m_actWaypoint_load);
    m_popupMenu->addSeparator();
    m_popupMenu->addAction(m_actSelectArea_beg);
    m_popupMenu->addAction(m_actSelectArea_end);
    m_popupMenu->addAction(m_actSelectArea_clear);
    m_popupMenu->addSeparator();
    m_popupMenu->addAction(m_actHome_Set);
    m_popupMenu->addAction(m_actHome_Safearea);
    m_popupMenu->addAction(m_actHome_ShowHide);

    menuAccessMode->addAction(m_actMapAccess_ServerAndCache);
    menuAccessMode->addAction(m_actMapAccess_Cache);

    return 0;
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::RightButton )
    {
        m_popupMenu->popup(event->globalPos());
    } else
    {
        mapcontrol::OPMapWidget::mousePressEvent(event);
    }
}

void MapWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton )
    {
        actWaypoint_add();
    }
    else
    {
        mapcontrol::OPMapWidget::mouseDoubleClickEvent(event);
    }
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    m_pWayOPWgt->setGeometry(this->width()-m_pWayOPWgt->width(), this->height()-m_pWayOPWgt->height(),
                             m_pWayOPWgt->width(), m_pWayOPWgt->height());
    mapcontrol::OPMapWidget::resizeEvent(event);
}
void MapWidget::paintEvent(QPaintEvent *event)
{
    mapcontrol::OPMapWidget::paintEvent(event);
}

void MapWidget::actMapType_SelectMap(void)
{
    MapTypeDlg          diag;
    core::MapType::Types    mt, mt0;

    // set current map type active
    mt0 = GetMapType();
    diag.setMapType(mt0);

    // open maptype selecting dialog
    if( QDialog::Accepted == diag.exec() ) {
        mt = diag.getMapType();

        if( mt0 == mt ) return;

        // set new map type
        SetMapType(mt);

        // update settings
        if( m_conf != NULL ) {
            m_conf->setValue("mapWidget_mapType", (int)(mt));
            m_conf->sync();
        }

        // reload map
        ReloadMap();
    }
}


void MapWidget::actMapAccess_ServerAndCache(void)
{
    m_actMapAccess_ServerAndCache->setChecked(true);
    m_actMapAccess_Cache->setChecked(false);

    configuration->SetAccessMode(core::AccessMode::ServerAndCache);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_accessMode", (int)(core::AccessMode::ServerAndCache));
        m_conf->sync();
    }
}

void MapWidget::actMapAccess_Cache(void)
{
    m_actMapAccess_ServerAndCache->setChecked(false);
    m_actMapAccess_Cache->setChecked(true);

    configuration->SetAccessMode(core::AccessMode::CacheOnly);

    if( m_conf != NULL ) {
        m_conf->setValue("mapWidget_accessMode", (int)(core::AccessMode::CacheOnly));
        m_conf->sync();
    }
}


void MapWidget::actWaypoint_add(void)
{
    internals::PointLatLng p;

    p = currentMousePosition();

    mapcontrol::WayPointItem *wp = this->WPCreate();
    wp->SetCoord(p);
    wp->SetAltitude(m_homeAlt + m_flightHeight);

    connect(wp, SIGNAL(WPEdit(int, WayPointItem*)),
            this, SLOT(actWPEdit(int, WayPointItem*)));
}

void MapWidget::actWaypoint_del(void)
{
    QList<mapcontrol::WayPointItem*>    wpList;
    int                     i, n;

    // set selected waypoints
    wpList = WPSelected();
    n = wpList.size();

    for(i=0; i<n; i++) {
        WPDelete(wpList[i]);
    }
}

void MapWidget::actWaypoint_edit(void)
{
    QMap<int, mapcontrol::WayPointItem*>    wpMap;
    QList<mapcontrol::WayPointItem*>        wpList;
    int                                     idx;

    WPEditDlg                     wpDialog;

    wpMap  = WPAll();
    wpList = WPSelected();

    wpDialog.setReferenceAltitude(m_homeAlt);

    if( wpList.size() == 1 ) {
        idx = wpList[0]->Number();

        wpDialog.setWaypoints(idx, &wpMap);
    } else {
        wpDialog.setWaypoints(-1, &wpMap);
    }

    // update edited data
    if( QDialog::Accepted == wpDialog.exec() ) {
        wpDialog.updateWaypoints();
    }
}

void MapWidget::actWaypoint_clear(void)
{
    WPDeleteAll();
    ReloadMap();
}

void MapWidget::actWPEdit(int num, WayPointItem *wp)
{
    QMap<int, mapcontrol::WayPointItem*>    wpMap;
    int                                     idx;

    WPEditDlg                     wpDialog;

    wpMap  = WPAll();
    idx    = num;

    wpDialog.setReferenceAltitude(m_homeAlt);
    wpDialog.setWaypoints(idx, &wpMap);

    if( QDialog::Accepted == wpDialog.exec() )
    {
        wpDialog.updateWaypoints();
    }
}


void MapWidget::actWaypoint_save(void)
{
    QMap<int, mapcontrol::WayPointItem*> wpMap;
    QString fname, path = "./data";

    // get waypoints
    wpMap  = WPAll();

    // get save filename
    fname = QFileDialog::getSaveFileName(this, tr("Waypoint file"),
                                         path,
                                         tr("Waypoint Files (*.wp)"));

    if( fname.size() < 1 ) return;
    if( !fname.endsWith(".wp", Qt::CaseInsensitive) ) {
        fname = fname + ".wp";
    }

    // copy to AP_WPArray
    AP_WPArray  arrWP;

    foreach(mapcontrol::WayPointItem* p, wpMap) {
        AP_WayPoint wp;

        wp.idx = p->Number();
        wp.lng = p->Coord().Lng();
        wp.lat = p->Coord().Lat();
        wp.alt = p->Altitude();
        wp.heading = p->Heading();

        arrWP.set(wp);
    }

    // save to file
    arrWP.save(fname.toStdString());
}

void MapWidget::actWaypoint_load(void)
{
    QString fname, path = "./data";

    AP_WPArray                  arrWP;
    AP_WayPointMap              *wpMap;
    AP_WayPointMap::iterator    it;
    AP_WayPoint                 *p;

    // get save filename
    fname = QFileDialog::getOpenFileName(this, tr("Waypoint file"),
                                         path,
                                         tr("Waypoint Files (*.wp)"));

    if( fname.size() < 1 ) return;

    // load AP_WPArray
    if( 0 != arrWP.load(fname.toStdString()) ) return;

    // delete exist waypoints
    this->WPDeleteAll();

    // insert to map widget
    wpMap = arrWP.getAll();

    for(it=wpMap->begin(); it!=wpMap->end(); it++) {
        p = it->second;

        // create new wp item
        internals::PointLatLng coord(p->lat, p->lng);
        mapcontrol::WayPointItem* wp = new mapcontrol::WayPointItem(coord, p->alt, map);
        wp->SetNumber(p->idx);
        wp->SetHeading(p->heading);

        // insert to widget
        WPCreate(wp);
    }

    // repaint map
    ReloadMap();
}


void MapWidget::actSelectArea_beg(void)
{
    internals::PointLatLng p;

    p = currentMousePosition();

    m_pSelectArea1.SetLat(p.Lat());
    m_pSelectArea1.SetLng(p.Lng());
}

void MapWidget::actSelectArea_end(void)
{
    internals::PointLatLng p;

    double  lat1, lat2;
    double  lng1, lng2;

    p = currentMousePosition();

    m_pSelectArea2.SetLat(p.Lat());
    m_pSelectArea2.SetLng(p.Lng());

    if( m_pSelectArea1.Lat() < -1000 || m_pSelectArea1.Lng() < -1000 )
        return;

    if( m_pSelectArea1.Lat() > m_pSelectArea2.Lat() ) {
        lat1 = m_pSelectArea1.Lat();
        lat2 = m_pSelectArea2.Lat();
    } else {
        lat1 = m_pSelectArea2.Lat();
        lat2 = m_pSelectArea1.Lat();
    }

    if( m_pSelectArea1.Lng() < m_pSelectArea2.Lng() ) {
        lng1 = m_pSelectArea1.Lng();
        lng2 = m_pSelectArea2.Lng();
    } else {
        lng1 = m_pSelectArea2.Lng();
        lng2 = m_pSelectArea1.Lng();
    }


    internals::RectLatLng rect(lat1, lng1, fabs(lng2 - lng1), fabs(lat2 - lat1));
    SetSelectedArea(rect);

    //this->update();
    ReloadMap();
}

void MapWidget::actSelectArea_clear(void)
{
    internals::RectLatLng rect;

    SetSelectedArea(rect);

    //this->update();
    ReloadMap();
}

void MapWidget::actHome_Set(void)
{
    internals::PointLatLng p;

    p = currentMousePosition();

    // get home altitude
    bool ok = true;
    double alt = QInputDialog::getDouble(this,
                                         tr("Input altitude of home"),
                                         tr("Altitude:"),
                                         m_homeAlt, -1000, 10000,
                                         1,
                                         &ok);
    if ( ok ) m_homeAlt = alt;
    else return;

    // set home position, altitude, safearea
    this->SetShowHome(true);
    this->Home->SetCoord(p);
    this->Home->SetAltitude((int)(m_homeAlt));
    this->Home->SetSafeArea((int)(m_homeSafearea));
    m_homePos = p;

    // write configure
    syncConf();

    // refresh map
    ReloadMap();

    // send home set signal
    emit HomeSet();
}

void MapWidget::actHome_Safearea(void)
{
    bool ok = true;
    double d = QInputDialog::getDouble(this,
                                       tr("Set safe area"),
                                       tr("Radius (m):"),
                                       m_homeSafearea, 20, 50000,
                                       1,
                                       &ok);
    if ( ok ) m_homeSafearea = d;
    else return;

    // write configure
    syncConf();

    if( Home != NULL ) {
        this->Home->SetSafeArea((int)(m_homeSafearea));

        // refresh map
        ReloadMap();
    }
}

void MapWidget::actHome_ShowHide(void)
{
    if( m_homeShow ) m_homeShow = 0;
    else             m_homeShow = 1;

    if( m_homeShow ) {
        setHome(m_homePos, m_homeAlt);

        m_actHome_ShowHide->setChecked(true);
    } else {
        this->SetShowHome(false);

        m_actHome_ShowHide->setChecked(false);
    }

    // write configure
    syncConf();
}

