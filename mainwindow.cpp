#include "mainwindow.h"
#include <qstringliteral.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings.load();
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
        using namespace std::string_literals;
        toStatusBar("qwerty");
        settings.setValue("numberOfPaths"s, "14"s);
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




