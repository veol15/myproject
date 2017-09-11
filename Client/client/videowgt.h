#ifndef VIDEOWGT_H
#define VIDEOWGT_H

#include <QWidget>

namespace Ui {
class VideoWgt;
}

class VideoWgt : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWgt(QWidget *parent = 0);
    ~VideoWgt();

private:
    Ui::VideoWgt *ui;
};

#endif // VIDEOWGT_H
