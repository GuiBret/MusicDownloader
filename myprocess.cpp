#include "myprocess.h"

MyProcess::MyProcess(Download *parent)
{
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    environment.insert("PATH", qApp->applicationDirPath()+"/bin/");
    qDebug() << environment.value("PATH");
    this->setProcessEnvironment(environment);
    this->parentDownload = parent;
    this->downloadState = DOWNLOAD_NOT_STARTED;
}


/*!
 * \brief MyProcess::downloadFile
 * Step 1 : get the file info
 * Step 2 : abort download
 * Step 3 : Restart download after adding info to the download profile
 *
 * \param args The list of settings used to download the video
 * *
 */
void MyProcess::downloadFile()
{
    QString url = parentDownload->getUrl();
    QString filename = parentDownload->getFilename();
    selectedCodec = parentDownload->getCodec();
    this->path = parentDownload->getPath();
    qDebug() << "Filename : " << path;

    this->command = "youtube-dl --extract-audio --audio-format "+ selectedCodec+" -o "+ "\""+ path.simplified() +"/%(title)s.(ext)s'\"" +" "+url;
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(this, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
    this->start(command);

}
/*!
 * \brief MyProcess::readOutput performs different operations according to the state of the download (this->downloadState) :
 *
 *        State 1 (DOWNLOAD_NOT_STARTED) : 1. waiting for the first line of the progress bar, parsing it, using data to create the download profile.
 *                                         2. pausing download until the download widget is created
 *                                         3. when created, switch to state 2 (DOWNLOAD_ABORTED)
 *        State 2 (DOWNLOAD_ABORTED) : Restart using MyProces::relaunchDownload function (changing state to DOWNLOAD_RESTARTED)
 *        State 3 (DOWNLOAD_RESTARTED) : 1. Parse the progress bar and update info in the widget
 *                                       2. When finished, switch state to DOWNLOAD_FINISHED
 *
 *
 *
 */
void MyProcess::readOutput()
{
    QString rawOutput = this->readAllStandardOutput();
    qDebug() << "Output : " +rawOutput;

    QStringList output = QString(rawOutput).split("[download] "); // Handling of multiple lines coming at the same time
    output.pop_front();
    //qDebug() << output;
    if(this->downloadState == DOWNLOAD_NOT_STARTED) // We wait for the first [download] line
    {
        for(QString outputLine : output)
        {
            if(!outputLine.startsWith("Destination"))
            {
                this->kill();
                this->downloadState = DOWNLOAD_ABORTED;
                QStringList fileInfo = Utils::handleInfo(outputLine);

                fileInfo << this->path;
                emit infoSent(fileInfo);
                break;
                qDebug() << "Info sent";
                qDebug() << this->state();

            }
        }
    }
    else if(this->downloadState == DOWNLOAD_RESTARTED) // Then we relaunch the download
    {
        for(QString outputLine : output)
        {
            outputLine = outputLine.simplified();
            if(!outputLine.startsWith("Resuming") && !outputLine.startsWith("Destination"))
            {
                qDebug() << "Download restarted";
                QStringList downloadInfo = Utils::handleOutput(outputLine);
                QString filename = (this->path.split("/").last());

                if(downloadInfo[0] == "100")
                    this->downloadState = DOWNLOAD_FINISHED;
                currentProfile->updateData(downloadInfo);
            }


        }
    }
    else if(this->downloadState == DOWNLOAD_FINISHED)
    {
        //TODO
        qDebug() << "Download finished";
        qDebug() << output;
        qDebug() << this->readAllStandardError();
        emit downloadFinished();
    }
}

void MyProcess::relaunchDownload()
{
    if(this->downloadState != DOWNLOAD_ABORTED)
    {
        return;
    }
    else
    {
        connect(this, SIGNAL(readyReadStandardError()), this, SLOT(readError()));
        QEventLoop el;
        qDebug() << "Restarting download";
        connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));

        this->currentProfile = this->parentDownload->getProfile();; // Update file info more easily
        this->downloadState = DOWNLOAD_RESTARTED;
        connect(this, SIGNAL(stateChanged(QProcess::ProcessState)), &el, SLOT(quit()));
        el.exec();
        this->start(this->command);
    }
}

QUrl MyProcess::getThumbnailUrl(QString url)
{
    QEventLoop el;
    QProcess p;
    connect(&p, SIGNAL(finished(int)), &el, SLOT(quit()));
    p.start("youtube-dl --get-thumbnail "+ url);
    el.exec();

    QString thumbnailUrl = p.readAllStandardOutput();
    return QUrl(thumbnailUrl.simplified());
}

// DEBUG FUNCTION
void MyProcess::readError()
{
    qDebug() << this->readAllStandardError();
}
