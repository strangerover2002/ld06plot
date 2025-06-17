#include <chrono>
#include <iostream>
#include <optional>
#include <signal.h>
#include <thread>
#include <vector>

#include "LD06Kit/lidarkit.hpp"

static bool is_running = true;

using namespace std;

void sigint_handler(int)
{
    is_running = false;
}
LP_t lp[360 +1];

int task()
{
    // register signal handler, for smooth CTRL+C interrupt
    signal(SIGINT, sigint_handler);
    int i,j;

    string uri = DEFAULT_DEVICE_URI;

    try {
        LidarKit lk(uri);
        lk.start();
        this_thread::sleep_for(0.1s);
        while (is_running) {
            auto v = lk.get_points();
            for (auto& p : v){
            	j=(int)(p.angle);
            	i++;i %= 360;
            	if(i>360)continue;
#if 1
            	lp[j].angle= p.angle;
            	lp[j].distance= p.distance;
            	lp[j].confidence= p.confidence;
#else
            	lp[i].angle= ((float)i);
            	lp[i].distance= 300;
            	lp[i].confidence= 200;
#endif
            }
            //printf("i %d\n",v.size());
            this_thread::sleep_for(0.2s);
        }
        lk.stop();
	auto v = lk.get_points();
        //for (auto& p : v) cout << p << endl;
    } catch (exception& e) {
        cout << "Fatal error!" << endl;
    }

    return 0;
}

int com_main()
{
    thread t1(task);
    return 0;
}
