/**
******************************************************************************
*
* @file       opmapwidget.h
* @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
* @brief      The Map Widget, this is the part exposed to the user
* @see        The GNU Public License (GPL) Version 3
* @defgroup   OPMapWidget
* @{
*
*****************************************************************************/

/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef OPMAPWIDGET_H
#define OPMAPWIDGET_H


#include <QObject>
#include <QtOpenGL/QGLWidget>

// Qt4 or Qt5
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QGraphicsView>
#else
#include <QtWidgets/QGraphicsView>
#endif

#include "../mapwidget/mapgraphicitem.h"
#include "../core/geodecoderstatus.h"
#include "../core/maptype.h"
#include "../core/languagetype.h"
#include "../core/diagnostics.h"
#include "omapconfiguration.h"
#include "waypointitem.h"
#include "QtSvg/QGraphicsSvgItem"
#include "uavitem.h"
#include "gpsitem.h"
#include "homeitem.h"
#include "waypointlineitem.h"
#include "mapripper.h"
#include "uavtrailtype.h"

namespace mapcontrol {

class UAVItem;
class GPSItem;
class HomeItem;

/**
    * @brief Collection of static functions to help dealing with various enums used
    *       Contains functions for enumToString conversio, StringToEnum, QStringList of enum values...
    *
    * @class Helper opmapwidget.h "opmapwidget.h"
    */
class Helper
{
public:
    static MapType::Types MapTypeFromString(QString const& value){return MapType::TypeByStr(value);}
    static QString StrFromMapType(MapType::Types const& value){return MapType::StrByType(value);}
    static QStringList MapTypes(){return MapType::TypesList();}
    static GeoCoderStatusCode::Types GeoCoderStatusCodeFromString(QString const& value) {
        return GeoCoderStatusCode::TypeByStr(value);
    }
    static QString StrFromGeoCoderStatusCode(GeoCoderStatusCode::Types const& value) {
        return GeoCoderStatusCode::StrByType(value);
    }
    static QStringList GeoCoderTypes() {
        return GeoCoderStatusCode::TypesList();
    }
    static internals::MouseWheelZoomType::Types MouseWheelZoomTypeFromString(QString const& value) {
        return internals::MouseWheelZoomType::TypeByStr(value);
    }
    static QString StrFromMouseWheelZoomType(internals::MouseWheelZoomType::Types const& value) {
        return internals::MouseWheelZoomType::StrByType(value);
    }
    static QStringList MouseWheelZoomTypes() {
        return internals::MouseWheelZoomType::TypesList();
    }
    static core::LanguageType::Types LanguageTypeFromString(QString const& value) {
        return core::LanguageType::TypeByStr(value);
    }
    static QString StrFromLanguageType(core::LanguageType::Types const& value) {
        return core::LanguageType::StrByType(value);
    }
    static QStringList LanguageTypes() {
        return core::LanguageType::TypesList();
    }
    static core::AccessMode::Types AccessModeFromString(QString const& value) {
        return core::AccessMode::TypeByStr(value);
    }
    static QString StrFromAccessMode(core::AccessMode::Types const& value) {
        return core::AccessMode::StrByType(value);
    }
    static QStringList AccessModeTypes() {
        return core::AccessMode::TypesList();
    }
    static UAVMapFollowType::Types UAVMapFollowFromString(QString const& value) {
        return UAVMapFollowType::TypeByStr(value);
    }
    static QString StrFromUAVMapFollow(UAVMapFollowType::Types const& value){return UAVMapFollowType::StrByType(value);}
    static QStringList UAVMapFollowTypes(){return UAVMapFollowType::TypesList();}
    static UAVTrailType::Types UAVTrailTypeFromString(QString const& value){return UAVTrailType::TypeByStr(value);}
    static QString StrFromUAVTrailType(UAVTrailType::Types const& value){return UAVTrailType::StrByType(value);}
    static QStringList UAVTrailTypes(){return UAVTrailType::TypesList();}
};


class OPMapWidget:public QGraphicsView
{
    Q_OBJECT

    // Q_PROPERTY(int MaxZoom READ MaxZoom WRITE SetMaxZoom)
    Q_PROPERTY(int MinZoom READ MinZoom WRITE SetMinZoom)
    Q_PROPERTY(bool ShowTileGridLines READ ShowTileGridLines WRITE SetShowTileGridLines)
    Q_PROPERTY(double Zoom READ ZoomTotal WRITE SetZoom)
    Q_PROPERTY(qreal Rotate READ Rotate WRITE SetRotate)
    Q_ENUMS(internals::MouseWheelZoomType::Types)
    Q_ENUMS(core::GeoCoderStatusCode::Types)

public:
    QSize sizeHint() const;

    OPMapWidget(QWidget *parent=0,Configuration *config=new Configuration);
    virtual ~OPMapWidget();

    bool ShowTileGridLines() const {
        return map->showTileGridLines;
    }

    void SetShowTileGridLines(bool const& value) {
        map->showTileGridLines=value;
        map->update();
    }
    int MaxZoom() const {
        return map->MaxZoom();
    }
    void SetMaxZoom(int mz) {
        map->SetMaxZoom(mz);
    }

    //  void SetMaxZoom(int const& value){map->maxZoom = value;}
    int MinZoom()const{return map->minZoom;}
    void SetMinZoom(int const& value){map->minZoom = value;}

    internals::MouseWheelZoomType::Types GetMouseWheelZoomType() {
        return  map->core->GetMouseWheelZoomType();
    }
    void SetMouseWheelZoomType(internals::MouseWheelZoomType::Types const& value) {
        map->core->SetMouseWheelZoomType(value);
    }
    //  void SetMouseWheelZoomTypeByStr(const QString &value){map->core->SetMouseWheelZoomType(internals::MouseWheelZoomType::TypeByStr(value));}
    //  QString GetMouseWheelZoomTypeStr(){return map->GetMouseWheelZoomTypeStr();}

    internals::RectLatLng SelectedArea() const {
        return  map->selectedArea;
    }
    void SetSelectedArea(internals::RectLatLng const& value) {
        map->selectedArea = value;
        this->update();
    }

    bool CanDragMap() const {
        return map->CanDragMap();
    }
    void SetCanDragMap(bool const& value) {
        map->SetCanDragMap(value);
    }

    internals::PointLatLng CurrentPosition() const {
        return map->core->CurrentPosition();
    }
    void SetCurrentPosition(internals::PointLatLng const& value) {
        map->core->SetCurrentPosition(value);
    }

    double ZoomReal()  {return map->Zoom();}
    double ZoomDigi()  {return map->ZoomDigi();}
    double ZoomTotal() {return map->ZoomTotal();}

    qreal Rotate(){return map->rotation;}
    void SetRotate(qreal const& value);

    void ReloadMap() {
        map->ReloadMap();
        map->resize();
    }

    GeoCoderStatusCode::Types SetCurrentPositionByKeywords(QString const& keys) {
        return map->SetCurrentPositionByKeywords(keys);
    }

    bool UseOpenGL(){return useOpenGL;}
    void SetUseOpenGL(bool const& value);

    MapType::Types GetMapType() {
        return map->core->GetMapType();
    }
    void SetMapType(MapType::Types const& value) {
        map->lastimage=QImage();
        map->core->SetMapType(value);
    }

    bool isStarted() {
        return map->core->isStarted();
    }

    Configuration* configuration;
    internals::PointLatLng currentMousePosition();
    void SetFollowMouse(bool const& value){followmouse=value;this->setMouseTracking(followmouse);}
    bool FollowMouse(){return followmouse;}
    internals::PointLatLng GetFromLocalToLatLng(QPointF p) {return map->FromLocalToLatLng(p.x(),p.y());}
    float metersToPixels(double meters);
    double bearing(internals::PointLatLng from, internals::PointLatLng to);
    internals::PointLatLng destPoint(internals::PointLatLng source, double bear, double dist);
    WayPointItem* WPCreate();
    void WPCreate(WayPointItem* item);
    void WPCreate(int id, WayPointItem* item);
    WayPointItem* WPCreate(internals::PointLatLng const& coord, int const& altitude);
    WayPointItem* WPCreate(internals::PointLatLng const& coord,int const& altitude, QString const& description);
    WayPointItem* WPInsert(int const& position);
    void WPInsert(WayPointItem* item,int const& position);
    WayPointItem* WPInsert(internals::PointLatLng const& coord,int const& altitude,int const& position);
    WayPointItem* WPInsert(internals::PointLatLng const& coord,int const& altitude, QString const& description,int const& position);
    void WPDelete(WayPointItem* item);
    void WPDeleteAll();
    QList<WayPointItem*> WPSelected();
    QMap<int, WayPointItem*> WPAll();
    void WPRenumber(WayPointItem* item,int const& newnumber);
    void SetShowCompass(bool const& value);
    // FIXME XXX Move to protected namespace
    UAVItem* UAV;
    QMap<int, QGraphicsItemGroup*> waypointLines;
    GPSItem* GPS;
    HomeItem* Home;
    // END OF FIXME XXX
    UAVItem* AddUAV(int id);
    void AddUAV(int id, UAVItem* uav);
    void DeleteUAV(int id);
    UAVItem* GetUAV(int id);
    const QList<UAVItem*> GetUAVS();
    QGraphicsItemGroup* waypointLine(int id);
    void SetShowUAV(bool const& value);
    bool ShowUAV()const{return showuav;}
    void SetUavPic(QString UAVPic);

    void SetShowHome(bool const& value);
    bool ShowHome()const{return showhome;}
    void SetShowDiagnostics(bool const& value);

    QMap<int, UAVItem*> UAVS;

private:
    internals::Core *core;
    QGraphicsScene mscene;
    bool useOpenGL;
    GeoCoderStatusCode x;
    MapType y;
    core::AccessMode xx;
    internals::PointLatLng currentmouseposition;
    bool followmouse;
    QGraphicsSvgItem *compass;
    bool showuav;
    bool showhome;
    QTimer *diagTimer;
    bool showDiag;
    QGraphicsTextItem *diagGraphItem;

private slots:
    void diagRefresh();
    //   WayPointItem* item;//apagar

protected:
    MapGraphicItem *map;
    void resizeEvent(QResizeEvent *event);
    void showEvent ( QShowEvent * event );
    void closeEvent(QCloseEvent *event);

    void mouseMoveEvent(QMouseEvent *event );
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void ConnectWP(WayPointItem* item);

    void paintEvent(QPaintEvent *event);

signals:
    void mouseMove(QMouseEvent *event);
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void zoomChanged(double zoomt,double zoom, double zoomd);
    void zoomChanged(int newZoom);
    void WPNumberChanged(int const& oldnumber,int const& newnumber,WayPointItem* waypoint);
    void WPValuesChanged(WayPointItem* waypoint);
    void WPReached(WayPointItem* waypoint);
    void WPInserted(int const& number,WayPointItem* waypoint);
    void WPDeleted(int const& number);
    void UAVReachedWayPoint(int const& waypointnumber, WayPointItem* waypoint);
    void UAVLeftSafetyBouble(internals::PointLatLng const& position);
    void OnCurrentPositionChanged(internals::PointLatLng point);
    void OnTileLoadComplete();
    void OnTileLoadStart();
    void OnMapDrag();
    void OnMapZoomChanged();
    void OnMapTypeChanged(MapType::Types type);
    void OnEmptyTileError(int zoom, core::Point pos);
    void OnTilesStillToLoad(int number);

public slots:
    void RipMap();
    void SetZoom(double const& value){map->SetZoom(value);}
    void SetZoom(int const& value){map->SetZoom(value);}
    void emitMapZoomChanged()
    {
        emit zoomChanged(ZoomReal());
    }
};

} // end of namespace mapcontrol

#endif // OPMAPWIDGET_H
