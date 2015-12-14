#include "moreinfocomputer.h"
#include "ui_moreinfocomputer.h"
#include "utilities/constants.h"
#include "data_types/computer.h"

using namespace std;

MoreInfoComputer::MoreInfoComputer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoreInfoComputer)
{
    ui->setupUi(this);
    displayInfo();

}

MoreInfoComputer::~MoreInfoComputer()
{
    delete ui;
}

void MoreInfoComputer::displayInfo(){

    computer = mainwindow.returnCurrentlySelectedComputer();

    ui->label_name->setText(QString::fromStdString(constants::COMP_NAME));

}

void MoreInfoComputer::on_pushButton_close_clicked()
{

}
