#include "launcher.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    if(Utils::checkYoutubeDlInstall())
    {
        Launcher w;
        w.show();

        return a.exec();
    }
    else
    {
        QMessageBox::information(NULL, "Error", "Youtube-dl is not installed in your computer. Please install it before restarting this program.");
        return 0;
    }
}
