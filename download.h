#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "launcher.h"
#include "myprocess.h"
#include "downloaddisplay.h"
#include "downloadprofile.h"
#include <QObject>

class Launcher;
class MyProcess;
class DownloadDisplay;
class DownloadProfile;

class Download : public QObject
{
    Q_OBJECT
    public:
        Download(Launcher *parent);
        void startDownload();
        QString getFilename();
        QString getCodec();
        QString getUrl();
        QString getPath();
        DownloadProfile *getProfile();
    private:
        Launcher *launcher;
        DownloadProfile *profile;
        DownloadDisplay *downloadWindow;
        MyProcess *process;
        //int download_state;
        QString path;
        QString codec;
        QString filename;
        QString url;

    private slots:
        void initProfile(QStringList args);
        void updateProfile(QStringList args);



};

#endif // DOWNLOAD_H
