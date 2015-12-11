#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities/constants.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dropdown_pioneers_order_by->addItem("Name");
    ui->dropdown_pioneers_order_by->addItem("Birth Year");
    ui->dropdown_computers_order_by->addItem("Name");
    ui->dropdown_computers_order_by->addItem("Build Year");
    ui->dropdown_pioneers_order_direction->addItem("Ascending");
    ui->dropdown_pioneers_order_direction->addItem("Descending");
    ui->dropdown_computers_order_direction->addItem("Ascending");
    ui->dropdown_computers_order_direction->addItem("Descending");

    displayAllPioneers();
    displayAllComputers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayAllPioneers()
{
    vector<Pioneer> pioneers = pioneerService.getList();
    displayPioneers(pioneers);
}

void MainWindow::displayAllComputers()
{
    vector<Computer> computers = computerService.getList();
    displayComputers(computers);
}

void MainWindow::displayPioneers(std::vector<Pioneer> pioneers)
{
    ui->list_pioneers->clear();

    for(unsigned int i = 0; i < pioneers.size(); i++){
        Pioneer currentPioneer = pioneers[i];

        ui->list_pioneers->addItem(QString::fromStdString(currentPioneer.getName()));
    }
}

void MainWindow::displayComputers(std::vector<Computer> computers)
{
    ui->list_computers->clear();

    for(unsigned int i = 0; i < computers.size(); i++){
        Computer currentComputer = computers[i];

        ui->list_computers->addItem(QString::fromStdString(currentComputer.getComputerName()));
    }
}

void MainWindow::on_input_search_pioneers_textChanged()
{
    string userInput = ui->input_search_pioneers->text().toStdString();

    // TO DO: change search function to take in name of column to search for instead of integer
    vector<Pioneer> pioneers = pioneerService.search(userInput, 1);
    displayPioneers(pioneers);
}

void MainWindow::on_input_search_computers_textChanged()
{
    string userInput = ui->input_search_computers->text().toStdString();

    // TO DO: change search function to take in name of column to search for instead of integer
    vector<Computer> computers = computerService.search(userInput, 1);
    displayComputers(computers);
}

void MainWindow::on_dropdown_pioneers_order_by_currentIndexChanged(int index)
{

}

void MainWindow::on_dropdown_computers_order_by_currentIndexChanged(int index)
{

}

void MainWindow::on_dropdown_pioneers_order_direction_currentIndexChanged(int index)
{

}

void MainWindow::on_dropdown_computers_order_direction_currentIndexChanged(int index)
{

}

string MainWindow::getCurrentOrderByPioneers()
{
    string orderBy = ui->dropdown_pioneers_order_by->currentText().toStdString();

    if(orderBy == "Name"){
        return constants::NAME;
    }
    else if(orderBy == "Birth Year"){
        return constants::BIRTH_YEAR;
    }
    else{
        return constants::NAME;
    }

}

string MainWindow::getCurrentOrderByComputers()
{
    string orderBy = ui->dropdown_computers_order_by->currentText().toStdString();

    if(orderBy == "Name"){
        return constants::COMP_NAME;
    }
    else if(orderBy == "Build Year"){
        return constants::BUILD_YEAR;
    }
    else{
        return constants::COMP_NAME;
    }
}