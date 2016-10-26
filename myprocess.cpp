#include "myprocess.h"

MyProcess::MyProcess(Launcher *parent)
{
    this->parentLauncher = parent;
    //this-> = new QProcess();
    this->downloadState = DOWNLOAD_NOT_STARTED;
}


/*!
 * \brief MyProcess::downloadFile
 * Step 1 : get the file info
 * Step 2 : abort download
 * Step 3 : Restart download after adding info to the download profile
 *
 * \param args The list of settings used to download the video
 *        args[0] : url
 *        args[1] : codec
 *        args[2] : path
 */
void MyProcess::downloadFile(QStringList args)
{
    QString url = args[0];
    selectedCodec = args[1];
    this->path = args[2].simplified();

    this->command = "youtube-dl --extract-audio --audio-format "+ selectedCodec+" -o "+ "\""+ path.simplified() +"."+ selectedCodec+"\"" +" "+url;
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(this, SIGNAL(infoReached()), this, SLOT(kill()));
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
                qDebug() << outputLine;
                QStringList downloadInfo = Utils::handleInfo(outputLine);
                QString filename = (this->path.split("/").last()) + "."+ this->selectedCodec;
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
    }
}

void MyProcess::relaunchDownload(DownloadProfile *dp)
{
    if(this->downloadState != DOWNLOAD_ABORTED)
    {
        return;
    }
    else
    {
        connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
        this->currentProfile = dp; // Update file info more easily
        this->downloadState = DOWNLOAD_RESTARTED;
        this->start(this->command);
    }
}
