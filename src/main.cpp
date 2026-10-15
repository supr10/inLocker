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
#define PIN_OUT_BUZZER 4

//display-related constants
#define OLED_W 128
#define OLED_H 64
#define OLED_RESET -1
#define OLED_ADDR 0x3c


class button{
    public:
        button() = default;
        button(int PIN){
            pin = PIN;
            pinMode(pin, INPUT_PULLUP);
        }
        bool check(){
            state = digitalRead(pin);
            if(state!=lastState){
                lastState = state;
                return state==LOW;
            }return false;
        }
    private:
        int pin, state, lastState;
};


bool running = false;

long secs;
unsigned long lastNow = 0;      //last time log (for counting time)
long lockInTime = 600;          //lock in time (10 min)
long breakTime = 200;
bool paused;


Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, OLED_RESET);

button accept;
button swtch;       //wanted to call it switch but its a keyword (obviously)

bool timer();
bool breakTimer();


void setup(){
    //setting up pins
    pinMode(PIN_DEBUG_LED, OUTPUT);
    pinMode(PIN_OUT_BUZZER, OUTPUT);

    accept = button(PIN_BUTTON_ACCEPT);
    swtch = button(PIN_BUTTON_SWITCH);

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

    //timer
    paused = false;
    secs = lockInTime;
}

void loop(){
    if(paused){
        if(breakTimer()==true){
            paused = false;
            secs = lockInTime;
        }
    }else{
        if(timer()==true){
            paused = true;
            secs = breakTime;
        }
    }
}

bool timer(){
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
    return accept.check()&&secs<0;
}

bool breakTimer(){
    unsigned long now = millis();
    if(now-lastNow>=1000){
        lastNow = now;
        display.clearDisplay();
        display.setCursor(0, 0);
        if(--secs>0){
            display.printf("break time!\n %d:%d", (int)secs/60, secs%60);
        }else{
            display.printf("the pause is over!");
            if(secs%2==0){
                tone(PIN_OUT_BUZZER, 440);
            }else{
                noTone(PIN_OUT_BUZZER);
            }
        }
        
        display.display();
    }
    return swtch.check();
}