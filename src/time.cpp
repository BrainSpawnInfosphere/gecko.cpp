/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#include "gecko/time.hpp"

namespace sc = std::chrono;

double time_now(){
    sc::microseconds ms = sc::duration_cast< sc::microseconds >(
    sc::system_clock::now().time_since_epoch());
    return (double)(ms.count())/1000000.0;
}
