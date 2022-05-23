#ifndef LOGS_H
#define LOGS_H

#include "enums.h"
#include <QTime>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

namespace WSTR {


class Logs
{
    std::stringstream base_{};
    std::ostream& out_{ std::cout };
public:
    Logs() = default;
    Logs(std::ostream& out);
    ~Logs();

    template <typename Type>
    void push(Type&& elem);
    void flush();


};



template <typename Type>
void Logs
::push(Type&& elem){
    QTime currentTime{ QTime::currentTime() };

    base_ << std::setw(10) << std::left << "Time: " << "| "
          << std::setw(15) << std::right << currentTime.toString("hh:mm:ss:zzz").toStdString() << "\n"
          << std::setw(10) << std::left << "Message: " << "| "
          << std::left << std::forward<Type>(elem) << "\n\n";
}


}

#endif // LOGS_H
