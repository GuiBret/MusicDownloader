#include "myprocess.h"

MyProcess::MyProcess(Launcher *parent)
{
    this->parentLauncher = parent;
    this->downloadState = DOWNLOAD_NOT_STARTED;
    this->downloadProcess = new QProcess();
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
    connect(this->downloadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(this, SIGNAL(infoReached()), this, SLOT(kill()));
    this->downloadProcess->start(command);

}

void MyProcess::readOutput()
{
    QStringList output = QString(this->downloadProcess->readAllStandardOutput()).split("[download] "); // Handling of multiple lines coming at the same time
    qDebug() << output;
    output.pop_front();

    if(this->downloadState == DOWNLOAD_NOT_STARTED) // We wait for the first [download] line
    {
        for(QString outputLine : output)
        {
            if(!outputLine.startsWith("Destination"))
            {
                qDebug() << output;
                //qDebug() << "Download aborted";
                this->downloadProcess->close();
                this->downloadState = DOWNLOAD_ABORTED;
                //qDebug() << this->downloadProcess->state();
                QStringList fileInfo = Utils::handleInfo(outputLine);
                qDebug() << this->path;
                fileInfo << this->path;
                emit infoSent(fileInfo);


                qDebug() << this->downloadProcess->state();
                //QMessageBox::information(NULL, "hELL", "FFZSEF");

            }
        }
    }
    else if(this->downloadState == DOWNLOAD_RESTARTED) // Then we relaunch the download
    {
        for(QString outputLine : output)
        {
            if(!outputLine.startsWith("Resuming") && !outputLine.startsWith("Destination"))
            {
                QStringList downloadInfo = Utils::handleInfo(outputLine);
                QString filename = (this->path.split("/").last()) + "."+ this->selectedCodec;
                downloadInfo << filename;

                if(downloadInfo[1] == "100")
                    this->downloadState = DOWNLOAD_FINISHED;
                currentProfile->updateData(downloadInfo);
                //emit downloadInfoSent(downloadInfo);
                qDebug() << downloadInfo;
            }


        }
    }
    else if(this->downloadState == DOWNLOAD_FINISHED)
    {
        qDebug() << "Dl fini";
    }
}

void MyProcess::relaunchDownload(DownloadProfile *dp)
{
    if(this->downloadState != DOWNLOAD_ABORTED)
    {
        qDebug() << this->downloadState;
        qDebug() << (dp == NULL);
        qDebug() << "Myprocess : Wrong state";
        qDebug() << this->state();
        return;
    }
    else
    {
        connect(this->downloadProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
        this->currentProfile = dp; // Update file info more easily
        this->downloadState = DOWNLOAD_RESTARTED;
        this->downloadProcess->start(this->command);

        qDebug() << "Download restarted";


    }
}
