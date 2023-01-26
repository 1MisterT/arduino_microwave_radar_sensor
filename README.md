# Arduino Microwave Radar Sensor 
Arduino Microwave Radar Sensor with Home Assistant Upload via MQTT


## Features:
- radar sensor with a range up to 7 meters (or more, idk)
- MQTT upload of the sensor data if it changes
- 10Hz polling rate, variable with delay function
- HA discovery message to quickly setup the sensor

## How to use?
- download repo
- extract .zip
- open in arduino 
- change wifi and mqtt credentials
```c
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqttServer = MQTT_SERVER;
const int   mqttPort = MQTT_PORT;
const char* mqttUser = MQTT_USER;
const char* mqttPassword = MQTT_PASSWORD;
```

- maybe change the homeassistant state and discovery topics:
```c
String discovery_topic = "homeassistant/binary_sensor/radar_" + String(unique_id) + "/config";
String state_topic = "tom/sensor/radar/" + String(unique_id);
```

and finally upload the code :)

## Wiring
- i used the **XYC-WB-DC** microwave radar sensor [click here](https://www.e-gizmo.net/oc/kits%20documents/XYC-WB-DC%20Microwave%20Radar/xyc-wb-dc.pdf) for the datasheet

![Wiring](/esp_radar.png)
