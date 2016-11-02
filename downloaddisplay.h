#ifndef DOWNLOADDISPLAY_H
#define DOWNLOADDISPLAY_H

#include <QWidget>
#include <QMainWindow>
#include "downloadprofile.h"
#include "launcher.h"
#include <QLabel>
#include <QVBoxLayout>

class DownloadProfile;
class Launcher;

namespace Ui {
class DownloadDisplay;
}

class DownloadDisplay : public QMainWindow
{
    Q_OBJECT

public:
    explicit DownloadDisplay(Launcher *parent);
    ~DownloadDisplay();
    QVector<Download*> downloadList;
    void enableActions(QString filename);

    void appendDownloadProfile(DownloadProfile *dp);
    void updateDownloadProfile(QStringList args);
signals:
    void window_loaded();

private:
    Ui::DownloadDisplay *ui;
    QVBoxLayout *vl_download;
private slots:
    void showEvent(QShowEvent *event);
    void testSignal();
    void appendDownload(Download *d);

};

#endif // DOWNLOADDISPLAY_H
