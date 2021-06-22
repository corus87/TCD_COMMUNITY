#include <iostream>
#include <yaml-cpp/yaml.h>
#include <signal.h>
#include "hardwarecontrol.h"
#include <vector>
using namespace std;

HardwareControl m_hardwareControl;

void signal_callback_handler(int signum) {
   cout << "Caught signal " << signum << endl;
   m_hardwareControl.Stop();
   // Terminate program
   //exit(signum);
}

int main(int argc, char **argv)
{

    string battery,power;
    bool initialize = false;
    //catch signal
    signal(SIGINT, signal_callback_handler);
    signal(SIGABRT, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);
    signal(SIGQUIT, signal_callback_handler);
    signal(SIGKILL, signal_callback_handler);

    vector <int> exclude;

    vector<string> args(argv + 1, argv + argc);
    if (argc > 1) {
        for (auto i = args.begin(); i != args.end(); ++i) {
            if (*i == "-h" || *i == "--help") {
                cout << "usage: TCD_COMMUNITY -p power_profile.yaml -b battery_profile.yaml\n" << endl;
                cout << "-h, --help            : Print this help message and exit" << endl;
                cout << "-p, --power_profile   : Path to the power profile" << endl;
                cout << "-b, --battery_profile : Path to the battery profile" << endl;
                return 0;

            } else if (*i == "-p" || *i == "--power_profile") {
                power = *++i;

            } else if (*i == "-b" || *i == "--battery_profile") {
                battery = *++i;
            }
        }

        if (power.empty()) {
            cout << "No power profile set!" << endl;
            return 0;
        } else if ( access( power.c_str(), F_OK ) != 0 ) {
            cout << "Could not find power profile: " << power << endl;
            return 0;
            }

        if (battery.empty()) {
            cout << "No battery profile set!" << endl;
            return 0;
        } else if ( access( battery.c_str(), F_OK ) != 0 ) {
                cout << "Could not find battery profile: " << battery << endl;
                return 0;
            }

    }

    else {
        cout << "A Power and Battery profile is required!" << endl;
        cout << "Check -h or --help for usage." << endl;
        return 0;
    }

    initialize=m_hardwareControl.Initialize(power,battery);
    if (initialize)
    {
        m_hardwareControl.StartPowerMonitor();
        m_hardwareControl.Run();
    }
    else
        cout<<"ERROR Service could not initialize, check settings files!!!"<<endl;
    return 0;
}
