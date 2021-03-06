#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./ui_mainwindow.h"
#include "concepts.h"
#include "settings.h"
#include "replay.h"
#include <sstream>
#include <thread>
#include <mutex>
#include <stdexcept>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_runScan_clicked();

    void on_paths_activated(int index);




    void on_checkBox_stateChanged(int arg1);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void headerClicked(int index);

    void on_auto_scan_stateChanged(int arg1);

    void on_openDir_clicked();

private:
    ///
    /// \brief toQString
    /// \param str
    /// \return
    ///
    template <typename Type>
    inline static std::pair< QString, bool > toQString(Type&& str);

    ///
    /// \brief variableInitialization
    ///
    inline void variableInitialization();

    ///
    /// \brief initPathsView
    /// \return
    ///
    inline bool initPathsView();

    ///
    /// \brief clearTable
    /// \param table
    ///
    void clearTable(QTableWidget* table);

    ///
    /// \brief scanDirectory
    /// \param pathDir
    /// \param filter
    /// \return
    ///
    template <typename TypeDirName, typename TypeFilter, typename = std::enable_if_t<std::is_same_v<std::decay_t<TypeFilter>, QString>> >
    inline QFileInfoList scanDirectory(TypeDirName&& pathDir, TypeFilter&& filter);

    ///
    /// \brief replaysToTableThreads
    /// \param table
    /// \param vec
    /// \return
    ///
    template <typename TypeWidget, typename= std::enable_if_t<std::is_same_v<TypeWidget, QTableWidget>>>
    bool replaysToTableThreads(TypeWidget& table, const std::vector<KHAS::Replay>& vec);

    ///
    /// \brief setHeaderInTable
    /// \param table
    /// \return
    ///
    template <typename Type = int>
    bool setHeaderInTable(QTableWidget& table);

    ///
    /// \brief setCurrentItemThreads
    /// \param table
    /// \param first
    /// \param begin
    /// \param end
    ///
    template <typename Iter, typename TypeWidget = QTableWidget>
    static void setCurrentItemThreads(TypeWidget& table, Iter first, Iter begin, Iter end);

    ///
    /// \brief toThreadStatusBar
    /// \param str
    /// \param label
    ///
    void toThreadStatusBar(QString&& str, QLabel* const label, int waitSec = 1);

    ///
    /// \brief showStatusBar
    /// \param str
    /// \param label
    ///
    static void showStatusBar(const QString& str, QLabel* const label, int waitSec);

    ///
    /// \brief createVectorWotReplaysThread
    /// \return
    ///
    std::pair<std::vector<KHAS::Replay>, bool> createVectorWotReplaysThread(const QList<QFileInfo>& listFiles);

    ///
    /// \brief createVectorWotReplays
    /// \param listFiles
    /// \param vec
    /// \param isSuccess
    /// \param isResult
    ///
    static void createVectorWotReplays(const QList<QFileInfo>& listFiles, std::vector<KHAS::Replay>& vec, bool& isSuccess, bool & isResult);

    ///
    /// \brief addToThread
    /// \param first
    /// \param begin
    /// \param end
    /// \param basePaths
    /// \param pb
    ///
    template< class TypeVecIter, class TypePaths>
    static void addToThread(TypeVecIter first, TypeVecIter begin, TypeVecIter end, const TypePaths& basePaths);

    ///
    /// \brief countThreads
    /// \return
    ///
    static uint countThreads() noexcept;

    ///
    /// \brief freezeWidgets
    /// \param condition
    ///
    void freezeWidgets(bool condition);

private:
    ///
    /// \brief ui
    ///
    Ui::MainWindow *ui;

};



//////////////////////////// implementation of class functions /////////////////////////////////////////////

///
/// \brief MainWindow::toQString
/// \param str
/// \return
///
template <typename Type>
std::pair< QString, bool > MainWindow
::toQString(Type&& str){
    std::stringstream ss;
    ss << std::forward<Type>(str);
    if(ss.fail()) {
        return { QString(), false };
    }
    return { QString::fromStdString(ss.str()), true };
}

///
/// \brief MainWindow::scanDirectory
/// \param pathDir
/// \param filter
/// \return
///
template <typename TypeDirName, typename TypeFilter, typename T3 >
QFileInfoList MainWindow
::scanDirectory(TypeDirName&& pathDir, TypeFilter&& filter){

    QDir dir{ QString(std::forward<TypeDirName>(pathDir)) };
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList filters(std::forward<TypeFilter>(filter));
    dir.setNameFilters(filters);

    dir.setSorting(QDir::Size | QDir::Reversed);
    return dir.entryInfoList();

}

///
/// \brief MainWindow::setCurrentItemThreads
/// \param table
/// \param first
/// \param begin
/// \param end
///
template <typename Iter, typename TypeWidget/*, typename std::enable_if_t<KHAS::IsMap<Type>::type>*/>
void MainWindow::setCurrentItemThreads(TypeWidget &table, Iter first, Iter begin, Iter end)
{
    static_assert (std::is_same_v<TypeWidget, QTableWidget>, "static void MainWindow::setCurrentItemThreads(TypeWidget &table, Iter begin, Iter end)\
    (TypeWidget == QTableWidget) == false");

    using namespace std::literals;
    if(end - begin <= 0) std::runtime_error("the end must be greater than the beginning");

    for(auto it{ begin }, ite{ end }; it < ite; ++it){

        auto distance{ std::distance(first, it) };

        auto isValidity{ (*begin).template getValue<bool>("validity"s) };


        for(int j{}, je{ table.columnCount() }; j < je; ++j){

            auto headerItem{ table.horizontalHeaderItem(j) };
            auto elemString{ headerItem->text().toStdString() };

            std::string val{"-"};

            if ((*it).template checkValue<bool>(elemString)){
                val = std::to_string((*it).template getValue<bool>(elemString));
            }
            else if ((*it).template checkValue<std::string>(elemString)){
                val = (*it).template getValue<std::string>(elemString);
            }
            else if ((*it).template checkValue<size_t>(elemString)){
                val = std::to_string((*it).template getValue<size_t>(elemString));
            }

            QTableWidgetItem* elem{ new QTableWidgetItem() };


            if(isValidity)  elem->setForeground(QColor(34, 34, 34));
            else            elem->setBackground(QColor(160, 0, 0));

            elem->setText(QString::fromStdString(std::move(val)));
            elem->setFont( {"Tahoma", 12, QFont::StyleNormal } );
            elem->setTextAlignment(Qt::AlignCenter);

            table.setItem(distance, j, elem);

        }
    }



}

///
/// \brief MainWindow::setHeaderInTable
/// \param table
/// \return
///
template <typename Type>
bool MainWindow::setHeaderInTable(QTableWidget &table)
{
    using settings = KHAS::Settings;

    for(size_t i{ 1 }, ie{ settings::getCountHeaderList() }; i <= ie; ++i){

        std::string headerName{ "header_" + std::to_string(i) };
        auto&& [value, isValue] = settings::getValue<std::string>(headerName, KHAS::SelectBase::Headers);

        if(isValue){
            QTableWidgetItem* elem{ new QTableWidgetItem ( QString::fromStdString(value) ) };
            elem->setFont( {"Tahoma", 14, QFont::StyleNormal } );
            elem->setForeground(Qt::darkGray);
            elem->setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);

            table.setHorizontalHeaderItem(i-1, elem );
        }
        else{
            table.setHorizontalHeaderItem(i-1, new QTableWidgetItem(QString(i, '-')));

            return false;
        }
    }
    return true;
}


template <typename TypeWidget, typename T2>
bool MainWindow::replaysToTableThreads(TypeWidget& table, const std::vector<KHAS::Replay> &vec)
{

    using settings = KHAS::Settings;


    static_assert (std::is_same_v<TypeWidget, QTableWidget>, "static bool MainWindow::replaysToTableThreads(TypeWidget& table, const std::vector<KHAS::Replay> &vec)\
    (TypeWidget == QTableWidget) == false");

    if(vec.size() == 0) return false;
    table.clear();

    table.setRowCount(static_cast<int>(vec.size()));
    table.setColumnCount(settings::getCountHeaderList());

    if(!setHeaderInTable(table)) return false;

    size_t size{ vec.size() };

    auto countThread{ countThreads() };

    size_t minThreads{ std::min(static_cast<size_t>((countThread < 2 ? 2 : countThread - 1)), size) };

    std::vector<std::thread> threads;
    threads.reserve(minThreads);

    size_t part{ std::max(size / minThreads, size_t(1)) };

    using iter =            decltype(vec.begin());
    using type_widget =     std::decay_t<TypeWidget>;

    for(size_t i{}, ie{ minThreads }; i != ie; ++i){

       size_t begin{ part * i };
       size_t end{ part * (i + 1) };

       if(i == ie - 1) end = size;

       threads.emplace_back(std::thread(&MainWindow::setCurrentItemThreads<iter, type_widget>
                                        , std::ref(table)
                                        , vec.begin()
                                        , vec.begin() + begin
                                        , vec.begin() + end));
    }

    for(auto&& elem: threads){
       if(elem.joinable()) elem.join();
    }


    ui->allCount->setText(QString::number(KHAS::Replay::getCount()));
    ui->allValidity->setText(QString::number(KHAS::Replay::getCountValidity()));
    return true;
}


#endif // MAINWINDOW_H
