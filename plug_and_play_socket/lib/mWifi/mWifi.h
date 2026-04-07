// WiFiKernel.h
#ifndef WIFI_KERNEL_H
#define WIFI_KERNEL_H

#include <WiFi.h>
#include <WebServer.h>


class WiFiKernel {
private:
    WiFiMode_t currentMode;
    const char* apSSID;
    const char* apPassword;
    int maxConnections;

public:
    WiFiKernel();
    ~WiFiKernel();
    
    // Function 1: Setup WiFi mode
    bool setupMode(WiFiMode_t mode, const char* apSSID, const char* apPassword);
    
    // Function 2: Get ESP IP (placeholder)
    // Function 3: Scan networks (placeholder)
    // Function 4: Setup web server (placeholder)
    // Function 5: Connect to network (placeholder)
    // Function 6: Setup network config (placeholder)
};

#endif