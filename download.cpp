#include "download.h"
/*!
 * \brief Download::Download
 */
Download::Download(Launcher *parent)
{
    this->launcher = parent;
    this->downloadWindow = launcher->downloads;
    //this->downloadScreen = parent->get
    this->path = parent->getPath();
    this->filename = parent->getCurrentFileName();
    this->codec = parent->getCodec();
    this->process = new MyProcess(this);
    this->url = parent->getUrl();
    this->profile = new DownloadProfile(this);


}


void Download::startDownload()
{
    //connect(process, SIGNAL(infoSent(QStringList)), this, SLOT(createProfile(QStringList)));
    connect(process, SIGNAL(infoSent(QStringList)), this, SLOT(initProfile(QStringList)));
    connect(process, SIGNAL(downloadFinished()), profile, SLOT(handleDownloadFinished()));
    QObject::connect(downloadWindow, SIGNAL(window_loaded()), process, SLOT(relaunchDownload()));
    connect(profile, SIGNAL(profileCreated()), process, SLOT(relaunchDownload()));

    process->downloadFile();
    //process->waitForFinished(-1);
    qDebug() << "Hello";
}

QString Download::getFilename()
{
    return this->filename;
}

QString Download::getUrl()
{
    return this->url;
}

QString Download::getCodec()
{
    return this->codec;
}


QString Download::getPath()
{
    return this->path;
}

void Download::initProfile(QStringList args)
{
    downloadWindow->show();
    qDebug() << "Window being shown";
    this->updateProfile(args);
}

void Download::updateProfile(QStringList args)
{
    qDebug() << "Profile updated";
    this->profile->updateData(args);
}

DownloadProfile *Download::getProfile()
{
    return this->profile;
}
