#pragma once

#include <string>

typedef struct {
    char cmd;
} msg_t;

typedef struct {
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    float timestamp;
} imu_t;

static std::string HOST{"10.0.1.116"};
constexpr int PORT = 9999;
