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
    ///
    ///
    ///
    static inline std::stringstream base_{};

    ///
    ///
    ///
    static inline std::ostream& out_{ std::cout };


    static inline AppType currentApplicationType_{ AppType::Debug };

public:
    Logs() = default;
    ~Logs();

    ///
    /// \brief push
    /// \param elem
    ///
    template <typename Type>
    static void push(Type&& elem, WSTR::AppType appType = WSTR::AppType::Release);

    ///
    /// \brief flush
    ///
    static void flush();

    ///
    /// \brief push
    /// \param elem
    ///
    template <typename Type>
    static void pushAndFlash(Type&& elem, WSTR::AppType appType = WSTR::AppType::Release);

};


//////////////////////////// implementation of class functions /////////////////////////////////////////////


template <typename Type>
void Logs
::push(Type&& elem, WSTR::AppType appType){
    QTime currentTime{ QTime::currentTime() };

        if(appType == WSTR::AppType::Release || currentApplicationType_ == WSTR::AppType::Debug){
            base_ << std::setw(10) << std::left << "Time: " << "| "
              << std::setw(15) << std::right << currentTime.toString("hh:mm:ss:zzz").toStdString() << "\n"
              << std::setw(10) << std::left << "Message: " << "| "
              << std::left << std::forward<Type>(elem) << "\n";
        }
}


template <typename Type>
void Logs
::pushAndFlash(Type&& elem, WSTR::AppType appType){

        push(std::forward<Type>(elem), appType);
        flush();
}


}

#endif // LOGS_H
