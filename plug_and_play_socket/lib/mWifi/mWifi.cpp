//WiFiKernel.cpp
#include "mWifi.h"

WiFiKernel::WiFiKernel() 
    : currentMode(WIFI_MODE_STA), apSSID(nullptr), apPassword(nullptr), maxConnections(4) {
}

WiFiKernel::~WiFiKernel() {
}

bool WiFiKernel::setupMode(WiFiMode_t mode, const char* apSSID, const char* apPassword) {
    this->currentMode = mode;
    this->apSSID = apSSID;
    this->apPassword = apPassword;
    this->maxConnections = maxConnections;
    
    // Disable WiFi first
    WiFi.mode(WIFI_OFF);
    delay(100);
    
    switch (mode) {
        case WIFI_MODE_AP:
            Serial.println("[WiFiKernel] Setting up AP mode...");
            if (!WiFi.softAP(apSSID, apPassword)) {
                Serial.println("[WiFiKernel] Failed to setup AP mode");
                return false;
            }
            Serial.println("[WiFiKernel] AP mode started successfully");
            return true;
            
        case WIFI_MODE_STA:
            Serial.println("[WiFiKernel] Setting up STA mode...");
            WiFi.mode(WIFI_STA);
            Serial.println("[WiFiKernel] STA mode set successfully");
            return true;
            
        case WIFI_MODE_APSTA:
            Serial.println("[WiFiKernel] Setting up AP+STA mode...");
            if (!WiFi.softAP(apSSID, apPassword)) {
                Serial.println("[WiFiKernel] Failed to setup AP mode");
                return false;
            }
            WiFi.mode(WIFI_AP_STA);
            Serial.println("[WiFiKernel] AP+STA mode set successfully");
            return true;
            
        default:
            Serial.println("[WiFiKernel] Invalid mode");
            return false;
    }
}