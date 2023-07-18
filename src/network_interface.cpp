
#include "network_interface.h"


#ifdef WIN32

    std::vector<network_interface_t> getNetworkInterfaces()
    {
        std::vector<network_interface_t> results = {};
        network_interface_t network_interface = {};

        ULONG bufferSize = 0;
        PIP_ADAPTER_ADDRESSES adapterAddresses = nullptr;

        // Call GetAdaptersAddresses to get the required buffer size
        DWORD result = GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &bufferSize);
        if (result == ERROR_BUFFER_OVERFLOW) {
            adapterAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new char[bufferSize]);

            // Call GetAdaptersAddresses again to retrieve the adapter information
            result = GetAdaptersAddresses(AF_INET, 0, nullptr, adapterAddresses, &bufferSize);

            if (result == NO_ERROR) {
                PIP_ADAPTER_ADDRESSES adapter = adapterAddresses;
                while (adapter != nullptr ) 
                {
                    if( adapter->OperStatus == IfOperStatusUp && adapter->IfType != IF_TYPE_SOFTWARE_LOOPBACK && adapter->IfType != IF_TYPE_TUNNEL)
                    {
                        PIP_ADAPTER_UNICAST_ADDRESS unicastAddress = adapter->FirstUnicastAddress;
                        sockaddr_in* pSockAddr = reinterpret_cast<sockaddr_in*>(unicastAddress->Address.lpSockaddr);
                        memcpy(network_interface.ip, &(pSockAddr->sin_addr), 4);
                        network_interface.name = std::string(adapter->AdapterName);
                        results.push_back(network_interface);
                    }
                    adapter = adapter->Next;
                }
            }
            else {
                std::cout << "Failed to retrieve adapter information. Error code: " << result << std::endl;
            }

            delete[] reinterpret_cast<char*>(adapterAddresses);
        }
        else {
            std::cout << "Failed to retrieve required buffer size. Error code: " << result << std::endl;
        }

        return results;
    }

#else 
    std::vector <network_interface_t> getNetworkInterfaces()
    {
        struct ifaddrs* ifAddrList = nullptr;
        network_interface_t network_interface = {};
        std::vector <network_interface_t> results = {};
        if (getifaddrs(&ifAddrList) == -1) {
            std::cerr << "Failed to get network interfaces information." << std::endl;
            return {};
        }

        char ip[INET_ADDRSTRLEN];
        struct ifaddrs* ifa = nullptr;
        for (ifa = ifAddrList; ifa != nullptr; ifa = ifa->ifa_next) 
        {
            if (ifa->ifa_addr == nullptr) continue;

            if (ifa->ifa_addr->sa_family == AF_INET && ifa->ifa_flags & IFF_BROADCAST) 
            {
                struct sockaddr_in* sa4 = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
                struct sockaddr_in* mask = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_netmask);
                uint8_t b_addr[4];
                network_interface.name = std::string(ifa->ifa_name);
                memcpy(network_interface.ip , &(sa4->sin_addr.s_addr), 4);
                
                results.push_back(network_interface);
                // std::cout << "Broadcast: " << getBroadcastAddress(ifa->ifa_addr, ifa->ifa_netmask) << std::endl;
                network_interface = {};
            }
        }

        freeifaddrs(ifAddrList);

        return results;
    }

#endif