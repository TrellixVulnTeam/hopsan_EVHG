#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <sstream>
#include <chrono>
#include "RemoteHopsanClient.h"
#include "MessageUtilities.h"

#include <tclap/CmdLine.h>

using namespace std;
using namespace std::chrono;
typedef duration<double> fseconds;

bool gKeepRunning = true;
double gRefreshTime = 10;

void inputThreadFunc()
{
    while (gKeepRunning)
    {
        string input;
        cin >> input;

        // If update
        if (input.front() == 'u')
        {
            stringstream ss;
            ss << input.substr(1);
            ss >> gRefreshTime;
        }
        // Quit
        else if (input.front() == 'q')
        {
            gKeepRunning=false;
        }
    }
}

std::string nowDateTime()
{
    std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buff[64];
    std::strftime(buff, sizeof(buff), "%b %d %H:%M:%S", std::localtime(&now_time));
    return std::string(&buff[0]);
}

void printInfoFromAddressServer(vector<ServerMachineInfoT> &machines, vector<int> &nopenslots)
{
    cout << endl << endl;
    cout << nowDateTime() << endl;
    cout << std::setiosflags(std::ios::left) << std::setw(24) << "Adddress:" << std::setw(10) << "Slots:" << std::setw(10) << "evalTime:" << std::setw(10) << "Description:" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    for (size_t i=0; i<machines.size(); ++i)
    {

        string addr = machines[i].relayaddress;
        if (addr.empty())
        {
            addr = machines[i].address;
        }
        stringstream ss;
        ss << nopenslots[i] << "/" << machines[i].numslots;

        cout << std::setw(24) << addr << std::setw(10) << ss.str() << std::setw(10) << machines[i].evalTime << std::setw(10) << machines[i].description << endl;
    }
}


int main(int argc, char* argv[])
{
    string addressServerAddress;

    TCLAP::CmdLine cmd("ServerMonitor", ' ', "0.1");

    // Define a value argument and add it to the command line.
    TCLAP::SwitchArg argList("l", "list", "List the servers and exit", cmd);
    TCLAP::SwitchArg argFreeList("f", "free", "List all free servers and exit", cmd);
    TCLAP::ValueArg<double> argRefreshtime("r","refreshtime","The number of seconds between each refresh attempt",false,gRefreshTime,"double", cmd);
    TCLAP::ValueArg<std::string> argAddressServerIP("a", "addresserver", "IP:port to address server", true, "127.0.0.1:50000", "IP:Port", cmd);

    // Parse the argv array.
    cmd.parse( argc, argv );

    addressServerAddress = argAddressServerIP.getValue();
    if (argRefreshtime.isSet())
    {
        gRefreshTime = argRefreshtime.getValue();
    }

    thread inputThread;

    cout << endl;
    cout << "Starting server monitor, connecting to: " << addressServerAddress << endl;
    if (!argList.isSet() && !argFreeList.isSet())
    {
        cout << "Enter  q  to quit or  uN  (where N is the refresh time in seconds)" << endl;
        inputThread = thread(inputThreadFunc);
    }

    // Prepare our context
    try
    {
#ifdef _WIN32
        zmq::context_t context(1, 63);
#else
        zmq::context_t context(1);
#endif
        RemoteHopsanClient rhc(context);
        bool rc = rhc.connectToAddressServer(addressServerAddress);
        if (rc && rhc.addressServerConnected())
        {
            if (argList.isSet() || argFreeList.isSet())
            {
                std::vector<ServerMachineInfoT> machines;
                if (rhc.requestServerMachines(-1, 1e150, machines))
                {

                    for (ServerMachineInfoT &m : machines)
                    {
                        if(argFreeList.isSet())
                        {
                            std::string addr = m.address;
                            if (!m.relayaddress.empty())
                            {
                                addr = m.relayaddress;
                            }
                            if (!rhc.connectToServer(addr))
                            {
                                cout << "Failed!" << endl;
                                cout << rhc.getLastErrorMessage() << endl;
                            }
                            if (rhc.serverConnected())
                            {

                                ServerStatusT status;
                                bool gotStatus = rhc.requestServerStatus(status);
                                if (gotStatus && status.numFreeSlots == status.numTotalSlots)
                                {
                                    cout << m.address << endl;
                                }
                            }
                            rhc.disconnectServer();
                        }
                        else
                        {
                            cout << m.address << endl;
                        }
                    }
                }
                else
                {
                    cout << "Error: could not get servers" << endl;
                }
                gKeepRunning=false;
            }

            steady_clock::time_point lastRefreshTime;
            while (gKeepRunning)
            {
                fseconds dur = duration_cast<fseconds>(steady_clock::now() - lastRefreshTime);
                if (dur.count() > gRefreshTime)
                {
                    vector<int> nopenslots;
                    std::vector<ServerMachineInfoT> machines;
                    bool rsm_rc = rhc.requestServerMachines(-1, 1e150, machines);

                    nopenslots.resize(machines.size(), -1);

                    lastRefreshTime = chrono::steady_clock::now();
                    if (rsm_rc)
                    {
                        for (size_t i=0; i<machines.size(); ++i)
                        {
                            //std::string relayIdentityFull;
                            std::string addr = machines[i].address;
                            if (!machines[i].relayaddress.empty())
                            {
//                                std::string ip, relayport, actualport, relayBase;
//                                splitaddress(machines[i].address, ip, actualport, relayBase);
//                                splitaddress(machines[i].relayaddress, ip, relayport, relayBase);
//                                rhc.requestRelaySlot(relayBase, atoi(actualport.c_str()), relayIdentityFull );
//                                addr = ip+":"+relayport+":"+relayIdentityFull;
                                addr = machines[i].relayaddress;
                            }

                            if (!rhc.connectToServer(addr))
                            {
                                cout << rhc.getLastErrorMessage() << endl;
                            }
                            if (rhc.serverConnected())
                            {
                                ServerStatusT status;
                                bool gotStatus = rhc.requestServerStatus(status);
                                if (gotStatus)
                                {
                                    nopenslots[i] = status.numFreeSlots;
                                }
                                else
                                {
                                    cout << "Error: Could not get status from server: " << addr << endl;
                                }
                            }
                            else
                            {
                                cout << "Error: Could not connect to server: " << addr << endl;
                            }
                            rhc.disconnectServer();
                        }
                        printInfoFromAddressServer(machines, nopenslots);
                    }
                    else
                    {
                        cout << "Error: could not get servers" << endl;
                    }
                }
                else
                {
                    this_thread::sleep_for(fseconds(min(max(gRefreshTime-dur.count(),0.), 1.0)));
                }
            }
        }
        else
        {
            cout << "Error: Failed to connect to address server!" << endl;
        }
    }
    catch(zmq::error_t e)
    {
        cout << " Error: Preparing our context: " << e.what() << endl;
    }

    gKeepRunning = false;
    if (inputThread.joinable())
    {
        inputThread.join();
        cout << " Closed!" << endl;
    }
    return 0;
}

