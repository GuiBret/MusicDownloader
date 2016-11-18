#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "signal.h"
#include "launcher.h"
#include "downloadprofile.h"
#include "download.h"
#include "stdlib.h"
#include <QUrl>

const int DOWNLOAD_NOT_STARTED = 1;
const int DOWNLOAD_ABORTED = 2;
const int DOWNLOAD_RESTARTED = 3;
const int DOWNLOAD_FINISHED = 4;
const int ERROR = -1;

class Download;
class DownloadProfile;
class MyProcess : public QProcess
{
    Q_OBJECT
public:
    MyProcess(Download *parent);
    void downloadFile();
public slots:
    void readOutput();
    void relaunchDownload();
    QUrl getThumbnailUrl(QString videoUrl);

    // TO BE DELETED
    void readError();
signals:
    void infoReached();
    void infoSent(QStringList info);
    void downloadInfoSent(QStringList downloadInfo);
    void downloadFinished();

private:
    Download *parentDownload;
    int downloadState;
    QString path = "";
    QString selectedCodec;

    DownloadProfile *currentProfile = NULL;
    QString command = "";


};

#endif // MYPROCESS_H
