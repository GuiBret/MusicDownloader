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


    process = new MyProcess(this);
    connect(process, SIGNAL(infoSent(QStringList)), this, SLOT(createProfile(QStringList)));
    QString codec = this->ui->cb_codec->currentText();
    QString path = ui->le_filelocation->text()+"/"+videoName;
    QStringList args;


    args << this->videoUrl << codec << path;
    process->downloadFile(args);
    process->waitForFinished(-1);


    /*
    //connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseOutput()));
    connect(process, SIGNAL(readyReadStandardError()), this, SLOT(parseError()));
    connect(this, SIGNAL(downloadFinished()), &el, SLOT(quit()));
    connect(this, SIGNAL(downloadFinished()), this, SLOT(disconnectSlots()));
    QString command = "youtube-dl --extract-audio --audio-format "+ selectedCodec+" -o "+ "\""+ path.simplified() +"."+ selectedCodec+"\"" +" "+videoUrl;

    process->start(command);
    qDebug() << process->pid();
    el.exec();
    */
    //QMessageBox::information(this, "Yay", "Loop unlocked");

}


void Launcher::parseOutput()
{
    QString output = process->readAllStandardOutput().simplified();
    bool download_finished = false;
    //for()

    qDebug() << output;

    if(output.startsWith("[download]"))
    {
        QStringList splittedOutput = output.split("[download]"); // Handle multiple lines
        splittedOutput.removeAt(0); // Remove the fake first line created by [download] at the beginning
        for(QString out : splittedOutput)
        {
            qDebug() << download_started;
            if(!out.contains("Destination"))
            {
                out.replace("of", "|").replace("at", "|").replace("ETA", "|").replace(" ", "").simplified();

                QStringList outputArgs = out.split("|");
                qDebug() << outputArgs;

                // If the download is not considered as started yet, we create a new download widget to be added in the downloads window
                if(!this->download_started)
                {
                    QString codec = ui->cb_codec->currentText();

                    outputArgs.insert(0, videoName);
                    outputArgs.append(codec);
                    //QMessageBox::information(this, "Hello", "Profile created");
                    process->pause();
                    qDebug() << process->pid();
                    this->download_started = true;
                    DownloadProfile *dp = new DownloadProfile(outputArgs, downloads);
                    connect(dp, SIGNAL(profileCreated()), process, SLOT(restart()));
                    downloads->appendDownloadProfile(dp);
                    downloads->show();
                    qDebug() << process->state();


                }
                else
                {
                    if (outputArgs[0] == "100%")
                    {

                        download_started = false;
                        disconnectSlots();
                        break;
                    }
                    else
                    {
                        //QMessageBox::information(this, "Hello", "updating profile");
                        downloads->updateDownloadProfile(outputArgs);
                    }
                }
            }
        }
        //MessageBox::information(this, "", "Coucou");

    }
    else
    {
        //qDebug() << output;
    }

    if(download_finished)
    {
        QMessageBox::information(this, "Download finished", "The download is finished.");
        process->terminate();
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
    DownloadProfile *dp = new DownloadProfile(info, this->downloads);

}
