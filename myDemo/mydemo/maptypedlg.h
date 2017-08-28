#ifndef MAPTYPEDLG_H
#define MAPTYPEDLG_H
#include <QDialog>
#include <opmapwidget.h>
#include <QDialogButtonBox>
#include <QComboBox>
class MapTypeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MapTypeDlg(QWidget *parent = 0);

    void setupUi(void);

    void setupMapType_list(void);

    void setMapType(core::MapType::Types t);
    core::MapType::Types getMapType(void);

public:
    QDialogButtonBox        *buttonBox;
    QLabel                  *labMapType;
    QComboBox               *cbMapType;
};

#endif // MAPTYPEDLG_H
