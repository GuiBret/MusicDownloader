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
DownloadProfile::DownloadProfile(QStringList args, DownloadDisplay *parent) :
    ui(new Ui::DownloadProfile)
{
    ui->setupUi(this);
    qDebug() << args;
    filename = args[0]+"."+args[5];
    percentage = Utils::parseDouble(args[1]);
    filesize = Utils::parseDouble(args[2]);
    download_speed = args[3];
    eta = args[4];
    QMessageBox::information(this, "d", "Profil créé");


    ui->lbl_filename->setText(filename);
    ui->pb_downloadprogress->setRange(0.0, filesize);
    ui->lbl_eta->setText(Utils::generateLabelEta(eta, percentage, filesize));

    parent->appendDownloadProfile(this);
    qDebug() << "Profile created";
    emit profileCreated(this);
}

void DownloadProfile::updateWidget()
{
    if(percentage != 0.0)
        ui->pb_downloadprogress->setValue(Utils::calculatePercentage(percentage, filesize));
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
        double new_percentage = Utils::parseDouble(args[1]); // args[1] : percentage not parsed
        QString eta  = args[3];
        this->percentage = new_percentage;
        double filesize = Utils::parseDouble(args[1]);
        //this->ui->lbl_eta->setText(Utils::generateLabelEta(eta, percentage, filesize));
        this->updateWidget();
    }
}
