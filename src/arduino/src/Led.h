#ifndef Led_h
#define Led_h

#include "Arduino.h"
 
class Led{
  private:
    String name;
    byte pin;

  public:
    Led(const String name, byte pin){
      this->name = name;
      this->pin = pin;
      init();
    }

    void init(){
      pinMode(pin, OUTPUT);
    }

    void on(){
      //Serial.println("Led #" + String(name) +" ON");
      digitalWrite(pin, LOW);
    }

    void off(){
      //Serial.println("Led #" + String(name) + " OFF");
      digitalWrite(pin, HIGH);      
    }

    String getName(){
      return name;
    }

    void update(String name, String state){
      if(name != this->name) return void();

      if(state == "on"){
        on();
      }else
      if(state == "off"){
        off();
      }
      
    }
};

 #endif
