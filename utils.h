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
#include <QProcess>
#include <QtGlobal>



#ifdef Q_OS_LINUX
#define ROOT "/"
#elif Q_OS_WIN32 | Q_OS_WIN64
#define ROOT "C:"
#endif

//TODO : fix regex problem
//Multiple regexp's to handle console output
const QRegularExpression YOUTUBE_LINK("https{0,1}://www{0,1}.youtube.com/watch\\?v=*");

const QRegExp DOWNLOAD_FINISHED_REGEX("100% of [0-9.]{4,}MiB in [0-9]{2}:[0-9]{2}");
const QRegExp DESTINATION_REGEX("Destination : (/home(/.*)*[.mp3, .aac])");
const QRegExp DOWNLOAD_PROGRESS("(\\d+.\\d)% of (\\d+.\\d+[KM]iB) at (\\d+.\\d+[KM]iB/s) ETA ([0-9]{2}:[0-9]{2})");

class Utils : public QObject
{
    Q_OBJECT
public:
    Utils();
    static QString generateLabelEta(QString eta, double percentage, double filesize);
    static double parseDouble(QString number);
    static double calculatePercentage(double percentage, double filesize);
    static QStringList handleInfo(QString output);
    static bool checkFileLocation(QString path);
    static bool checkYoutubeLink(QString clipboard);
    static QPixmap getThumbnail(QUrl url);
    static void openFolder(QString path);
    static bool checkYoutubeDlInstall();
    static QStringList handleOutput(QString output);
private:


};

#endif // UTILS_H
