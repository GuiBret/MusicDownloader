#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "signal.h"
#include "launcher.h"
#include "downloadprofile.h"

const int DOWNLOAD_NOT_STARTED = 1;
const int DOWNLOAD_ABORTED = 2;
const int DOWNLOAD_RESTARTED = 3;
const int DOWNLOAD_FINISHED = 4;
const int ERROR = -1;

class Launcher;

class MyProcess : public QProcess
{
    Q_OBJECT
public:
    MyProcess(Launcher *launcher);
    void downloadFile(QStringList args);
public slots:
    void readOutput();
    void relaunchDownload(DownloadProfile *dp);
signals:
    void infoReached();
    void infoSent(QStringList info);
    void downloadInfoSent(QStringList downloadInfo);


private:
    QProcess *downloadProcess;
    Launcher *parentLauncher;
    int downloadState;
    QString path = "";
    QString selectedCodec;

    DownloadProfile *currentProfile = NULL;
    QString command = "";

};

#endif // MYPROCESS_H
