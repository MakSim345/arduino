#ifndef SECRETS_H
#define SECRETS_H

// Template: copy this file to "secrets.h" and put your real networks there.
// secrets.h is listed in .gitignore and never goes to the git repo.

struct WifiCredential {
  const char* ssid;
  const char* password;
};

// 2.4 GHz networks only. The strongest one in range is used.
const WifiCredential WIFI_NETWORKS[] = {
  {"home_ssid",  "home_password"},
  {"phone_ssid", "phone_password"},
};

const int WIFI_NETWORKS_COUNT = sizeof(WIFI_NETWORKS) / sizeof(WIFI_NETWORKS[0]);

#endif
