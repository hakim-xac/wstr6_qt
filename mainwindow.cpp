#include "mainwindow.h"
#include <qstringliteral.h>
#include <QFileDialog>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings.logs.pushAndFlash("MainWindow Initializated", WSTR::AppType::Debug);

    settings.load();

    variableInitialization();

}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::on_runScan_clicked
///
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
        settings.setValue("countOfPaths"s, sd);
        settings.save();
        auto&& [v, is] = settings.getValue("countOfPaths");
        if(is){
            toStatusBar("ok");
        }
        else{
            toStatusBar("be");
        }
    }

    settings.logs.flush();
}

///
/// \brief MainWindow::variableInitialization
///
void MainWindow::variableInitialization()
{
    using namespace std::string_literals;
    settings.logs.pushAndFlash("start MainWindow::variableInitialization()", WSTR::AppType::Debug);

    initPathsView();


    settings.logs.pushAndFlash("end MainWindow::variableInitialization()", WSTR::AppType::Debug);
}

///
/// \brief MainWindow::initPathsView
/// \param keyInMap_CountPaths
///
void MainWindow::initPathsView()
{
    using namespace std::string_literals;
    settings.logs.pushAndFlash("start MainWindow::initPathsView()", WSTR::AppType::Debug);

    auto&& [countPaths_s, isCountPath] = settings.getValue("countOfPaths"s);
    if(isCountPath){

        settings.logs.pushAndFlash("isCountPath == true", WSTR::AppType::Debug);

        QStringList paths;
        auto&& [countPaths_i, isCountPaths_s] = toType<int>(countPaths_s);
        if(!isCountPaths_s){

            std::stringstream ss{"auto [countPaths_i, isCountPaths_s] = toType<int>(countPaths_s);\n"};
            ss << "isCountPaths_s == false\n";
            ss << "countPaths_i: " << countPaths_i << " isCountPaths_s: " << isCountPaths_s << "\n";
            settings.logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
            return;
        }
        settings.logs.pushAndFlash("isCountPaths_s == true", WSTR::AppType::Debug);

        for(size_t i{}; i < countPaths_i; ++i){
            auto&& [path, isPath] = settings.getValue("path_"s+std::to_string(i), WSTR::SelectBase::Paths);
            if(!isPath) break;
            paths.append(QString::fromStdString(path));
        }

        if(paths.size() > 0) {
            ui->paths->addItems(paths);

            auto [currentPathIndex_s, isCurrentPathIndex] = settings.getValue("currentPathIndex"s);
            if(isCurrentPathIndex){

            auto [currentIndex, isCurrentIndex] = toType<int>(currentPathIndex_s);
            if(isCurrentIndex) ui->paths->setCurrentIndex(currentIndex);
                settings.save();
            }
            return;
        }
        settings.logs.pushAndFlash("paths.size() == 0", WSTR::AppType::Debug);
    }
    settings.logs.pushAndFlash("isCountPath == false", WSTR::AppType::Debug);

    settings.setValue("path_0", settings.getDefaultPath().toStdString(), WSTR::SelectBase::Paths);
    settings.save();
    ui->paths->addItem(settings.getDefaultPath());

    settings.logs.pushAndFlash("end MainWindow::initPathsView()", WSTR::AppType::Debug);

}

///
/// \brief MainWindow::on_pushButton_clicked
///
void MainWindow::on_pushButton_clicked()
{
    QString currentDir{ ui->paths->itemText(ui->paths->currentIndex()) };

    QString dir{ QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    currentDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks) };
    auto findIndex{ ui->paths->findText(dir) };

    if(findIndex != -1){
        ui->paths->setCurrentIndex(findIndex);
        toStatusBar(ui->paths->itemText(findIndex).toStdString());
    }
    else{
        ui->paths->addItem(dir);
        ui->paths->setCurrentIndex(ui->paths->count()-1);
    }
    settings.setValue("countOfPaths", std::to_string(ui->paths->count()));
    settings.setValue("currentPathIndex", std::to_string(ui->paths->currentIndex()));

    settings.PathFromQComboBoxToPathsBufer(*ui->paths);
    settings.save();
}





void MainWindow::on_paths_activated(int index)
{
    settings.setValue("currentPathIndex", std::to_string(index));
    settings.save();
}

