#include "logs.h"

namespace WSTR {

Logs::Logs(std::ostream& out)
    : base_()
    , out_(std::cout)
    , currentApplicationType_(WSTR::AppType::Debug) {}


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
