#include "maptypedlg.h"


struct MapType_Data
{
    char    name[256];
    int     typeID;
};

#define MAPTYPE_STRUCT(n)   { #n, core::MapType::n }

static MapType_Data g_arrMapType[] =
{
    MAPTYPE_STRUCT(GaoDeMap),
    MAPTYPE_STRUCT(GaoDeSatellite),
    MAPTYPE_STRUCT(GaoDeLabels),
    MAPTYPE_STRUCT(GaoDeHybrid),

//    MAPTYPE_STRUCT(GoogleMap),
//    MAPTYPE_STRUCT(GoogleSatellite),
//    MAPTYPE_STRUCT(GoogleLabels),
//    MAPTYPE_STRUCT(GoogleTerrain),
//    MAPTYPE_STRUCT(GoogleHybrid),

    MAPTYPE_STRUCT(GoogleMapChina),
    MAPTYPE_STRUCT(GoogleSatelliteChina),
    MAPTYPE_STRUCT(GoogleLabelsChina),
    MAPTYPE_STRUCT(GoogleTerrainChina),
    MAPTYPE_STRUCT(GoogleHybridChina),

//    MAPTYPE_STRUCT(OpenStreetMap),
//    MAPTYPE_STRUCT(OpenStreetOsm),
//    MAPTYPE_STRUCT(OpenStreetMapSurfer),
//    MAPTYPE_STRUCT(OpenStreetMapSurferTerrain),

//    MAPTYPE_STRUCT(YahooMap),
//    MAPTYPE_STRUCT(YahooSatellite),
//    MAPTYPE_STRUCT(YahooLabels),
//    MAPTYPE_STRUCT(YahooHybrid),

//    MAPTYPE_STRUCT(BingMap),
//    MAPTYPE_STRUCT(BingSatellite),
//    MAPTYPE_STRUCT(BingHybrid),

//    MAPTYPE_STRUCT(ArcGIS_Map),
//    MAPTYPE_STRUCT(ArcGIS_Satellite),
//    MAPTYPE_STRUCT(ArcGIS_ShadedRelief),
//    MAPTYPE_STRUCT(ArcGIS_Terrain),
//    MAPTYPE_STRUCT(ArcGIS_WorldTopo),

//    MAPTYPE_STRUCT(ArcGIS_MapsLT_Map),
//    MAPTYPE_STRUCT(ArcGIS_MapsLT_OrtoFoto),
//    MAPTYPE_STRUCT(ArcGIS_MapsLT_Map_Labels),
//    MAPTYPE_STRUCT(ArcGIS_MapsLT_Map_Hybrid),

//    MAPTYPE_STRUCT(PergoTurkeyMap),
//    MAPTYPE_STRUCT(SigPacSpainMap),

//    MAPTYPE_STRUCT(GoogleMapKorea),
//    MAPTYPE_STRUCT(GoogleSatelliteKorea),
//    MAPTYPE_STRUCT(GoogleLabelsKorea),
//    MAPTYPE_STRUCT(GoogleHybridKorea),

//    MAPTYPE_STRUCT(YandexMapRu),
//    MAPTYPE_STRUCT(Statkart_Topo2),

    {"NULL", -1}
};

char *getMapName_fromID(core::MapType::Types t)
{
    int     i = 0;

    while(1) {
        if( g_arrMapType[i].typeID == t )
            return g_arrMapType[i].name;

        if( g_arrMapType[i].typeID < 0 ) return NULL;

        i ++;
    }

    return NULL;
}

MapTypeDlg::MapTypeDlg(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("Select Map Type");

    setupUi();

    setupMapType_list();
    setMapType(core::MapType::GaoDeMap);
}

void MapTypeDlg::setupUi(void)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MapWidget_MapTypeDiag"));
    this->setFixedSize(374, 122);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(20, 80, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    labMapType = new QLabel(this);
    labMapType->setObjectName(QString::fromUtf8("labMapType"));
    labMapType->setGeometry(QRect(32, 34, 61, 15));
    labMapType->setText("Map Type:");

    cbMapType = new QComboBox(this);
    cbMapType->setObjectName(QString::fromUtf8("cbMapType"));
    cbMapType->setGeometry(QRect(100, 30, 251, 25));

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QMetaObject::connectSlotsByName(this);
}

void MapTypeDlg::setupMapType_list(void)
{
    int     i;

    i = 0;
    while(1) {
        if( g_arrMapType[i].typeID >= 0 )
            cbMapType->addItem(g_arrMapType[i].name);
        else
            break;

        i ++;
    }
}

void MapTypeDlg::setMapType(core::MapType::Types t)
{
    int i = 0;

    while(1) {
        if( g_arrMapType[i].typeID == t ) {
            cbMapType->setCurrentIndex(i);
            return;
        }

        if( g_arrMapType[i].typeID == -1 ) break;

        i ++;
    }

    // set default map
    cbMapType->setCurrentIndex(0);
}

core::MapType::Types MapTypeDlg::getMapType(void)
{
    int     idx, typeID;

    idx    = cbMapType->currentIndex();
    typeID = g_arrMapType[cbMapType->currentIndex()].typeID;

    return (core::MapType::Types) typeID;
}
