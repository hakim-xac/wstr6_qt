#include "logs.h"

namespace WSTR {

Logs::Logs(std::ostream& out)
    : base_()
    , out_(std::cout) {}


Logs::~Logs()
{
    flush();
}


void Logs::flush()
{
    out_ << base_.str() << std::endl;
}








}
