#include "mWifi.h"
#include <WiFi.h>

// ─────────────────────────────────────────────
//  PrintWifiEnc
// ─────────────────────────────────────────────
void PrintWifiEnc(int encType) {
    Serial.print("Encryption: ");
    switch (encType) {
        case WIFI_AUTH_OPEN:            Serial.println("Open (none)");       break;
        case WIFI_AUTH_WEP:             Serial.println("WEP");               break;
        case WIFI_AUTH_WPA_PSK:         Serial.println("WPA-PSK");           break;
        case WIFI_AUTH_WPA2_PSK:        Serial.println("WPA2-PSK");          break;
        case WIFI_AUTH_WPA_WPA2_PSK:    Serial.println("WPA/WPA2-PSK");      break;
        case WIFI_AUTH_WPA2_ENTERPRISE: Serial.println("WPA2-Enterprise");   break;
        case WIFI_AUTH_WPA3_PSK:        Serial.println("WPA3-PSK");          break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.println("WPA2/WPA3-PSK");     break;
        default:                        Serial.println("Unknown");           break;
    }
}

// ─────────────────────────────────────────────
//  WifiScan  —  find a specific SSID
// ─────────────────────────────────────────────
bool WifiScan(const char* ssid) {
    Serial.printf("[WifiScan] Looking for SSID: %s\n", ssid);

    int found = WiFi.scanNetworks();

    if (found == WIFI_SCAN_FAILED || found < 0) {
        Serial.println("[WifiScan] Scan failed.");
        WiFi.scanDelete();
        return false;
    } 

    if (found == 0) {
        Serial.println("[WifiScan] No networks found.");
        WiFi.scanDelete();
        return false;
    }

    bool match = false;
    for (int i = 0; i < found; i++) {
        if (strcmp(WiFi.SSID(i).c_str(), ssid) == 0) {
            Serial.printf("[WifiScan] Found! RSSI: %d dBm  ", WiFi.RSSI(i));
            PrintWifiEnc(WiFi.encryptionType(i));
            match = true;
            break;
        }
    }

    if (!match) {
        Serial.printf("[WifiScan] SSID \"%s\" not found in scan results.\n", ssid);
    }

    WiFi.scanDelete();
    return match;
}

// ─────────────────────────────────────────────
//  WifiScanAll  —  return all visible networks
// ─────────────────────────────────────────────
WifiNetwork* WifiScanAll(int& count) {
    count = 0;

    int found = WiFi.scanNetworks();

    if (found == WIFI_SCAN_FAILED || found <= 0) {
        Serial.println("[WifiScanAll] Scan failed or no networks found.");
        WiFi.scanDelete();
        return nullptr;
    }

    WifiNetwork* networks = new WifiNetwork[found];

    for (int i = 0; i < found; i++) {
        // Copy SSID safely — strlcpy ensures null termination
        strncpy(networks[i].ssid, WiFi.SSID(i).c_str(), sizeof(networks[i].ssid) - 1);
        networks[i].ssid[sizeof(networks[i].ssid) - 1] = '\0';

        networks[i].rssi           = WiFi.RSSI(i);
        networks[i].encryptionType = WiFi.encryptionType(i);
        networks[i].password       = nullptr;  // Not stored until backend is wired up
    }

    WiFi.scanDelete();
    count = found;
    return networks;
}

// ─────────────────────────────────────────────
//  FreeWifiScanResults
// ─────────────────────────────────────────────
void FreeWifiScanResults(WifiNetwork* networks) {
    if (networks != nullptr) {
        delete[] networks;
    }
}

// ─────────────────────────────────────────────
//  ConnectToWifi
// ─────────────────────────────────────────────
bool ConnectToWifi(const char* ssid, const char* password, uint32_t timeoutMs) {
    Serial.printf("[ConnectToWifi] Connecting to \"%s\"...\n", ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start >= timeoutMs) {
            Serial.println("\n[ConnectToWifi] Timed out.");
            WiFi.disconnect(true);
            return false;
        }
        delay(250);
        Serial.print(".");
    }

    Serial.printf("\n[ConnectToWifi] Connected! IP: %s\n",
                  WiFi.localIP().toString().c_str());
    return true;
}