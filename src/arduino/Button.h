#ifndef Button_h
#define Button_h

#include "Arduino.h"
 
class Button{
  private:
    String name;
    Led* led;
    byte pin;
    byte lastReading;

  public:
    Button(const String name, byte pin){
      this->name = name;
      this->pin = pin;
      lastReading = LOW;
      init();
    }

    void init(){
       pinMode(pin, INPUT);
    }

    void setup(Led* led){
      this->led = led;
    }

    void loop(){
      byte reading = digitalRead(pin);

      if(reading == lastReading) return void();
      lastReading = reading;

      if(reading == HIGH){
        release();
      }else{
        press();
      }
    }

    void press(){
      Serial.println("Button #" + String(name)  +  " pressed");
      //Serial.println(&led->getName());
      this->led->on();
    }

    void release(){
      Serial.println("Button #" + String(name)  +  " released");
      this->led->off();
    }
    
};

 #endif
