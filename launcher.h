#ifndef LAUNCHER_H
#define LAUNCHER_H



#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QEventLoop>
#include <QClipboard>


#include "downloaddisplay.h"
#include "downloadprofile.h"
#include "download.h"

class MyProcess;
class Download;
class DownloadDisplay;



namespace Ui {
class Launcher;
}

class Launcher : public QMainWindow
{
    Q_OBJECT
    friend class Download;

public:
    explicit Launcher(QWidget *parent = 0);
    ~Launcher();
    QString getCodec();
    QString getUrl();
    QString getPath();
    QString getCurrentFileName();
    QVector<Download *> downloadList;

private:
    Ui::Launcher *ui;
    QString videoUrl;
    QString videoName;
    DownloadDisplay *downloads;
    bool download_started;
    QClipboard *cb;
    QString currentFileName = "";
    void checkYoutubeDlInstall();
    Download *searchDownload(QString filename);
    QUrl getThumbnailUrl(QString url);

private slots:
    void browseFileLocation();
    void checkUrlValidity();
    void downloadFile();
    void checkClipboard();
    void searchThumbnail();
    void handleDownloadFinished();
    QVector<Download *> getDownloadList();
signals:
    void downloadStarted();
    void profileAdded();
    void validUrl();
    void youtubeDlNotInstalled();
    void downloadInstanceCreated(Download *d);

};

#endif // LAUNCHER_H
