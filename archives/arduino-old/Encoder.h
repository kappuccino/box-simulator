#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"
 
class Encoder {
  private:
    String name;
    byte pinDT;
    byte pinCLK;
    byte prev;
    int value;
    unsigned long time;
    
  public:
    Encoder(const String name, byte pinDT, byte pinCLK){
      this->name = name;
      this->pinDT = pinDT;
      this->pinCLK = pinCLK;
      init();
    }

    void init(){
      pinMode(pinDT, INPUT);
      pinMode(pinCLK, INPUT);

      prev = digitalRead(pinDT);
      time = millis();
    }
    
    void changement(){
      
      // on mesure PIN
      byte read = digitalRead(pinDT);
      bool left = false;
             
      // controle du temps pour eviter des erreurs 
      if( abs(millis() - time) > 50 ){
    
        // Si CLK different de l'ancien état de DT alors
        if(digitalRead(pinCLK) != prev){
          left = true;
          value--;
        }else{
          value++;
          left = false;
        }
    
        // memorisation du temps
        time = millis();
    
        //Serial.println("ROTARY #"+ String(name) +" value: " + String(value));
        Serial.print(name);
        Serial.write((byte)58);
        if(left){    
          Serial.write((byte)100); // d fo down
        }else{
          Serial.write((byte)117); // u for up
        }
        Serial.write((byte)3); // 3 -> end of text le byte 3
      } 
    
      // memorisation de l'état
      prev = read;
    }

    byte getPinDT(){
      return pinDT;
    }
};



 #endif
