#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./ui_mainwindow.h"
#include "concepts.h"
#include "settings.h"
#include "replay.h"
#include <sstream>


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

    void on_pushButton_clicked();

    void on_paths_activated(int index);

    void on_checkBox_clicked();

private:
    ///
    /// \brief toQString
    /// \param str
    /// \return
    ///
    template <typename Type>
    static std::pair< QString, bool > toQString(Type&& str);

    ///
    /// \brief toStatusBar
    /// \param in
    ///
    template<typename Type>
    void toStatusBar(Type&& in);

    ///
    /// \brief variableInitialization
    ///
    void variableInitialization();

    ///
    /// \brief initPathsView
    ///
    void initPathsView();

    ///
    /// \brief scanDirectory
    /// \param pathDir
    /// \param filter
    /// \return
    ///
    template <typename TypeDirName, typename TypeFilter = QString >
    QFileInfoList scanDirectory(TypeDirName&& pathDir, TypeFilter&& filter = QString("*.wotreplay"));

    ///
    /// \brief replaysToTable
    /// \param table
    /// \param vec
    /// \return
    ///
    bool replaysToTable(QTableWidget& table, const std::vector<WSTR::Replay>& vec);

    ///
    /// \brief setHeaderInTable
    /// \param table
    /// \return
    ///
    template <typename Type = int>
    const std::pair< std::map<std::string, size_t>, bool> setHeaderInTable(QTableWidget& table);

    ///
    /// \brief setCurrentItem
    /// \param table
    /// \param vec
    /// \param mp
    ///
    template <typename Type/*, typename std::enable_if_t<WSTR::IsMap<Type>::type>*/>
    void setCurrentItem(QTableWidget& table, const std::vector<WSTR::Replay> &vec, const Type& mp);

private:
    Ui::MainWindow *ui;

    ///
    /// \brief settings
    ///
    WSTR::Settings settings;

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
/// \brief MainWindow::toStatusBar
/// \param str
///
template <typename Type>
void MainWindow
::toStatusBar(Type&& str){
    settings.logs.push(str);

    std::stringstream ss{"Error transform!\n"};
    ss << "String: " << str;

    auto&& [new_str, isTransform] = toQString(std::forward<Type>(str));
    if(!isTransform){
        settings.logs.pushAndFlash(ss.str(), WSTR::AppType::Debug);
        return;
    }
    ui->statusBar->setText(std::move(new_str));

}

///
/// \brief MainWindow::scanDirectory
/// \param pathDir
/// \param filter
/// \return
///
template <typename TypeDirName, typename TypeFilter>
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
/// \brief MainWindow::setCurrentItem
/// \param table
/// \param vec
/// \param mp
///
template <typename Type/*, typename std::enable_if_t<WSTR::IsMap<Type>::type>*/>
void MainWindow::setCurrentItem(QTableWidget &table, const std::vector<WSTR::Replay> &vec, const Type& mp)
{

    for(int i{}, ie{ table.rowCount() }; i < ie; ++i){
        for(int j{}, je{ table.columnCount() }; j < je; ++j){
            auto headerItem{ table.horizontalHeaderItem(j) };
            auto elemString{ headerItem->text().toStdString() };

            std::string val{"-"};
            if (vec[i].checkValue<bool>(elemString)){
                val = std::to_string(vec[i].getValue<bool>(elemString));
            }
            else if (vec[i].checkValue<std::string>(elemString)){
                val = vec[i].getValue<std::string>(elemString);
            }
            else if (vec[i].checkValue<size_t>(elemString)){
                val = std::to_string(vec[i].getValue<size_t>(elemString));
            }
            QTableWidgetItem* elem{ new QTableWidgetItem(QString::fromStdString(val)) };
            table.setItem(i, j, elem);
        }
    }
}


template <typename Type>
const std::pair< std::map<std::string, size_t>, bool> MainWindow::setHeaderInTable(QTableWidget &table)
{
    std::map<std::string, size_t> mp;
    for(size_t i{ 1 }, ie{ settings.getCountHeaderList() }; i <= ie; ++i){
        std::string headerName{ "header_" + std::to_string(i) };
        auto&& [value, isValue] = settings.getValue<std::string>(headerName, WSTR::SelectBase::Headers);

        if(isValue){
            QTableWidgetItem* elem{ new QTableWidgetItem ( QString::fromStdString(value) ) };
            elem->setFont( {"Tahoma", 14, QFont::StyleNormal } );
            elem->setForeground(Qt::darkGray);
            elem->setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);

            table.setHorizontalHeaderItem(i-1, elem );

            mp.insert( {value, static_cast<size_t>(i) } );
        }
        else{
            table.setHorizontalHeaderItem(i-1, new QTableWidgetItem(QString(i, '-')));

            return { {}, false };
        }
    }
    return { mp, true };
}


#endif // MAINWINDOW_H
