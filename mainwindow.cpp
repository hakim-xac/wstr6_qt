#include "mainwindow.h"
#include <qstringliteral.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_runScan_clicked()
{
    if(settings.save()){
        toStatusBar("good!");
    }
    else{
        toStatusBar("fail!");
    }
}




