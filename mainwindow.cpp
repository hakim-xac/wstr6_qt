#include "mainwindow.h"
#include <qstringliteral.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings.logs.pushAndFlash("MainWindow Initializated", WSTR::AppType::Debug);

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
        std::string sd{"7"};
        settings.setValue("numberOfPaths"s, sd);
        settings.save();
        auto&& [v, is] = settings.getValue("numberOfPaths");
        if(is){
            toStatusBar("ok");
        }
        else{
            toStatusBar("be");
        }
    }

    settings.logs.flush();
}


void MainWindow::variableInitialization()               ////////////TODO
{
    using namespace std::string_literals;

    auto&& [countPaths, isCountPaths] = settings.getValue("numberOfPaths"s);


    auto [countPaths_i, isCountPath] = toType<int>(countPaths);
    if(isCountPath){
        settings.setValue("numberOfPaths"s, countPaths);
    }
            //for (auto i{ 0 }; ; ) {

    //}
}




