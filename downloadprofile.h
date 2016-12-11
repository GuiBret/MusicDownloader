#ifndef DOWNLOADPROFILE_H
#define DOWNLOADPROFILE_H

#include <QWidget>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <QMenu>
#include <QPoint>

#include "downloaddisplay.h"
#include "utils.h"
#include "downloadprofilecontextmenu.h"
#include "download.h"

class DownloadDisplay;
class DownloadProfileContextMenu;
class Download;


namespace Ui {
class DownloadProfile;
}

class DownloadProfile : public QWidget
{
    Q_OBJECT
    friend class DownloadProfileContextMenu;

public:
    explicit DownloadProfile(Download *parent);
    ~DownloadProfile();
    void updateDownloadProfile();
    QString getFilename();
    void updateData(QStringList args);
    void handleDownloadFinished();
signals:
    void profileCreated();
private:
    Ui::DownloadProfile *ui;
    QString filename;
    QString eta;
    QString download_speed;
    double filesize;
    double percentage;
    QString path;

    void updateWidget();
    DownloadProfileContextMenu *contextMenu;

private slots:
    void showContextMenu(QPoint p);



};

#endif // DOWNLOADPROFILE_H
