#include "./ui_mainwindow.h"
#include <sstream>


namespace WSTR {
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class Functions : public Ui_MainWindow{
    public:
        Functions();

        template <typename Type>
        static QString toString(Type&& str);

        template <typename Type>
        void toStatusBar(Type&& in);

    private:
        Ui::MainWindow* ui{ nullptr };
    };





    template <typename Type>
    QString Functions
    ::toString(Type&& str){
        std::stringstream ss;
        ss << std::forward<Type>(str);
        return QString::fromStdString(ss.str());
    }



    template <typename Type>
    void  Functions
    ::toStatusBar(Type&& str){
        //ui->statusBar->setText("jj");
    }
}
