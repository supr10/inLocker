// Locker-in/main.cpp by Nolan Bazin (ggiver/supr10)
//@bazinnolan2@gmail.com


#include <Arduino.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


//used board pins
#define PIN_DEBUG_LED 2
#define PIN_BUTTON_ACCEPT 13
#define PIN_BUTTON_SWITCH 12

//display-related constants
#define OLED_W 128
#define OLED_H 64
#define OLED_RESET -1
#define OLED_ADDR 0x3c


Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, OLED_RESET);


void setup(){
    //setting up pins
    pinMode(PIN_DEBUG_LED, OUTPUT);
    pinMode(PIN_BUTTON_ACCEPT, INPUT_PULLUP);
    pinMode(PIN_BUTTON_SWITCH, INPUT_PULLUP);

    //setting up other things
    digitalWrite(PIN_DEBUG_LED, HIGH);      //to indicate that things are being set up
    delay(100);     //let the controller rest???
    
    //screen
    while(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) delay(100);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);   
    //adafruit splashscreen
    display.display();
    delay(300);
    digitalWrite(PIN_DEBUG_LED, LOW);   //to indicate that setting up phase is done
}

bool running = false;
bool overtime = false;

long secs = 20;
unsigned long lastNow = 0;      //last time log (for counting time)
long lockInTime = 600;          //lock in time (10 min)

void loop(){
    unsigned long now = millis();
    if(now-lastNow>=1000){
        lastNow = now;
        display.clearDisplay();
        display.setCursor(0, 0);
        if(--secs>0){
            display.printf("lock in!\n %d:%d", (int)secs/60, secs%60);
        }else{
            display.printf("overtime!\n %d:%d", -(int)secs/60, -secs%60);
        }
        
        display.display();
    }
}