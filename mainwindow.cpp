#include "mainwindow.h"
#include <qstringliteral.h>
#include <QFileDialog>
#include <QStringListModel>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <map>

///
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings.logs.pushAndFlash("MainWindow Initializated", WSTR::AppType::Debug);

    settings.load();

    variableInitialization();

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->show();

}

///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{
    delete ui;
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
        auto&& [countPaths_i, isCountPaths_s] = settings.toType<int>(countPaths_s);
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

            auto [currentIndex, isCurrentIndex] = settings.toType<int>(currentPathIndex_s);

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
/// \brief MainWindow::clearTable
/// \param table
///
void MainWindow::clearTable(QTableWidget *table) const
{
    if(!table) return;
    table->setColumnCount(0);
    table->setRowCount(0);
    table->clearContents();
}

///
/// \brief MainWindow::replaysToTable
/// \param table
/// \param vec
/// \return
///
 bool MainWindow::replaysToTable(QTableWidget& table, const std::vector<WSTR::Replay> &vec)
{
    if(vec.size() == 0) return false;
    table.clear();

    table.setRowCount(static_cast<int>(vec.size()));
    table.setColumnCount(settings.getCountHeaderList());

    auto&& [headerMap, isHeaderMap] = setHeaderInTable(table);
    if(!isHeaderMap) return false;
    setCurrentItem(table, vec, headerMap);
    return true;
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
    if(dir.isEmpty()) return;

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

    clearTable(ui->tableWidget);
}




///
/// \brief MainWindow::on_paths_activated
/// \param index
///
void MainWindow::on_paths_activated(int index)
{
    settings.setValue("currentPathIndex", std::to_string(index));
    settings.save();
    clearTable(ui->tableWidget);
}



void MainWindow::on_checkBox_clicked()
{
#if 0
    //
    auto listFiles { scanDirectory(ui->paths->itemText(ui->paths->currentIndex())) };

    for(int i{1}; i <= 10; ++i){
        for(auto&& elem: listFiles){

            auto from{ elem.absoluteFilePath().toStdString() };

            std::ifstream fin{ from, std::ios::binary };

            if(!fin.is_open()){
                std::cout << "can not open file: " << from << std::endl;
                continue;
            }
            auto splitName{ elem.fileName().split(".") };
            splitName.insert(1, QString::fromStdString("_"+std::to_string(i)+"."));
            auto dir{ elem.absoluteDir().path().toStdString() + "/" };

            auto to{ dir + splitName.join("").toStdString() };
            std::ofstream fout{to , std::ios::binary };

            bool isCreate{};
            if(fout.is_open()){
                fout << fin.rdbuf();
                if(fout.good()) isCreate = true;
            }

            std::cout << "status fileopen: " << std::boolalpha << isCreate << " filename: " << to << std::endl;
            fin.close();
            fout.close();

        }
    }
#endif

}

///
/// \brief MainWindow::on_runScan_clicked
///
void MainWindow::on_runScan_clicked()
{
    clearTable(ui->tableWidget);
    QString filters{ "*.wotreplay" };
    auto listFiles { scanDirectory(ui->paths->itemText(ui->paths->currentIndex()), filters) };

    if(listFiles.size() == 0){
        toStatusBar(("Файлы \""+filters+"\" в данной папке отсутствуют!").toStdString());
        return;
    }

    std::vector<WSTR::Replay> vecReplays(listFiles.size());

    if(vecReplays.size() == 0){
        toStatusBar(("Файлы \""+filters+"\" в данной папке отсутствуют!").toStdString());
        return;
    }

    ui->tableWidget->setEnabled(false);
    {
        size_t i{};
        std::for_each(std::begin(vecReplays), std::end(vecReplays), [&listFiles, &i](auto&& elem){

            elem = WSTR::Replay(listFiles[i].absoluteFilePath().toStdString(), i);
            ++i;
        });
    }
    replaysToTable(*ui->tableWidget, vecReplays);

    ui->tableWidget->setEnabled(true);


}

