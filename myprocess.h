#ifndef MYPROCESS_H
#define MYPROCESS_H

#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "signal.h"
#include "launcher.h"

const int DOWNLOAD_NOT_STARTED = 1;
const int DOWNLOAD_ABORTED = 2;
const int DOWNLOAD_RESTARTED = 3;
const int ERROR = -1;

class Launcher;

class MyProcess : public QProcess
{
    Q_OBJECT
public:
    MyProcess(Launcher *launcher);
    void downloadFile(QStringList args);
public slots:
    void pause();
    void restart();
    void output();
    void readOutput();
signals:
    void infoReached();
    void infoSent(QStringList info);


private:
    QProcess *p;
    Launcher *parentLauncher;
    int downloadState;
};

#endif // MYPROCESS_H
