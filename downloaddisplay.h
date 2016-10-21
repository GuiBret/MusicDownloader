#ifndef DOWNLOADDISPLAY_H
#define DOWNLOADDISPLAY_H

#include <QWidget>
#include <QMainWindow>
#include "downloadprofile.h"
#include <QLabel>
#include <QVBoxLayout>

class DownloadProfile;

namespace Ui {
class DownloadDisplay;
}

class DownloadDisplay : public QMainWindow
{
    Q_OBJECT

public:
    explicit DownloadDisplay(QWidget *parent = 0);
    ~DownloadDisplay();
    QVector<DownloadProfile *> downloads;

    void appendDownloadProfile(DownloadProfile *dp);
    void updateDownloadProfile(QStringList args);
signals:
    void profileCreated(DownloadProfile *dp);

private:
    Ui::DownloadDisplay *ui;
    QVBoxLayout *vl_download;
};

#endif // DOWNLOADDISPLAY_H
