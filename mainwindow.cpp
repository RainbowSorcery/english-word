#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wordmanage.h"
#include "studyview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete study;
    delete manage;
}


void MainWindow::on_pushButton_clicked()
{
    manage = new wordManage();
    manage->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    study = new StudyView();

    study->show();
}

