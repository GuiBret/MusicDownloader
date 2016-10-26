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

    connect(ui->btn_exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->btn_browselocation, SIGNAL(clicked()), this, SLOT(browseFileLocation()));
    connect(ui->btn_url, SIGNAL(clicked()), this, SLOT(checkUrlValidity()));
    connect(ui->btn_download, SIGNAL(clicked()), this, SLOT(downloadFile()));
    //connect(ui->act_showDownload, SIGNAL(triggered(bool)), downloads, SLOT(show()));

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
    }
}

void Launcher::downloadFile()
{
    QString selectedCodec = ui->cb_codec->currentText();
    QString path = this->ui->le_filelocation;
    QString filename = this->ui->lbl_error->text().remove("Video name : ");
    path << "/" << filename << "." << selectedCodec;
    QEventLoop el;
    if(!Utils::checkFileLocation(path))
        QMessageBox::information(this, "Error", "The file has already been downloaded.");
    else
    {

        process = new MyProcess(this);


        connect(process, SIGNAL(infoSent(QStringList)), this, SLOT(createProfile(QStringList)));
        connect(process, SIGNAL(downloadInfoSent(QStringList)), this, SLOT(updateProfile(QStringList)));
        connect(process, SIGNAL(downloadFinished()), &el, SLOT(quit()));
        connect(downloads, SIGNAL(profileCreated(DownloadProfile*)), process, SLOT(relaunchDownload(DownloadProfile*)));

        QString codec = this->ui->cb_codec->currentText();
        QString path = ui->le_filelocation->text()+"/"+videoName;
        QStringList args;


        args << this->videoUrl << codec << path;
        process->downloadFile(args);
        el.quit();
    }

}



void Launcher::parseError()
{
    QMessageBox::critical(this, "Error", process->readAllStandardError());
}

void Launcher::disconnectSlots()
{
    disconnect(process, SIGNAL(readyRead()), this, SLOT(parseError()));
}

void Launcher::createProfile(QStringList info)
{

    //connect(downloads, SIGNAL(profileCreated(DownloadProfile*)), process, SLOT(relaunchDownload(DownloadProfile*)));

    DownloadProfile *dp = new DownloadProfile(info, this->downloads);
    //qDebug() << "createProfile";

}
