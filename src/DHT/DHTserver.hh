//
// @brief Blocking server for distributed hash table 
//  
// :TODO: UDP Sockets may be more suitable ?
//
// Preprocessor {{{
#ifndef __DHTCLIENT_HH__
#define __DHTCLIENT_HH__

#include <string.h>
#include <utils.hh>  //! Where the hash function is
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdint.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>
#include <map>
#include <hash.hh>

using std::map;

#ifndef MRR_IP_LENGTH
#define MRR_IP_LENGTH 64 
#endif
// }}}
// DHTserver class definition {{{
class DHTserver {
 public:
  DHTserver (int port) {
   this->port = port;
   thread_continue = true;
  }
  ~DHTserver() { this->close (); };

  bool bind ();
  bool set_interface (const char*);
  bool listen ();
  bool close ();
  bool report (const char*, int);

 protected:
  bool server_request (int key);
  int server_receive ();
  static void* listening (void*);

 protected:
  int port;
  int server_fd;
  struct sockaddr_in server_addr; 

  pthread_t tserver;
  map<int, int> table;
  bool thread_continue; //! there is not race condition
};

#endif
// }}}