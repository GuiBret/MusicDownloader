#include "optionspanel.h"
#include "ui_optionspanel.h"

OptionsPanel::OptionsPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OptionsPanel)
{
    ui->setupUi(this);
}

OptionsPanel::~OptionsPanel()
{
    delete ui;
}
