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

private:
    ///
    /// \brief toQString
    /// \param str
    /// \return
    ///
    template <typename Type>
    static std::pair< QString, bool > toQString(Type&& str);

    ///
    /// \brief toType
    /// \param str
    /// \return
    ///
    template <typename OutType, typename Type>
    static std::pair< OutType, bool > toType(Type&& str);


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

    QFileInfoList scanDirectory(const QString& pathDir);

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
/// \brief MainWindow::toType
/// \param str
/// \return
///
template <typename OutType, typename Type>
std::pair< OutType, bool > MainWindow
::toType(Type&& str){
    std::stringstream ss;
    ss << std::forward<Type>(str);
    if(ss.fail()) return { OutType(), false };

    OutType tmp{};
    ss >> tmp;
    if(ss.fail()) return { OutType(), false };

    return { tmp, true };
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


#endif // MAINWINDOW_H
