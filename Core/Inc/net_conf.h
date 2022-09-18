#ifndef __NET_CONF_H
#define __NET_CONF_H

// ИЗМЕНИТЬ  МАКРОС В СООТВЕТСВИИ С ВАШЕМ WIFI

#define WIFI_TAR 1

#if WIFI_TAR == 0

#define AP_SSID     (const char*)"НАЗВАНИЕ ТОЧКИ ДОСТУПА(ТД)"
#define AP_PSWD     (const char*)"ПАРОЛЬ ТД"

#elif WIFI_TAR == 1

#define AP_SSID     (const char*)"НАЗВАНИЕ ТОЧКИ ДОСТУПА(ТД)"
#define AP_PSWD     (const char*)"ПАРОЛЬ ТД"

#endif

#define CONNECT_MODE 0

#if CONNECT_MODE == 0

#define IpServer       "АДРЕС MQTT СЕРВЕРА"
#define ServerPort     "ПОРТ MQTT СЕРВЕРА"

#elif CONNECT_MODE == 1

#define IpServer       "АДРЕС MQTT СЕРВЕРА"
#define ServerPort     "ПОРТ MQTT СЕРВЕРА"

#endif

#define MQTT_DEVICE_ID (uint8_t*)"ID УСТРОЙСТВА"
#define MQTT_SECRET    (uint8_t*)"СЕКРЕТНЫЙ КОД УСТРОЙСТВА"

#define MQTT_CLIENTID   "MQTT КЛИЕНТ ID"
#define MQTT_USERNAME   "MQTT ЛОГИН"
#define MQTT_PASSWORD   "MQTT ПАРОЛЬ"

#endif
