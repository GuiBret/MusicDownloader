#include "utils.h"

Utils::Utils()
{

}

QString Utils::generateLabelEta(QString eta, double percentage, double filesize)
{
    double downloaded = Utils::calculatePercentage(percentage, filesize);
    QString labelString = QString::number(downloaded, 'f', 2) + " of "+ QString::number(filesize, 'f', 2) + "MiB downloaded ("+ QString::number(percentage, 'f', 1) +"%)";
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
    QString filesize = QString::number(Utils::parseDouble(outputList[1]));
    QString percentage = QString::number(Utils::parseDouble(outputList[0]));
    QString eta = outputList[2];
    info << filesize << percentage << eta;

    return info;
}

bool Utils::checkFileLocation(QString path)
{
    return (QFile::exists(path));
}

bool Utils::checkYoutubeLink(QString clipboardContent)
{
    //qDebug() << YOUTUBE_LINK.exactMatch(clipboardContent);
    qDebug() << clipboardContent;
    return YOUTUBE_LINK.match(clipboardContent).hasPartialMatch();
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
    QStringList pathWithoutFileName = path.split("/");
    pathWithoutFileName.pop_back();
    qDebug() << pathWithoutFileName.join("/");
    QProcess p;
    p.startDetached("nautilus --browser "+pathWithoutFileName.join("/"));

}
