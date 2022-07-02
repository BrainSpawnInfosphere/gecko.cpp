#include <marko/udpsocket.hpp>
#include <marko/utils.hpp>
#include <marko/time.hpp>
#include <serial.hpp>
#include <cstdio>
#include <thread>
// #include <atomic>
#include "common.hpp"

using namespace std;

EventSignal e;

imu_t cb(const msg_t& s){
    static int cnt = 0;
    printf(">> msg_t %d\n", cnt++);

    imu_t r {1,2,3,4,5,6,7,8,9,1};

    return r;
}

void reply(){
    TReply<msg_t, imu_t> r;
    r.bind(HOST,PORT);
    r.register_cb(cb);
    r.loop(e);

    printf(">> reply thread is don!!!\n");
}

void func(int i){
    e.clear();

    static int count = 0;
    int max = 2;
    if (count++ == max) {
        printf("** hard kill **");
        exit(1);
        marko::sleep(1);
    }
}


int main() {

    cout << time_now() << endl;
    printf("%lf\n", time_now());
    cout << asciitime_now() << endl;

    exit(0);

    Serial s;

    e.enable(func);
    e.set();

    // thread t(reply, std::ref(e));
    thread t(reply);

    while (true){
        cout<< "main loop" << endl;
        marko::sleep(2);

        if (! e.is_set()) break;
    }

    t.join();

    return 0;
}
