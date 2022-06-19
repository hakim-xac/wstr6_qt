#include "logs.h"

namespace WSTR {

Logs::~Logs()
{
    flush();
}


void Logs::flush()
{
    if(!base_.str().empty()){
        out_ << base_.str() << std::endl;
        base_.str(std::string());
    }
}










}
