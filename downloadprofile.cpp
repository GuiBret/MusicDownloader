#include "downloadprofile.h"
#include "ui_downloadprofile.h"
/*!
 * \brief DownloadProfile::DownloadProfile creates a download widget which will be updated at any console output
 *
 * \param args List of arguments coming from the download process:
 *
 * 0 : filename
 * 1 : percentage completed
 * 2 : file size
 * 3 : downloading speed
 * 4 : ETA
 * 5 : filetype (appended to filename)
 * \param parent
 */
DownloadProfile::DownloadProfile(Download *parent) :
    ui(new Ui::DownloadProfile)
{
    ui->setupUi(this);

    contextMenu = new DownloadProfileContextMenu(this);

    filename = parent->getFilename();
    percentage = 0.0;
    filesize = 0.0;
    download_speed = 0.0;
    //eta = args[4];
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));


    ui->lbl_filename->setText(filename);
    ui->pb_downloadprogress->setRange(0.0, 100.0);
    ui->lbl_eta->setText(Utils::generateLabelEta(eta, percentage, filesize));
    emit profileCreated();

}

void DownloadProfile::updateWidget()
{
    qDebug() << percentage;
    if(percentage != 0.0)
    {
        ui->pb_downloadprogress->setValue(percentage);
        qDebug() << Utils::calculatePercentage(percentage, filesize);
    }
    else
        ui->pb_downloadprogress->setValue(0.0);


    this->update();
}

DownloadProfile::~DownloadProfile()
{
    delete ui;
}

QString DownloadProfile::getFilename()
{
    return this->filename;
}

void DownloadProfile::updateData(QStringList args)
{
    if(args.length() == 4)
    {
        // Output looks like "0.5% of 25.22MiB at 555KiB/s ETA 00:25" captured in this order
        double new_percentage = Utils::parseDouble(args[0]); // args[0] : percentage not parsed
        double filesize = Utils::parseDouble(args[1]);
        QString download_speed = args[2];
        QString eta  = args[3];
        this->percentage = new_percentage;
        qDebug() << args;

        this->ui->lbl_eta->setText(Utils::generateLabelEta(eta, percentage, filesize));
        this->updateWidget();
    }
}

void DownloadProfile::showContextMenu(QPoint p)
{
    contextMenu->exec(this->mapToGlobal(p));
}

void DownloadProfile::handleDownloadFinished()
{
    this->contextMenu->handleDownloadFinished();
}



