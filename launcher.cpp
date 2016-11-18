#include "launcher.h"
#include "ui_launcher.h"


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
    connect(ui->act_showDownload, SIGNAL(triggered(bool)), downloads, SLOT(show()));

    ui->widget_location->setEnabled(false);
    ui->widget_buttons->setEnabled(false);


}

Launcher::~Launcher()
{
    delete downloads;
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
        this->currentFileName = output;
        emit validUrl();
    }
}

void Launcher::downloadFile()
{
    QString selectedCodec = ui->cb_codec->currentText();
    QString path = this->ui->le_filelocation->text();
    QString filename = this->ui->lbl_error->text().remove("Video name : ");
    path = path + ROOT + filename+ "." + selectedCodec;

    if(Utils::checkFileLocation(path))
        QMessageBox::information(this, "Error", "The file has already been downloaded.");
    else
    {
        Download *currentDownload = new Download(this);
        emit downloadInstanceCreated(currentDownload); // Updates the download window content in real time
        this->downloadList.append(currentDownload);
        currentDownload->startDownload();
    }

}

/*!
 * \brief Launcher::checkClipboard checks if the clipboard text is a YT link and puts it in the URL lineedit if it is
 */
void Launcher::checkClipboard()
{
    // TODO : fix regex
    if(Utils::checkYoutubeLink(cb->text()))
    {
        this->ui->le_url->setText(cb->text());
        this->searchThumbnail();
    }
}

void Launcher::searchThumbnail()
{
    QString url = this->ui->le_url->text();
    this->ui->lbl_image->setPixmap(Utils::getThumbnail(this->getThumbnailUrl(url)));
    this->ui->lbl_image->update();
}

QString Launcher::getCodec()
{
    return this->ui->cb_codec->currentText();
}

QString Launcher::getUrl()
{
    return this->ui->le_url->text();
}

QString Launcher::getPath()
{
    return this->ui->le_filelocation->text();
}

QString Launcher::getCurrentFileName()
{
    return this->currentFileName;
}


void Launcher::handleDownloadFinished()
{
    downloads->enableActions(this->currentFileName);
}



Download *Launcher::searchDownload(QString filename)
{
    Download *d;
    for(Download *download : downloadList)
    {
        if(download->getFilename() == filename)
        {
            d = download;
            break;
        }
    }

    return d;
}
// To be moved in another class
QUrl Launcher::getThumbnailUrl(QString url)
{
    QEventLoop el;
    QProcess p;
    connect(&p, SIGNAL(finished(int)), &el, SLOT(quit()));
    p.start("youtube-dl --get-thumbnail "+ url);
    el.exec();

    QString thumbnailUrl = p.readAllStandardOutput();
    return QUrl(thumbnailUrl.simplified());
}

QVector<Download*> Launcher::getDownloadList()
{
    return this->downloadList;
}
