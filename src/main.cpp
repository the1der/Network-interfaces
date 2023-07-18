#include <cstdio>
#include <iostream>
#include "network_interface.h"


int main()
{
    std::vector <network_interface_t> networks = getNetworkInterfaces();
    if (networks.empty()) return -1;

    for(int i=0; i<networks.size(); i++)
    {
        std::cout << "Interface: " << networks[i].name << std::endl;
        printf("Ip: %u.%u.%u.%u\n\n", networks[i].ip[0], networks[i].ip[1], networks[i].ip[2], networks[i].ip[3]);
    }
}