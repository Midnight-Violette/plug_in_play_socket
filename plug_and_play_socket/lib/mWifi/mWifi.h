#pragma once

#include <Arduino.h>
#include <WiFi.h>

// ─────────────────────────────────────────────
//  WiFi Network Structure
//  Extend later (e.g. add savedPassword field)
//  when you connect to a backend / DB.
// ─────────────────────────────────────────────
struct WifiNetwork {
    char  ssid[33];           // Max SSID length is 32 chars + null terminator
    int32_t rssi;             // Signal strength in dBm
    uint8_t encryptionType;   // Maps to wifi_auth_mode_t values

    // Reserved for future use (web/DB password storage).
    // Always nullptr until you wire up that feature.
    const char* password = nullptr;
};

// ─────────────────────────────────────────────
//  mWifi API
// ─────────────────────────────────────────────

/**
 * Prints a human-readable encryption type name to Serial.
 * @param encType  The wifi_auth_mode_t integer returned by the WiFi scan.
 */
void PrintWifiEnc(int encType);

/**
 * Scans for networks and returns true if the given SSID is found.
 * @param ssid  The network name to look for (case-sensitive).
 * @return true if found, false otherwise.
 */
bool WifiScan(const char* ssid);

/**
 * Scans for all visible networks and returns them as a heap-allocated array.
 * Caller MUST free the result with FreeWifiScanResults() when done.
 *
 * @param count  Output parameter — set to the number of networks found.
 * @return       Pointer to a WifiNetwork array, or nullptr on failure.
 *
 * Example:
 *   int n = 0;
 *   WifiNetwork* nets = WifiScanAll(n);
 *   for (int i = 0; i < n; i++) { ... }
 *   FreeWifiScanResults(nets);
 */
WifiNetwork* WifiScanAll(int& count);

/**
 * Frees memory allocated by WifiScanAll().
 * Safe to call with nullptr.
 */
void FreeWifiScanResults(WifiNetwork* networks);

/**
 * Connects to a WiFi network and blocks until connected or timed out.
 *
 * @param ssid      Network name.
 * @param password  Network password (use "" for open networks).
 * @param timeoutMs Connection timeout in milliseconds (default: 10 000).
 * @return true if connected successfully, false on timeout.
 */
bool ConnectToWifi(const char* ssid, const char* password, uint32_t timeoutMs = 10000);