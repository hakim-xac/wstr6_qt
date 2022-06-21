#include "mainwindow.h"
#include <qstringliteral.h>
#include <QFileDialog>
#include <QStringListModel>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <map>
#include <QThread>
#include <thread>
#include <functional>
#include <chrono>

///
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , countThreads_(std::thread::hardware_concurrency())    
{

    using settings = WSTR::Settings;

    ui->setupUi(this);
    WSTR::Logs::pushAndFlash("MainWindow Initializated", WSTR::AppType::Debug);

    if(!settings::load()){
        settings::loadFromDefault();

        toThreadStatusBar("Загрузка из файла конфигурации не удалась! Загружены данные по умолчанию!",ui->statusBar, 5);
    }

    variableInitialization();

    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));

}

///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{
    WSTR::Settings::save();
    delete ui;
}

///
/// \brief MainWindow::variableInitialization
///
void MainWindow::variableInitialization()
{
    WSTR::Logs::pushAndFlash("start MainWindow::variableInitialization()", WSTR::AppType::Debug);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->show();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // prohibition of editing
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->progressBar->setValue(0);

    if(!initPathsView()){
        toThreadStatusBar("Не удалось Загрузить данные из файла настроек!", ui->statusBar, 5);
        WSTR::Logs::pushAndFlash("if(!initPathsView()) == true", WSTR::AppType::Debug);
        WSTR::Logs::pushAndFlash("break MainWindow::variableInitialization()", WSTR::AppType::Debug);
        return;
    }

    toThreadStatusBar("ХАЙ!", ui->statusBar, 5);

    WSTR::Logs::pushAndFlash("end MainWindow::variableInitialization()", WSTR::AppType::Debug);
}

///
/// \brief MainWindow::initPathsView
/// \return
///
bool MainWindow::initPathsView()
{
    using namespace std::string_literals;

    using settings = WSTR::Settings;
    using fieldNames = WSTR::FieldNames;

    WSTR::Logs::pushAndFlash("start MainWindow::initPathsView()", WSTR::AppType::Debug);

    auto&& [countPaths_s, isCountPath] = settings::getValue(settings::getFieldName(fieldNames::CountOfPaths));
    if(isCountPath){

        WSTR::Logs::pushAndFlash("isCountPath == true", WSTR::AppType::Debug);

        QStringList paths;
        auto&& [countPaths_i, isCountPaths_s] = settings::toType<int>(countPaths_s);
        if(!isCountPaths_s){

            std::stringstream ss;
            ss << "auto [countPaths_i, isCountPaths_s] = toType<int>(countPaths_s);\n";
            ss << "isCountPaths_s == false\n";
            ss << "countPaths_i: " << countPaths_i << " isCountPaths_s: " << isCountPaths_s << "\n";
            WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
            return false;
        }
        WSTR::Logs::pushAndFlash("isCountPaths_s == true", WSTR::AppType::Debug);

        if(!settings::checkIsRange(0, 50, countPaths_i))  {
            std::stringstream ss;
            ss << "if(!WSTR::Settings::checkIsRange(0, 50, countPaths_i)) == false\n";
            ss << "countPaths_i: " << countPaths_i << "\n";
            WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);
            return false;
        }

        for(size_t i{}; i < countPaths_i; ++i){
            auto&& [path, isPath] = settings::getValue("path_"s+std::to_string(i), WSTR::SelectBase::Paths);
            if(!isPath) break;
            paths.append(QString::fromStdString(path));
        }

        if(paths.size() > 0) {
            ui->paths->addItems(paths);

            auto [currentPathIndex_s, isCurrentPathIndex] = settings::getValue(settings::getFieldName(fieldNames::CurrentPathIndex));
            if(isCurrentPathIndex){

            auto [currentIndex, isCurrentIndex] = settings::toType<int>(currentPathIndex_s);
            ui->paths->setCurrentIndex(0);

            if(isCurrentIndex && settings::checkIsRange(0, static_cast<int>(paths.size()) - 1, currentIndex)) {
                ui->paths->setCurrentIndex(currentIndex);
            }

            settings::save();
            }
            return true;
        }
        WSTR::Logs::pushAndFlash("paths.size() == 0", WSTR::AppType::Debug);
    }


    std::stringstream ss;
    ss << "isCountPath == false\n";
    ss << "countPaths_s == " << countPaths_s << "\n";
    WSTR::Logs::pushAndFlash(ss.str(), WSTR::AppType::Debug);

    settings::setValue("path_0", WSTR::Settings::getDefaultPath().toStdString(), WSTR::SelectBase::Paths);
    settings::setValue(settings::getFieldName(fieldNames::CountOfPaths), "1", WSTR::SelectBase::General);

    settings::save();
    ui->paths->addItem(settings::getDefaultPath());

    WSTR::Logs::pushAndFlash("end MainWindow::initPathsView()", WSTR::AppType::Debug);

    return false;
}

///
/// \brief MainWindow::clearTable
/// \param table
///
void MainWindow::clearTable(QTableWidget *table)
{

    if(!table){
        WSTR::Logs::pushAndFlash("void MainWindow::clearTable(QTableWidget *table) \
(table == nullptr) == true", WSTR::AppType::Debug);
        return;
    }
    ui->allCount->setText("0");
    ui->allValidity->setText("0");
    table->setColumnCount(0);
    table->setRowCount(0);
    table->clearContents();
    ui->progressBar->setValue(0);
}

///
/// \brief MainWindow::toThreadStatusBar
/// \param str
/// \param label
///
void MainWindow::toThreadStatusBar(QString&& str, QLabel* const label, int waitSec)
{
    using time = std::chrono::high_resolution_clock;
    using ms = std::chrono::milliseconds;
    using settings = WSTR::Settings;
    using fieldNames = WSTR::FieldNames;

        if(!label){
            WSTR::Logs::pushAndFlash("void MainWindow::toThreadStatusBar(QString&& str, QLabel* const label, int waitSec) \
    (label == nullptr) == true", WSTR::AppType::Debug);
            return;
        }


        static auto tm{ time::now() };
        static bool isRun{};
        auto now{ time::now() };

        auto&& [waitUpdateStatusBar_s, isWait] = settings::getValue<size_t>(WSTR::Settings::getFieldName(WSTR::FieldNames::WaitUpdateStatusBar_s));
        if(!isWait) waitUpdateStatusBar_s = 1;
        if((std::chrono::duration_cast<ms>(now - tm)).count() < waitUpdateStatusBar_s * 1000 && isRun) return;

        tm = now;
        if(waitSec <= 0) waitSec = 1;

        std::thread thread(showStatusBar, str, label, waitSec);
        thread.detach();

        isRun = true;

}

///
/// \brief MainWindow::showStatusBar
/// \param str
/// \param label
///
void MainWindow::showStatusBar(const QString &str, QLabel* const label, int waitSec)
{
    if(!label){
        WSTR::Logs::pushAndFlash("void MainWindow::showStatusBar(const QString &str, QLabel* const label, int waitSec) \
(label == nullptr) == true", WSTR::AppType::Debug);
        return;
    }

    label->setText(str);
    QThread::sleep(waitSec);
    label->setText(QString());
}

std::pair<std::vector<WSTR::Replay>, bool> MainWindow::createVectorWotReplays(const QList<QFileInfo>& listFiles)
{
    if(!listFiles.size()){

    WSTR::Logs::pushAndFlash("std::pair<std::vector<WSTR::Replay>, bool> MainWindow::createVectorWotReplays(const QList<QFileInfo>& listFiles) \
    if(!listFiles.size()) == true", WSTR::AppType::Debug);

        return { {}, false };
    }

    ui->runScan->setEnabled(false);

    size_t size{ static_cast<size_t>(listFiles.size()) };

    size_t minThreads{ std::min(static_cast<size_t>((countThreads_ ? countThreads_ - 1 : 2)), size) };

    std::vector<std::thread> threads;
    threads.reserve(minThreads);

    std::vector<WSTR::Replay> vecReplays(size);

    size_t part{ std::max(size / minThreads, size_t(1)) };

    using iter = decltype(vecReplays.begin());
    using type_paths = decltype(listFiles);

    for(size_t i{}, ie{ minThreads }; i != ie; ++i){

        size_t begin{ part * i };
        size_t end{ part * (i + 1) };

        if(i == ie - 1) end = size;

        threads.emplace_back(std::thread(&MainWindow::addToThread<iter, type_paths>
                                         , vecReplays.begin()
                                         , vecReplays.begin() + begin
                                         , vecReplays.begin() + end
                                         , std::cref(listFiles)));
    }

    for(auto&& elem: threads){
        if(elem.joinable()) elem.join();
    }

    ui->runScan->setEnabled(true);

    return { vecReplays, true };
}



/////
/// \brief MainWindow::addToThread
/// \param first
/// \param begin
/// \param end
/// \param basePaths
///
template< class TypeVecIter, class TypePaths>
void MainWindow::addToThread(TypeVecIter first, TypeVecIter begin, TypeVecIter end, const TypePaths& basePaths){

    if(end - begin <= 0) return;
    for(auto it{ begin }, ite{ end }; it != ite; ++it){
        auto distance{ std::distance(first, it) };
        *it = std::move(WSTR::Replay(basePaths[distance].absoluteFilePath().toStdString(), distance));
    }
}

///
/// \brief MainWindow::on_pushButton_clicked
///
void MainWindow::on_pushButton_clicked()
{

    using settings = WSTR::Settings;
    using fieldNames = WSTR::FieldNames;

    QString currentDir{ ui->paths->itemText(ui->paths->currentIndex()) };

    QString dir{ QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    currentDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks) };
    if(dir.isEmpty()) return;

    auto findIndex{ ui->paths->findText(dir) };

    if(findIndex != -1){
        ui->paths->setCurrentIndex(findIndex);
    }
    else{
        ui->paths->addItem(dir);
        ui->paths->setCurrentIndex(ui->paths->count()-1);
    }

    settings::setValue(settings::getFieldName(fieldNames::CountOfPaths), std::to_string(ui->paths->count()));
    settings::setValue(settings::getFieldName(fieldNames::CurrentPathIndex), std::to_string(ui->paths->currentIndex()));

    settings::PathFromQComboBoxToPathsBufer(*ui->paths);
    settings::save();

    clearTable(ui->tableWidget);

    toThreadStatusBar("Каталог успешно выбран!", ui->statusBar);
}




///
/// \brief MainWindow::on_paths_activated
/// \param index
///
void MainWindow::on_paths_activated(int index)
{
    using settings = WSTR::Settings;
    using fieldNames = WSTR::FieldNames;

    settings::setValue(settings::getFieldName(fieldNames::CurrentPathIndex), std::to_string(index));
    //settings.save();
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

    using replay = WSTR::Replay;
    using settings = WSTR::Settings;
    using fildNames = WSTR::FieldNames;

    clearTable(ui->tableWidget);

    QString filters{ "*.wotreplay" };

    auto listFiles { scanDirectory(ui->paths->itemText(ui->paths->currentIndex()), filters) };

    if(listFiles.size() == 0){
        toThreadStatusBar(QString("Файлы \""+filters+"\" в данной папке отсутствуют!"), ui->statusBar);
        return;
    }

    //*  BE SURE TO CALL !!!  *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    replay::clearCounts(); // if not called, then the result of "valid replays" is incorrect.

    assert(replay::getCount() == 0 && replay::getCountValidity() == 0);

    ui->tableWidget->setEnabled(false);
    ui->progressBar->setValue(0);

    auto&& [vecReplays, isVecReplays] = createVectorWotReplays(listFiles);

    if(!isVecReplays) {
        WSTR::Logs::pushAndFlash("    auto&& [vecReplays, isVecReplays] = createVectorWotReplays(listFiles);\
                                   if(!isVecReplays) == false", WSTR::AppType::Debug);
        toThreadStatusBar("Неизвестная Ошибка!", ui->statusBar);

        clearTable(ui->tableWidget);
        return;
    }

    ui->progressBar->setValue(50);
    replaysToTableThreads(*ui->tableWidget, vecReplays);
    ui->progressBar->setValue(75);


    auto&& [activeColumn, isActiveColumn] = settings::getValue<size_t>(settings::getFieldName(fildNames::ActiveColumn));
    auto&& [typeSortColumns, isTypeSortColumns] = settings::getValue<bool>(settings::getFieldName(fildNames::TypeSortColumns));

    Qt::SortOrder typeSort{ Qt::SortOrder::AscendingOrder };
    if(isTypeSortColumns && typeSortColumns) typeSort = Qt::SortOrder::DescendingOrder;
    // sort by index or zero
    if(isActiveColumn) ui->tableWidget->sortByColumn(activeColumn, typeSort);
    else ui->tableWidget->sortByColumn(0, typeSort);


    ui->progressBar->setValue(100);
    toThreadStatusBar("Обновлено!", ui->statusBar);
    ui->tableWidget->setEnabled(true);


}


void MainWindow::on_checkBox_stateChanged(int arg1)
{


}


void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    std::cout << ".column: " << item->column() << " row: " << item->row() << std::endl;
}

void MainWindow::headerClicked(int index)
{
    using settings = WSTR::Settings;
    using fildNames = WSTR::FieldNames;

    auto&& [typeSortColumns, isTypeSortColumns] = settings::getValue<bool>(settings::getFieldName(fildNames::TypeSortColumns));

    Qt::SortOrder typeSort{ Qt::SortOrder::DescendingOrder };
    if(isTypeSortColumns && typeSortColumns) typeSort = Qt::SortOrder::AscendingOrder;

    settings::setValue("typeSortColumns", std::to_string(!typeSortColumns));

    auto&& [activeColumn, isActiveColumn] = settings::getValue<size_t>(settings::getFieldName(fildNames::ActiveColumn));
    if(!isActiveColumn) {

        WSTR::Logs::pushAndFlash("auto&& [activeColumn, isActiveColumn] = settings::getValue<size_t>(settings::getFieldName(fildNames::ActiveColumn));\
                                 if(!isActiveColumn) == false", WSTR::AppType::Debug);

        ui->tableWidget->sortByColumn(0, typeSort);
        return;
    }

    if(!settings::checkIsRange(0ull, settings::getCountHeaderList(), activeColumn)){

        WSTR::Logs::pushAndFlash("if(!settings::checkIsRange(0ull, settings::getCountHeaderList(), activeColumn)) == false", WSTR::AppType::Debug);
        ui->tableWidget->sortByColumn(0, typeSort);
        return;
    }

    ui->tableWidget->sortByColumn(index, typeSort);

    settings::setValue("activeColumn", std::to_string(index));

}


