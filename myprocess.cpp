#include "myprocess.h"

MyProcess::MyProcess(Download *parent)
{
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
    this->path = parentDownload->getPath()+"/"+filename.simplified() +"."+selectedCodec;
    qDebug() << "Filename : " << path;

    this->command = "youtube-dl --extract-audio --audio-format "+ selectedCodec+" -o "+ "\""+ path.simplified() +"\"" +" "+url;
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
    QStringList output = QString(this->readAllStandardOutput()).split("[download] "); // Handling of multiple lines coming at the same time
    output.pop_front();
    qDebug() << output;
    if(this->downloadState == DOWNLOAD_NOT_STARTED) // We wait for the first [download] line
    {
        for(QString outputLine : output)
        {
            if(!outputLine.startsWith("Destination"))
            {
                this->close();
                this->downloadState = DOWNLOAD_ABORTED;
                QStringList fileInfo = Utils::handleInfo(outputLine);
                fileInfo << this->path;
                emit infoSent(fileInfo);

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
                QStringList downloadInfo = Utils::handleInfo(outputLine);
                QString filename = (this->path.split("/").last());
                downloadInfo << filename;

                if(downloadInfo[1] == "100")
                    this->downloadState = DOWNLOAD_FINISHED;
                currentProfile->updateData(downloadInfo);
            }


        }
    }
    else if(this->downloadState == DOWNLOAD_FINISHED)
    {
        //TODO

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
        connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));

        this->currentProfile = this->parentDownload->getProfile();; // Update file info more easily
        this->downloadState = DOWNLOAD_RESTARTED;
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
