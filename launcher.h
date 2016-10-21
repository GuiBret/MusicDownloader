#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QEventLoop>

#include "downloaddisplay.h"
#include "downloadprofile.h"
#include "myprocess.h"

class MyProcess;

namespace Ui {
class Launcher;
}

class Launcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Launcher(QWidget *parent = 0);
    ~Launcher();

private:
    Ui::Launcher *ui;
    QString videoUrl;
    QString videoName;
    MyProcess *process;
    DownloadDisplay *downloads;
    bool download_started;

private slots:
    void browseFileLocation();
    void checkUrlValidity();
    void downloadFile();
    void parseOutput();
    void parseError();
    void disconnectSlots();
    void createProfile(QStringList info);
signals:
    void downloadStarted();
    void profileAdded();
    void downloadFinished();
};

#endif // LAUNCHER_H
