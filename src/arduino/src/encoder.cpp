#include "Arduino.h"
#include "encoder.h"

const unsigned long debounceTime = 100;

Encoder::Encoder(byte pinDT, byte pinCLK, byte pinSW, uint8_t shortcuts[3][2])
{
    this->pinDT = pinDT;
    this->pinCLK = pinCLK;
    this->pinSW = pinSW;
    this->prev = digitalRead(pinDT);
    this->time = millis(); // debounce
    pinMode(pinDT, INPUT);
    pinMode(pinCLK, INPUT);
    pinMode(pinSW, INPUT_PULLUP);

    // keyboard shortcuts
    for (int i=0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            this->shortcuts[i][j] = shortcuts[i][j];
        }
    };
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
            presskey(shortcuts[0]); // up
        }
        else
        {
            presskey(shortcuts[1]); // down; 
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
            presskey(shortcuts[2]);
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

void Encoder::presskey(uint8_t keys[2])
{
    Keyboard.press(keys[0]);
    delayMicroseconds(10000);
    Keyboard.press(keys[1]);
    delayMicroseconds(100000);
    Keyboard.releaseAll();
};