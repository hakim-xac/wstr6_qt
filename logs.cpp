#include "logs.h"

namespace KHAS {

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
