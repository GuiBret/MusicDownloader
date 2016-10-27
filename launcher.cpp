#include "launcher.h"
#include "ui_launcher.h"

typedef qint64 Q_PID;

Launcher::Launcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Launcher)
{
    ui->setupUi(this);
    downloads = new DownloadDisplay(this);
    downloads->hide();
    this->download_started = false;
    QClipboard *cb = QApplication::clipboard();
    connect(cb, SIGNAL(dataChanged()), this, SLOT(checkClipboard()));

    connect(ui->btn_exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->btn_browselocation, SIGNAL(clicked()), this, SLOT(browseFileLocation()));
    connect(ui->btn_url, SIGNAL(clicked()), this, SLOT(checkUrlValidity()));
    connect(ui->btn_download, SIGNAL(clicked()), this, SLOT(downloadFile()));
    connect(this, SIGNAL(validUrl()), this, SLOT(searchThumbnail()));

    ui->widget_location->setEnabled(false);
    ui->widget_buttons->setEnabled(false);
}

Launcher::~Launcher()
{
    delete ui;
}

void Launcher::browseFileLocation()
{
    QString filename = QFileDialog::getExistingDirectory(this, "File location", qApp->applicationDirPath());
    this->ui->le_filelocation->setText(filename);
}

void Launcher::checkUrlValidity()
{
    QProcess p;
    QString url = ui->le_url->text();
    QString command = "youtube-dl -e "+ url;

    p.start(command);
    p.waitForFinished();

    QString output = p.readAll();
    QString error = p.readAllStandardError();

    if(error != "")
    {
        this->ui->lbl_error->setText("This URL is not valid. Please retry.");
    }
    else
    {
        this->ui->lbl_error->setText("Video name : "+ output);
        this->videoName = output;
        this->ui->widget_location->setEnabled(true);
        ui->widget_buttons->setEnabled(true);
        this->videoUrl = url;
        emit validUrl();
    }
}

void Launcher::downloadFile()
{
    QString selectedCodec = ui->cb_codec->currentText();
    QString path = this->ui->le_filelocation->text();
    QString filename = this->ui->lbl_error->text().remove("Video name : ");
    path = path + "/" + filename+ "." + selectedCodec;

    if(Utils::checkFileLocation(path))
        QMessageBox::information(this, "Error", "The file has already been downloaded.");
    else
    {

        process = new MyProcess(this);


        connect(process, SIGNAL(infoSent(QStringList)), this, SLOT(createProfile(QStringList)));
        connect(process, SIGNAL(downloadInfoSent(QStringList)), this, SLOT(updateProfile(QStringList)));
        connect(downloads, SIGNAL(profileCreated(DownloadProfile*)), process, SLOT(relaunchDownload(DownloadProfile*)));

        QString codec = this->ui->cb_codec->currentText();
        QString path = ui->le_filelocation->text()+"/"+videoName;
        QStringList args;


        args << this->videoUrl << codec << path;
        process->downloadFile(args);
    }

}

void Launcher::createProfile(QStringList info)
{
    DownloadProfile *dp = new DownloadProfile(info, this->downloads);
}
/*!
 * \brief Launcher::checkClipboard checks if the clipboard text is a YT link and puts it in the URL lineedit if it is
 */
void Launcher::checkClipboard()
{
    if(Utils::checkYoutubeLink(cb->text()))
    {
        qDebug() <<cb->text();
        this->ui->le_url->setText(cb->text());
    }
}

void Launcher::searchThumbnail()
{
    QString url = this->ui->le_url->text();
    QPixmap image = Utils::getThumbnail(url);
    this->ui->lbl_image->setPixmap(Utils::getThumbnail(process->getThumbnailUrl(url)));
    qDebug() << "Hello";
    this->ui->lbl_image->update();
}
