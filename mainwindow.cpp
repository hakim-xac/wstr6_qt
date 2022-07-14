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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    using settings = KHAS::Settings;

    ui->setupUi(this);
    KHAS::Logs::pushAndFlash("MainWindow Initializated", KHAS::AppType::Debug);

    if(!settings::load()){
        settings::loadFromDefault();

        toThreadStatusBar("Загрузка из файла конфигурации не удалась! Загружены данные по умолчанию!",ui->statusBar, 5);
    }

    variableInitialization();

    connect(ui->tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));

    ui->statusBar->setText("");
}

MainWindow::~MainWindow()
{
    KHAS::Settings::save();
    delete ui;
}

void MainWindow::variableInitialization()
{
    KHAS::Logs::pushAndFlash("start MainWindow::variableInitialization()", KHAS::AppType::Debug);

    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->horizontalHeader()->show();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // prohibition of editing
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    if(!initPathsView()){
        toThreadStatusBar("Не удалось Загрузить данные из файла настроек!", ui->statusBar, 5);
        KHAS::Logs::pushAndFlash("if(!initPathsView()) == true", KHAS::AppType::Debug);
        KHAS::Logs::pushAndFlash("break MainWindow::variableInitialization()", KHAS::AppType::Debug);
        return;
    }

    KHAS::Logs::pushAndFlash("end MainWindow::variableInitialization()", KHAS::AppType::Debug);
}

bool MainWindow::initPathsView()
{
    using namespace std::string_literals;

    using settings = KHAS::Settings;
    using fieldNames = KHAS::FieldNames;

    KHAS::Logs::pushAndFlash("start MainWindow::initPathsView()", KHAS::AppType::Debug);

    auto&& [countPaths_s, isCountPath] = settings::getValue(settings::getFieldName(fieldNames::CountOfPaths));
    if(isCountPath){

        KHAS::Logs::pushAndFlash("isCountPath == true", KHAS::AppType::Debug);

        QStringList paths;
        auto&& [countPaths_i, isCountPaths_s] = settings::stringToType<int>(countPaths_s);
        if(!isCountPaths_s){

            std::stringstream ss;
            ss << "auto [countPaths_i, isCountPaths_s] = stringToType<int>(countPaths_s);\n";
            ss << "isCountPaths_s == false\n";
            ss << "countPaths_i: " << countPaths_i << " isCountPaths_s: " << isCountPaths_s << "\n";
            KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
            return false;
        }
        KHAS::Logs::pushAndFlash("isCountPaths_s == true", KHAS::AppType::Debug);

        if(!settings::checkIsRange(0, 50, countPaths_i))  {
            std::stringstream ss;
            ss << "if(!KHAS::Settings::checkIsRange(0, 50, countPaths_i)) == false\n";
            ss << "countPaths_i: " << countPaths_i << "\n";
            KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);
            return false;
        }

        for(size_t i{}; i < countPaths_i; ++i){
            auto&& [path, isPath] = settings::getValue("path_"s+std::to_string(i), KHAS::SelectBase::Paths);
            if(!isPath) break;
            paths.append(QString::fromStdString(path));
        }

        if(paths.size() > 0) {
            ui->paths->addItems(paths);

            auto [currentPathIndex_s, isCurrentPathIndex] = settings::getValue(settings::getFieldName(fieldNames::CurrentPathIndex));
            if(isCurrentPathIndex){

            auto [currentIndex, isCurrentIndex] = settings::stringToType<int>(currentPathIndex_s);
            ui->paths->setCurrentIndex(0);

            if(isCurrentIndex && settings::checkIsRange(0, static_cast<int>(paths.size()) - 1, currentIndex)) {
                ui->paths->setCurrentIndex(currentIndex);
            }

            settings::save();
            }
            return true;
        }
        KHAS::Logs::pushAndFlash("paths.size() == 0", KHAS::AppType::Debug);
    }


    std::stringstream ss;
    ss << "isCountPath == false\n";
    ss << "countPaths_s == " << countPaths_s << "\n";
    KHAS::Logs::pushAndFlash(ss.str(), KHAS::AppType::Debug);

    settings::setValue("path_0", KHAS::Settings::getDefaultPath().toStdString(), KHAS::SelectBase::Paths);
    settings::setValue(settings::getFieldName(fieldNames::CountOfPaths), "1", KHAS::SelectBase::General);

    settings::save();
    ui->paths->addItem(settings::getDefaultPath());

    KHAS::Logs::pushAndFlash("end MainWindow::initPathsView()", KHAS::AppType::Debug);

    return false;
}

void MainWindow::clearTable(QTableWidget *table)
{

    if(!table){
        KHAS::Logs::pushAndFlash("void MainWindow::clearTable(QTableWidget *table) \
(table == nullptr) == true", KHAS::AppType::Debug);
        return;
    }
    ui->allCount->setText("0");
    ui->allValidity->setText("0");
    table->setColumnCount(0);
    table->setRowCount(0);
    table->clearContents();
}

void MainWindow::toThreadStatusBar(QString&& str, QLabel* const label, int waitSec)
{
    using time = std::chrono::high_resolution_clock;
    using ms = std::chrono::milliseconds;
    using settings = KHAS::Settings;
    using fieldNames = KHAS::FieldNames;

        if(!label){
            KHAS::Logs::pushAndFlash("void MainWindow::toThreadStatusBar(QString&& str, QLabel* const label, int waitSec) \
    (label == nullptr) == true", KHAS::AppType::Debug);
            return;
        }


        static auto tm{ time::now() };
        static bool isRun{};
        auto now{ time::now() };

        auto&& [waitUpdateStatusBar_s, isWait] = settings::getValue<size_t>(KHAS::Settings::getFieldName(KHAS::FieldNames::WaitUpdateStatusBar_s));
        if(!isWait) waitUpdateStatusBar_s = 1;
        if((std::chrono::duration_cast<ms>(now - tm)).count() < waitUpdateStatusBar_s * 1000 && isRun) return;

        tm = now;
        if(waitSec <= 0) waitSec = 1;

        std::thread thread(showStatusBar, str, label, waitSec);
        thread.detach();

        isRun = true;

}

void MainWindow::showStatusBar(const QString &str, QLabel* const label, int waitSec)
{
    if(!label){
        KHAS::Logs::pushAndFlash("void MainWindow::showStatusBar(const QString &str, QLabel* const label, int waitSec) \
(label == nullptr) == true", KHAS::AppType::Debug);
        return;
    }

    label->setText(str);
    QThread::sleep(waitSec);
    label->setText(QString());
}

std::pair<std::vector<KHAS::Replay>, bool> MainWindow::createVectorWotReplaysThread(const QList<QFileInfo>& listFiles)
{
    std::vector<KHAS::Replay> vecReplays;
    bool isVecReplays{};
    bool isSuccess{};

    std::thread createVectorWotReplaysThread{ &MainWindow::createVectorWotReplays, std::cref(listFiles), std::ref(vecReplays), std::ref(isSuccess), std::ref(isVecReplays) };
    createVectorWotReplaysThread.detach();

    QString text{ "Ожидание" };
    QString newText{ text };
    while(!isSuccess){

        newText += '.';
        ui->statusBar->setText(newText);
        QApplication::processEvents();
        QThread::msleep(100);
        if(newText.size() > text.size() + 3) newText = text;

    }
    ui->statusBar->setText("");

    return { vecReplays, true };
}

void MainWindow::createVectorWotReplays(const QList<QFileInfo>& listFiles, std::vector<KHAS::Replay>& vec, bool& isSuccess, bool& isResult)
{
    isSuccess = false;

    if(!listFiles.size()){

    KHAS::Logs::pushAndFlash("std::pair<std::vector<KHAS::Replay>, bool> MainWindow::createVectorWotReplays(const QList<QFileInfo>& listFiles) \
    if(!listFiles.size()) == true", KHAS::AppType::Debug);

        isSuccess = true;
        vec.clear();
        isResult = false;
    }

    //ui->runScan->setEnabled(false);

    size_t size{ static_cast<size_t>(listFiles.size()) };

    auto countThread{ countThreads() };

    size_t minThreads{ std::min(static_cast<size_t>((countThread < 2 ? 2 : countThread - 1)), size) };

    std::vector<std::thread> threads;
    threads.reserve(minThreads);

    vec.clear();
    vec.resize(size);

    size_t part{ std::max(size / minThreads, size_t(1)) };

    using iter = decltype(vec.begin());
    using type_paths = decltype(listFiles);

    for(size_t i{}, ie{ minThreads }; i != ie; ++i){

        size_t begin{ part * i };
        size_t end{ part * (i + 1) };

        if(i == ie - 1) end = size;

        threads.emplace_back(std::thread(&MainWindow::addToThread<iter, type_paths>
                                         , vec.begin()
                                         , vec.begin() + begin
                                         , vec.begin() + end
                                         , std::cref(listFiles)));
    }

    for(auto&& elem: threads){
        if(elem.joinable()) elem.join();
    }

    isSuccess = true;
    isResult = true;

    //ui->runScan->setEnabled(true);

}

uint MainWindow::countThreads() noexcept
{
    return  std::thread::hardware_concurrency();
}

void MainWindow::freezeWidgets(bool condition)
{
    ui->tableWidget->setEnabled(!condition);
    ui->tableWidget->setVisible(!condition);
    ui->runScan->setEnabled(!condition);
    ui->paths->setEnabled(!condition);
    ui->openDir->setEnabled(!condition);
    ui->auto_scan->setEnabled(!condition);
}

template< class TypeVecIter, class TypePaths>
void MainWindow::addToThread(TypeVecIter first, TypeVecIter begin, TypeVecIter end, const TypePaths& basePaths){

    if(end - begin <= 0) return;
    for(auto it{ begin }, ite{ end }; it != ite; ++it){
        auto distance{ std::distance(first, it) };
        *it = std::move(KHAS::Replay(basePaths[distance].absoluteFilePath().toStdString(), distance));
    }
}

void MainWindow::on_paths_activated(int index)
{
    using settings = KHAS::Settings;
    using fieldNames = KHAS::FieldNames;

    settings::setValue(settings::getFieldName(fieldNames::CurrentPathIndex), std::to_string(index));
    //settings.save();
    clearTable(ui->tableWidget);
}

void MainWindow::on_runScan_clicked()
{


   // std::string s{ "hello" };

    //KHAS::Zip p{ "1.zip" };

   // p.test_compression();

    using replay = KHAS::Replay;
    using settings = KHAS::Settings;
    using fildNames = KHAS::FieldNames;

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

    freezeWidgets(true);

    auto&& [vecReplays, isVecReplays] = createVectorWotReplaysThread(listFiles);

    if(!isVecReplays) {
        KHAS::Logs::pushAndFlash("auto&& [vecReplays, isVecReplays] = createVectorWotReplaysThread(listFiles);\
                                 if(!isVecReplays) == false", KHAS::AppType::Debug);
        toThreadStatusBar("Неизвестная Ошибка!", ui->statusBar);

        clearTable(ui->tableWidget);
        return;
    }

    replaysToTableThreads(*ui->tableWidget, vecReplays);


    auto&& [activeColumn, isActiveColumn] = settings::getValue<size_t>(settings::getFieldName(fildNames::ActiveColumn));
    auto&& [typeSortColumns, isTypeSortColumns] = settings::getValue<bool>(settings::getFieldName(fildNames::TypeSortColumns));

    Qt::SortOrder typeSort{ Qt::SortOrder::AscendingOrder };
    if(isTypeSortColumns && typeSortColumns) typeSort = Qt::SortOrder::DescendingOrder;
    // sort by index or zero
    if(isActiveColumn) ui->tableWidget->sortByColumn(activeColumn, typeSort);
    else ui->tableWidget->sortByColumn(0, typeSort);

    toThreadStatusBar("Обновлено!", ui->statusBar);

    freezeWidgets(false);





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
    using settings = KHAS::Settings;
    using fildNames = KHAS::FieldNames;

    auto&& [typeSortColumns, isTypeSortColumns] = settings::getValue<bool>(settings::getFieldName(fildNames::TypeSortColumns));

    Qt::SortOrder typeSort{ Qt::SortOrder::DescendingOrder };
    if(isTypeSortColumns && typeSortColumns) typeSort = Qt::SortOrder::AscendingOrder;

    settings::setValue("typeSortColumns", std::to_string(!typeSortColumns));

    auto&& [activeColumn, isActiveColumn] = settings::getValue<size_t>(settings::getFieldName(fildNames::ActiveColumn));
    if(!isActiveColumn) {

        KHAS::Logs::pushAndFlash("auto&& [activeColumn, isActiveColumn] = settings::getValue<size_t>(settings::getFieldName(fildNames::ActiveColumn));\
                                 if(!isActiveColumn) == false", KHAS::AppType::Debug);

        ui->tableWidget->sortByColumn(0, typeSort);
        return;
    }

    if(!settings::checkIsRange(0ull, settings::getCountHeaderList(), activeColumn)){

        KHAS::Logs::pushAndFlash("if(!settings::checkIsRange(0ull, settings::getCountHeaderList(), activeColumn)) == false", KHAS::AppType::Debug);
        ui->tableWidget->sortByColumn(0, typeSort);
        return;
    }

    ui->tableWidget->sortByColumn(index, typeSort);

    settings::setValue("activeColumn", std::to_string(index));

}



void MainWindow::on_auto_scan_stateChanged(int arg1)
{

#if 0
    //
    auto listFiles { scanDirectory(ui->paths->itemText(ui->paths->currentIndex()), QString("*.wotreplay")) };

    std::cout << "listfiles size: " << listFiles.size() << std::endl;

    for(int i{1}; i <= 2; ++i){
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

            //std::cout << "status fileopen: " << std::boolalpha << isCreate << " filename: " << to << std::endl;
            fin.close();
            fout.close();

        }
    }
#endif
}


void MainWindow::on_openDir_clicked()
{

    using settings = KHAS::Settings;
    using fieldNames = KHAS::FieldNames;

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

