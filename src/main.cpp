#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>
#include <string.h>

/*
*define info mqtt
*/
 const char *ssid = "VNPT TUAN";
 const char *password = "";
const char *mqtt_client_id = "sw_controller";

const char *mqtt_server_ip = "mqtt.ngoinhaiot.com";
const char *mqtt_user = "ngocphong260899";
const char *mqtt_password = "ngocphong260899";
const char *mqtt_topic_sub = "ngocphong260899/app";
const char *mqtt_topic_pub = "ngocphong260899/device";
int mqtt_port = 1111;
int led = LED_BUILTIN;
int button = D3;
int state, lastState = 0;
#define json_pos "pos"
#define json_status "status"
#define json_sw_wifi "sw_wifi"
WiFiClient espClient;
PubSubClient client(espClient);
void smart_Config();
void control(int vitri, int status);
void callback(char *p_toppic, uint8_t *p_data, unsigned int length)
{
  
  Serial.println((char*)p_data);

  StaticJsonBuffer<1024> JSONBuffer;
  JsonObject &data = JSONBuffer.parseObject((char *)p_data);
  if (!data.success())
  {
    Serial.print("Phong_hg56 : Faild");
    return;
  }
int pos = data[json_pos];
int status = data[json_status];
int sw_wifi = data[json_sw_wifi];

Serial.println(pos);
Serial.println(status);
  
switch(sw_wifi){
  case 1:
  {
    control(pos,status);
  }
  break;

  case 2:
  {
    if(digitalRead(led) == 1)
    {
      String relay1_on = "{\"pos\":1,\"status\":0}";
      client.publish("ngocphong260899/device",(char*)relay1_on.c_str());
    }
    else if(digitalRead(led) ==0)
    {
      String relay1_on = "{\"pos\":1,\"status\":1}";
      client.publish("ngocphong260899/device",(char*)relay1_on.c_str());
    }
  }
  break;
}
}



void control(int vitri, int status)
{
  switch(vitri){
  case 1:
  {
    if(status == 1){
      digitalWrite(led,HIGH);
      String relay1_on = "{\"pos\":1,\"status\":0}";
      client.publish("ngocphong260899/device",(char*)relay1_on.c_str());
    }
    else if(status ==0)
    {
      digitalWrite(led,LOW);
      String relay1_on = "{\"pos\":1,\"status\":1}";
      client.publish("ngocphong260899/device",(char*)relay1_on.c_str());
    }
  }
  break;
}
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Dang ket noi MQTT...");
    // Connect MQTT
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_password))
    {
      Serial.println("Da ket noi xong MQTT"); // Ket noi xong , hien thi
      client.subscribe(mqtt_topic_sub);
    }
    else
    {
      Serial.print("No connect: ");
      Serial.print(client.state());
      Serial.println("Doi 5 giay");
      delay(5000);
    }
  }
}
void btn_control();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  digitalWrite(led,HIGH);
  WiFi.begin(ssid, password);
  Serial.println();
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10);
    Serial.print("..........");
  }
  Serial.print("Connect succuess");
 
  Serial.println("Setup done");
  client.setServer(mqtt_server_ip, mqtt_port);
  client.setCallback(callback);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  btn_control();
}
// button handle

void btn_control()
{  
  state = digitalRead(button);
  if(state != lastState && !state)
  {
    digitalWrite(led, !digitalRead(led));
    if(digitalRead(led) == 1)
    {
      String relay1_on = "{\"pos\":1,\"status\":0}";
      client.publish("ngocphong260899/device",(char*)relay1_on.c_str());
    }
    else if(digitalRead(led) ==0)
    {
      String relay1_on = "{\"pos\":1,\"status\":1}";
      client.publish("ngocphong260899/device",(char*)relay1_on.c_str());
    }
  }
  lastState = state;
}

