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
    auto&& [value, isGetValue] = settings.getValue("numberOfPau");

    if(isGetValue){
        toStatusBar(value);
    }
    else{
        toStatusBar("qwerty");
        settings.setValue("numberOfPaths", "14");
        settings.save();
        auto&& [v, is] = settings.getValue("numberOfPaths");
        if(is){
            toStatusBar("ok");
        }
        else{
            toStatusBar("be");
        }
    }
}




