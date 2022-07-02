/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

#include <string>
#include <chrono>

double time_now();

inline std::string asciitime_now(){
    return std::to_string( time_now() );
}
