#include <marko/udpsocket.hpp>
#include <string>
#include <iostream>
#include "common.hpp"

using namespace std;

int main(){

    sockaddr_t addr = make(HOST,PORT);

    TRequest<msg_t, imu_t> r;
    msg_t msg{'g'};

    for (int i=0; i < 5; i++){
        imu_t resp = r.request(msg, addr);
        cout << ">> " << resp.mx << endl;
    }

    return 0;
}
