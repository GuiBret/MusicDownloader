#ifndef DOWNLOADDISPLAY_H
#define DOWNLOADDISPLAY_H

#include <QWidget>
#include <QMainWindow>
#include "downloadprofile.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QSignalSpy>

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
    void window_loaded();

private:
    Ui::DownloadDisplay *ui;
    QVBoxLayout *vl_download;
private slots:
    void showEvent(QShowEvent *event);
    void testSignal();
};

#endif // DOWNLOADDISPLAY_H
