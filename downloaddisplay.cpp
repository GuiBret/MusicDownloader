#include "downloaddisplay.h"
#include "ui_downloaddisplay.h"

DownloadDisplay::DownloadDisplay(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DownloadDisplay)
{
    ui->setupUi(this);
    connect(ui->btn_quit, SIGNAL(clicked(bool)), this, SLOT(hide()));
    this->vl_download = new QVBoxLayout();

    ui->scrollArea->setLayout(vl_download);
    vl_download->addWidget(new QLabel("Hello"));
}

DownloadDisplay::~DownloadDisplay()
{
    delete ui;
}

void DownloadDisplay::appendDownloadProfile(DownloadProfile *dp)
{

    this->downloads.append(dp);
    this->vl_download->addWidget(dp);
    this->repaint();
    emit profileCreated(dp);
}
/*!
 * \brief DownloadDisplay::updateDownloadProfile searches for the download widget in the QVector<T> downloads, modifies its values and updates the window
 *        If there is no corresponding profile, we break and raise an exception (TODO)
 * \param args
 */
void DownloadDisplay::updateDownloadProfile(QStringList args)
{
    QString filename = args[0]; // Reminder : args[0] => filename
    DownloadProfile *effectiveDownloadProfile;
    qDebug() << downloads.length();
    for(DownloadProfile *dp : downloads)
    {
        qDebug() << args[0];
        if(dp->getFilename() == args[0])
        {
            effectiveDownloadProfile = dp;
            break;
        }
    }
    if (effectiveDownloadProfile == NULL)
    {
        // TODO : raise and handle exception
        return;
    }
    else
    {
        effectiveDownloadProfile->updateData(args);
    }
}
