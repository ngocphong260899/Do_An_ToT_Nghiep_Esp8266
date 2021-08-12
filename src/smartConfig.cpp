#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <time.h>
#include "led_button.h"
#include "mqtt.h"
#define LED1 LED_BUILTIN
#define BUTTON1 D3

#define LED_ON() digitalWrite(LED1, HIGH)
#define LED_OFF() digitalWrite(LED1, LOW)
#define LED_TOGGLE() digitalWrite(LED1, digitalRead(LED1) ^ 0x01)

Ticker ticker;

bool longPress()
{
    static int lastPress = 0;
    if (millis() - lastPress > 3000 && digitalRead(BUTTON1) == 0)
    {
        return true;
    }
    else if (digitalRead(BUTTON1) == 1)
    {
        lastPress = millis();
    }
    return false;
}

void tick()
{
    //toggle state
    int state = digitalRead(LED1); // get the current state of GPIO1 pin
    digitalWrite(LED1, !state);    // set pin to the opposite state
}

bool in_smartconfig = false;
void enter_smartconfig()
{
    if (in_smartconfig == false)
    {
        in_smartconfig = true;
        ticker.attach(0.5, tick);
        WiFi.beginSmartConfig();
    }
}

void exit_smart()
{
    ticker.detach();
    LED_ON();
    in_smartconfig = false;
}

void smart_config_init()
{
    ticker.attach(1, tick);
}

void smart_config_loop()
{
    if (longPress())
    {
        enter_smartconfig();
        Serial.println("Enter smartconfig");
    }
    if (WiFi.status() == WL_CONNECTED && in_smartconfig && WiFi.smartConfigDone())
    {
        exit_smart();
        Serial.println("Connected, Exit smartconfig");
    } 
}

void get_Wifi()
{
    char msg[100];
    String ssid = WiFi.SSID();
    char streng = WiFi.RSSI();
    sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d,\"ssid\":\"%s\",\"streng\":\"%c\"}", 2, 0, 0,ssid,streng);
    
    queueMsg(msg);

}