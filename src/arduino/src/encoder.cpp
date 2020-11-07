#include "Arduino.h"
#include "encoder.h"

const unsigned long debounceTime = 50;

Encoder::Encoder(const String name, byte pinDT, byte pinCLK, byte pinSW, uint8_t kup, uint8_t kdown, uint8_t kpressed)
{
    this->name = name;
    this->pinDT = pinDT;
    this->pinCLK = pinCLK;
    this->pinSW = pinSW;
    this->prev = digitalRead(pinDT);
    this->time = millis(); // debounce
    pinMode(pinDT, INPUT);
    pinMode(pinCLK, INPUT);
    pinMode(pinSW, INPUT_PULLUP);

    // keyboard
    this->keyboardKeyUp = kup;
    this->keyboardKeyDown = kdown;
    this->keyboardKeyPress = kpressed;
}

void Encoder::tick()
{
    // on mesure PIN
    byte read = digitalRead(pinDT);  

    // controle du temps pour eviter des erreurs
    if (millis() - time > debounceTime)
    {
        // Si CLK different de l'ancien état de DT alors
        if (digitalRead(pinCLK) != prev)
        {
            presskey(keyboardKeyUp); // up
        }
        else
        {
            presskey(keyboardKeyDown); // down; 
        }               
        // memorisation du temps
        time = millis();
    }
    // memorisation de l'état
    this->prev = read;
}

void Encoder::btnClicked()
{
    if (digitalRead(pinSW) == LOW)
    {
        if (millis() - time > debounceTime)
        {
            presskey(keyboardKeyPress);
        }
        time = millis();
    }
}

byte Encoder::getPinDT()
{
    return pinDT;
}

byte Encoder::getPinCLK()
{
    return pinCLK;
}

// keyboard mod

void Encoder::presskey(uint8_t k)
{
    Keyboard.press(k);
    delayMicroseconds(50000);
    Keyboard.release(k);
};