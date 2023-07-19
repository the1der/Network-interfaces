#ifndef NETWORK_INTERFACE__H
#define NETWORK_INTERFACE__H

#ifdef _WIN32
    #define _WIN32_WINNT 0x0502  // Replace with the desired value
    #include <iostream>
    #include <string>
    #include <windows.h>
    #include <iphlpapi.h>
    #include <iptypes.h>
    #include <vector>

    #pragma comment(lib, "iphlpapi.lib")
#else
    #include <iostream>
    #include <cstring>
    #include <sys/types.h>
    #include <ifaddrs.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <net/if.h>
    #include <vector>
#endif

struct network_interface_t
{
    std::string name;
    uint8_t ip[4];
};


/**
 * @brief Get the Network Interfaces object
 * 
 * @return std::vector <network_interface_t> 
 */
std::vector <network_interface_t> getNetworkInterfaces();

#endif