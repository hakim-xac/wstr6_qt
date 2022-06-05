#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./ui_mainwindow.h"
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
    ///
    void replaysToTable(QTableWidget& table, const std::vector<WSTR::Replay>& vec);

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


#endif // MAINWINDOW_H
