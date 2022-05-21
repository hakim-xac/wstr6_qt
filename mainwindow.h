#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./ui_mainwindow.h"
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

private:
    template <typename Type>
    static QString toString(Type&& str);
    template<typename Type>
    void toStatusBar(Type&& in);
private:
    Ui::MainWindow *ui;
};




template <typename Type>
    QString MainWindow
    ::toString(Type&& str){
        std::stringstream ss;
        ss << std::forward<Type>(str);
        return QString::fromStdString(ss.str());
    }


    template <typename Type>
        void MainWindow
        ::toStatusBar(Type&& str){
            ui->statusBar->setText(toString(std::forward<Type>(str)));
        }
#endif // MAINWINDOW_H
