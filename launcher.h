#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include <QEventLoop>
#include <QClipboard>


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
    QClipboard *cb;

private slots:
    void browseFileLocation();
    void checkUrlValidity();
    void downloadFile();
    void createProfile(QStringList info);
    void checkClipboard();
    void searchThumbnail();
signals:
    void downloadStarted();
    void profileAdded();
    void downloadFinished();
    void validUrl();
};

#endif // LAUNCHER_H
