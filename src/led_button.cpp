#include <Arduino.h>
#include "mqtt.h"
#define TIME_SLICE_TO_READ_BUTTON 10
#define OS_BTN_IS_PRESS_TIME_MAX 500
#define OS_BTN_IS_PRESS_TIME_MIN 10

#define NUMBER_ARR 3
#define BTN1 0
#define BTN2 1
#define BTN3 2

typedef enum
{
    LED1 = LED_BUILTIN,
    LED2 = D2,
    LED3 = D1
} led;

typedef enum
{
    BUTTON1 = D3,
    BUTTON2 = D5,
    BUTTON3 = D6
} button;

typedef enum
{
    CH1_ON = 1,
    CH1_OFF = 2,
    CH2_ON = 3,
    CH2_OFF = 4,
    CH3_ON = 5,
    CH3_OFF = 6
} mqtt_cmd_control;

uint8_t m_led[NUMBER_ARR] = {LED1, LED2, LED3};
uint8_t m_button[NUMBER_ARR] = {BUTTON1, BUTTON2, BUTTON3};
static uint16_t time_button_press[NUMBER_ARR] = {0, 0, 0};

void led_init()
{
    for (int i = 0; i < NUMBER_ARR; i++)
    {
        pinMode(m_led[i], OUTPUT);
    }
}
void led_staus_start()
{
    for (int i = 0; i < NUMBER_ARR; i++)
    {
        digitalWrite(m_led[i], HIGH);
    }
}

void button_init()
{
    for (int i = 0; i < NUMBER_ARR; i++)
    {
        pinMode(m_button[i], INPUT_PULLUP);
    }
}

void led_btn_init()
{
    led_init();
    button_init();
}

void toggle_led(uint8_t pin)
{
    digitalWrite(pin, !digitalRead(pin));
}

void gpio_on(uint8_t pin)
{
    digitalWrite(pin, HIGH);
}

void gpio_off(uint8_t pin)
{
    digitalWrite(pin, LOW);
}

void scan_button(uint8_t button_index)
{
    char msg[100];
    if (!digitalRead(m_button[button_index])) /* press */
    {
        time_button_press[button_index] += TIME_SLICE_TO_READ_BUTTON;
        if (time_button_press[button_index] > OS_BTN_IS_PRESS_TIME_MAX)
        {
            //blink_led_status();
        }
    }
    else
    { /* realse */
        if (time_button_press[button_index] >= OS_BTN_IS_PRESS_TIME_MIN && time_button_press[button_index] <= OS_BTN_IS_PRESS_TIME_MAX)
        {
            if (button_index < 3)
            {
                Serial.println("Button was press");
                toggle_led(m_led[button_index]);
                int stt = digitalRead(m_led[button_index]);
                sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, button_index+1, stt);
                queueMsg(msg);
            }
        }
        time_button_press[button_index] = 0;
    }
}

void button_handler()
{
    scan_button(BTN1);
    scan_button(BTN2);
    scan_button(BTN3);
}

void control_IO(int cmd)
{
    char msg[100];
    switch (cmd)
    {
    case CH1_ON:
    {
        gpio_on(LED1);
        int stt = digitalRead(LED1);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, 1, stt);
        queueMsg(msg);
        Serial.println("Debug_print: CH1 on");
    }
    break;
    case CH1_OFF:
    {
        gpio_off(LED_BUILTIN);
        int stt = digitalRead(LED1);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, 1, stt);
        queueMsg(msg);
        Serial.println("Debug_print: CH1 off");
    }
    break;

    case CH2_ON:
    {
        gpio_on(LED2);
        int stt = digitalRead(LED2);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, 2, stt);
        queueMsg(msg);
        Serial.println("Debug_print: CH2 on");
    }
    break;
    case CH2_OFF:
    {
        gpio_off(LED2);
        int stt = digitalRead(LED2);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, 2, stt);
        queueMsg(msg);
        Serial.println("Debug_print: CH2 off");
    }
    break;

    case CH3_ON:
    {
        gpio_on(LED3);
        int stt = digitalRead(LED2);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, 3, stt);
        queueMsg(msg);
        Serial.println("Debug_print: CH3 on");
    }
    break;
    case CH3_OFF:
    {
        gpio_off(LED3);
        int stt = digitalRead(LED3);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, 3, stt);
        queueMsg(msg);
        Serial.println("Debug_print: CH3 off");
    }
    break;
    default:
    {
        Serial.println("Debug_print: Not controller");
    }
    }
}


void get_State_IO()
{
    char msg[100];
    for(int i =0; i < 3; i++)
    {
        int stt = digitalRead(m_led[i]);
        sprintf(msg, "{\"sw_wifi\":%d,\"pos\":%d,\"status\":%d}", 1, i+1, stt);
         queueMsg(msg);
    }
   
}