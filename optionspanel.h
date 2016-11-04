#ifndef OPTIONSPANEL_H
#define OPTIONSPANEL_H

#include <QMainWindow>

namespace Ui {
class OptionsPanel;
}

class OptionsPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit OptionsPanel(QWidget *parent = 0);
    ~OptionsPanel();

private:
    Ui::OptionsPanel *ui;
};

#endif // OPTIONSPANEL_H
