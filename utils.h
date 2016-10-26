#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QRegularExpression>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QEventLoop>

//TODO : fix regex problem
const QRegularExpression YOUTUBE_LINK("https{0,1}://www{0,1}.youtube.com/watch\?v=[a-zA-Z0-9]*");

const QRegExp DOWNLOAD_FINISHED_REGEX("100% of [0-9.]{4,}MiB in [0-9]{2}:[0-9]{2}");
const QRegExp DESTINATION_REGEX("Destination : [\/home]{1,}.[mp4, aac]");

class Utils
{
public:
    Utils();
    static QString generateLabelEta(QString eta, double percentage, double filesize);
    static double parseDouble(QString number);
    static double calculatePercentage(double percentage, double filesize);
    static QStringList handleInfo(QString output);
    static bool checkFileLocation(QString path);
    static bool checkYoutubeLink(QString clipboard);
    static QPixmap getThumbnail(QString url);
private:
    //Multiple regexp's to handle console output

};

#endif // UTILS_H
