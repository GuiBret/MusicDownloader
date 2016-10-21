#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
class Utils
{
public:
    Utils();
    static QString generateLabelEta(QString eta, double percentage, double filesize);
    static double parseDouble(QString number);
    static double calculatePercentage(double percentage, double filesize);
    static QStringList handleInfo(QString output);
private:

};

#endif // UTILS_H
