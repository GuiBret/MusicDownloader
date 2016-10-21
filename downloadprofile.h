#ifndef DOWNLOADPROFILE_H
#define DOWNLOADPROFILE_H

#include <QWidget>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>

#include "downloaddisplay.h"
#include "utils.h"

class DownloadDisplay;


namespace Ui {
class DownloadProfile;
}

class DownloadProfile : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadProfile(QStringList args, DownloadDisplay *parent);
    ~DownloadProfile();
    void updateDownloadProfile();
    QString getFilename();
    void updateData(QStringList args);
signals:
    void profileCreated(DownloadProfile *currentProfile);

private:
    Ui::DownloadProfile *ui;
    QString filename;
    QString eta;
    QString download_speed;
    double filesize;
    double percentage;

    void updateWidget();



};

#endif // DOWNLOADPROFILE_H
