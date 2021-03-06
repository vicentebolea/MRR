#include <iostream>
#include <vector>
#include <file_distributor/fileserver.hh>
#include <fstream>
#include <sstream>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <mapreduce/definitions.hh>
#include <common/settings.hh>
#include <exception>

using namespace std;

char master_address[BUF_SIZE];

bool master_is_set = false;

int port = -1;
int dhtport = -1;

fileserver afileserver;

int main (int argc, const char *argv[])
{
    // initialize data structures from setup.conf
    string token;
    Settings setted;
    setted.load_settings();

    try
    {
      port = setted.port();
      dhtport = setted.dhtport();
      strcpy (master_address, setted.master_addr().c_str());
      master_is_set = true;
    }
    catch (exception& e) 
    {
      cout << e.what() << endl;
    }

    // run the file server
    afileserver.run_server (dhtport, master_address);
    return 0;
}
