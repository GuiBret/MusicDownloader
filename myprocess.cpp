#include "myprocess.h"

MyProcess::MyProcess(Launcher *parent)
{
    this->parentLauncher = parent;
    this->downloadState = 1;
}

void MyProcess::pause()
{
    p = new QProcess(this);


    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(output()));
    connect(this, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(output()));
    qint64 pid = this->pid();
    ::kill(pid, SIGSTOP);
    qDebug() << this->pid();
    qDebug() << "Process paused";
    qDebug() << this->state();
}

void MyProcess::restart()
{

    QProcess p(this);
    qDebug() << "Goodbye";
    qint64 pid = this->pid();
    ::kill(pid, SIGCONT);
    //p.start("kill -s SIGCONT " + QString::number(pid));
}


void MyProcess::output()
{
    QMessageBox::information(NULL, "Hello", "Meug");
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
    QString selectedCodec = args[1];
    this->path = args[2];

    QString command = "youtube-dl --extract-audio --audio-format "+ selectedCodec+" -o "+ "\""+ path.simplified() +"."+ selectedCodec+"\"" +" "+url;
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(this, SIGNAL(infoReached()), this, SLOT(kill()));
    this->start(command);

}

void MyProcess::readOutput()
{
    QStringList output = QString(this->readAllStandardOutput()).split("[download] "); // Handling of multiple lines coming at the same time
    output.pop_front();
    //qDebug() << output;
    if(this->downloadState == DOWNLOAD_NOT_STARTED) // We wait for the first [download] line
    {
        for(QString outputLine : output)
        {
            if(!outputLine.startsWith("Destination"))
            {
                QStringList fileInfo = Utils::handleInfo(outputLine);
                fileInfo << this->path;
                emit infoSent(fileInfo);
            }
        }
    }
}
