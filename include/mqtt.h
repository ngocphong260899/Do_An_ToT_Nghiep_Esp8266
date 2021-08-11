#include <Arduino.h>
#include <string.h>
#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include "PubSubClient.h"

#define mqtt_client_id  "sw_controller"
#define mqtt_server_ip  "mqtt.ngoinhaiot.com"
#define mqtt_user  "ngocphong260899"
#define mqtt_password  "ngocphong260899"
#define mqtt_topic_sub  "ngocphong260899/app"
#define mqtt_topic_pub  "ngocphong260899/device"
#define mqtt_port 1111

#define  key_json_pos "pos"
#define  key_status "status"
#define  key_json_sw_wifi "sw_wifi"



void mqtt_init();
void mqtt_loop();
void queueMsg(char* msg);