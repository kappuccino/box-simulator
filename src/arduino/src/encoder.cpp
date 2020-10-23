#include "Arduino.h"
#include "encoder.h"

const unsigned long debounceTime = 100;

Encoder::Encoder(const String name, byte pinDT, byte pinCLK)
{
    this->name = name;
    this->pinDT = pinDT;
    this->pinCLK = pinCLK;
    this->prev = digitalRead(pinDT);
    this->time = millis();
    pinMode(pinDT, INPUT);
    pinMode(pinCLK, INPUT);
}

void Encoder::tick()
{
    // on mesure PIN
    byte read = digitalRead(pinDT);

    // controle du temps pour eviter des erreurs
    if (abs(millis() - time) > debounceTime)
    {
        Serial.print(name);
        Serial.write((byte)58);
        // Si CLK different de l'ancien état de DT alors
        if (digitalRead(pinCLK) != prev)
        {
            Serial.write((byte)100); // d for down
        }
        else
        {
            Serial.write((byte)117); // u for up
        }
        Serial.write((byte)3); // 3 -> end of text le byte 3
        // memorisation du temps
        time = millis();
    }
    // memorisation de l'état
    this->prev = read;
}

byte Encoder::getPinDT()
{
    return pinDT;
}

byte Encoder::getPinCLK()
{
    return pinCLK;
}
