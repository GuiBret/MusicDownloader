#ifndef DOWNLOADPROFILECONTEXTMENU_H
#define DOWNLOADPROFILECONTEXTMENU_H

#include <QMenu>
#include <QAction>
#include <QObject>

#include "utils.h"
#include "downloadprofile.h"

class DownloadProfile;

class DownloadProfileContextMenu : public QMenu
{
    Q_OBJECT
public:
    DownloadProfileContextMenu(DownloadProfile *parent);
    ~DownloadProfileContextMenu();
    void handleDownloadFinished();
private:
    QAction* searchFileInBrowser;
    QAction* deleteFile;
    QAction* removeDownloadFromWidget;
    DownloadProfile *parent;
private slots:
    void openFolderInBrowser();
};

#endif // DOWNLOADPROFILECONTEXTMENU_H
