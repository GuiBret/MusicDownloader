#include "utils.h"

Utils::Utils()
{

}

QString Utils::generateLabelEta(QString eta, double percentage, double filesize)
{
    double downloaded = Utils::calculatePercentage(percentage, filesize);
    QString labelString = QString::number(downloaded, 'f', 2) + " of "+ QString::number(filesize, 'f', 2) + tr("MiB downloaded (ETA : ")+ eta +")";
    return labelString;
}

double Utils::parseDouble(QString number)
{

    return number.remove(QRegExp("[^0-9.]")).toDouble();
}

double Utils::calculatePercentage(double percentage, double filesize)
{
    return double((percentage/100) * filesize);
}

QStringList Utils::handleInfo(QString output)
{
    // TODO : improve this
    output = output.replace(" of ", "|").replace(" at ", "|").replace(" ETA ", "|");

    QStringList outputList = output.split("|");
    QStringList info;
    QString percentage = QString::number(Utils::parseDouble(outputList[0]));
    QString filesize = QString::number(Utils::parseDouble(outputList[1]));


    QString eta = outputList[3];
    info << filesize << percentage << eta;

    return info;
}

bool Utils::checkFileLocation(QString path)
{
    qDebug() << "Checking file location";
    qDebug() << path;
    return (QFile::exists(path));
}

bool Utils::checkYoutubeLink(QString clipboardContent)
{
    qDebug() << YOUTUBE_LINK.match(clipboardContent).hasMatch();
    qDebug() << clipboardContent;
    return YOUTUBE_LINK.match(clipboardContent).hasMatch();
}

QPixmap Utils::getThumbnail(QUrl url)
{
    qDebug() << ROOT;

    QNetworkAccessManager *nam = new QNetworkAccessManager;

    QEventLoop el;
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)), &el, SLOT(quit()));
    QNetworkReply *rep = nam->get(QNetworkRequest(url));
    el.exec();

    QPixmap image;
    image.loadFromData(rep->readAll());
    image = image.scaled(100, 100);

    return image;


}

void Utils::openFolder(QString path)
{
    qDebug() << path;
    QStringList pathWithoutFileName = path.split("/");
    QProcess p;
    qDebug() << EXPLORER + path;
    p.startDetached(EXPLORER + path);


}

bool Utils::checkYoutubeDlInstall()
{
    QProcess p;
    bool installed = true;
    QString currentFolder = qApp->applicationDirPath();

    if(QString(ROOT) == "/") // Unix-based systems
    {
        p.start("apt-cache policy youtube-dl");
        p.waitForFinished(-1);
        if(p.readAllStandardOutput() == "") // If the program is not installed
            installed = false;
    }
    else if(ROOT == "C:") // Win32/64
    {

        p.start(currentFolder+"/youtube-dl.exe");
        p.waitForFinished(-1);
        if(p.readAllStandardError() == "")
            installed = false;
        else
        {
            qDebug() << p.readAllStandardOutput();
        }
    }


    return installed;
}

QStringList Utils::handleOutput(QString output)
{
    QStringList info;
    if(DOWNLOAD_PROGRESS.exactMatch(output))
    {
        int pos =0;
        while(pos = DOWNLOAD_PROGRESS.indexIn(output, pos) != -1)
        {
            qDebug() << DOWNLOAD_PROGRESS.cap(1);
            info << DOWNLOAD_PROGRESS.cap(1) << DOWNLOAD_PROGRESS.cap(2) << DOWNLOAD_PROGRESS.cap(3) << DOWNLOAD_PROGRESS.cap(4);
            pos += DOWNLOAD_PROGRESS.matchedLength();
        }
        qDebug() << info.join(", ");
    }
    else
    {
        //TODO : fix
        info << "100";
    }
    return info;
}
