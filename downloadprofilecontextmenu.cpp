#include "downloadprofilecontextmenu.h"

DownloadProfileContextMenu::DownloadProfileContextMenu(DownloadProfile *profile)
{
    this->parent = profile;
    searchFileInBrowser = new QAction("Open in browser");
    deleteFile = new QAction("Delete file");
    removeDownloadFromWidget = new QAction("Remove download from widget");

    connect(this->searchFileInBrowser, SIGNAL(triggered(bool)), this, SLOT(openFolderInBrowser()));

    this->addAction(this->searchFileInBrowser);
    this->addAction(this->deleteFile);
    this->addAction(this->removeDownloadFromWidget);

    deleteFile->setEnabled(false);
    removeDownloadFromWidget->setEnabled(false);
}

DownloadProfileContextMenu::~DownloadProfileContextMenu()
{
    /*
    delete searchFileInBrowser;
    delete deleteFile;
    delete removeDownloadFromWidget;
    */
}

void DownloadProfileContextMenu::openFolderInBrowser()
{
    Utils::openFolder(parent->path);
}

void DownloadProfileContextMenu::handleDownloadFinished()
{
    qDebug() << "Handle download finished";
    this->deleteFile->setEnabled(true);
    this->removeDownloadFromWidget->setEnabled(true);
}
