#ifndef WAYPOINTITEM_H
#define WAYPOINTITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QLabel>
#include "../internals/pointlatlng.h"
#include "mapgraphicitem.h"
#include <QObject>

namespace mapcontrol {

class WayPointItem : public QObject,public QGraphicsItem
{
    Q_OBJECT

    Q_INTERFACES(QGraphicsItem)

public:
    enum { Type = UserType + 1 };
    WayPointItem(internals::PointLatLng const& coord, double const& altitude, MapGraphicItem* map);
    WayPointItem(internals::PointLatLng const& coord,double const& altitude,QString const& description,MapGraphicItem* map);

    ~WayPointItem();
    QString Description(){return description;}
    void SetDescription(QString const& value);
    bool Reached(){return reached;}
    void SetReached(bool const& value);
    int Number(){return number;}
    void SetNumber(int const& value);
    internals::PointLatLng Coord() const {return coord;}
    void SetCoord(internals::PointLatLng const& value);
    bool ShowNumber(){return shownumber;}
    virtual void SetShowNumber(bool const& value);
    double Altitude(){return altitude;}
    void SetAltitude(double const& value);
    float Heading(){return heading;}
    void SetHeading(float const& value);

    int type() const;
    virtual QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);
    void RefreshPos();
    void RefreshToolTip();
    QPixmap picture;

    static int snumber;

protected:
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

    MapGraphicItem* map;
    bool autoreachedEnabled;    ///< If the waypoint should change appearance once it has been reached
    QGraphicsSimpleTextItem* text;
    QGraphicsRectItem* textBG;
    QGraphicsSimpleTextItem* numberI;
    QGraphicsRectItem* numberIBG;
    QTransform transf;
    internals::PointLatLng coord;//coordinates of this WayPoint
    bool reached;
    QString description;
    bool shownumber;
    bool isDragging;
    double altitude;
    float heading;
    int number;
    bool m_mouseDown;

public slots:
    void WPDeleted(int const& number);
    void WPRenumbered(int const& oldnumber,int const& newnumber,WayPointItem* waypoint);
    void WPInserted(int const& number,WayPointItem* waypoint);


signals:

    void WPNumberChanged(int const& oldnumber,int const& newnumber,WayPointItem* waypoint);
    void WPValuesChanged(WayPointItem* waypoint);
    void WPEdit(int num, WayPointItem *wp);
};

} // end of namespace mapcontrol

#endif // WAYPOINTITEM_H
