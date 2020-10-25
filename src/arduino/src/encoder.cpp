#include "Arduino.h"
#include "encoder.h"

const unsigned long debounceTime = 50;

Encoder::Encoder(const String name, byte pinDT, byte pinCLK, byte pinSW)
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
}

void Encoder::tick()
{
    // on mesure PIN
    byte read = digitalRead(pinDT);

    // controle du temps pour eviter des erreurs
    if (millis() - time > debounceTime)
    {
        Serial.print(name);
        Serial.write(static_cast<byte>(58));
        // Si CLK different de l'ancien état de DT alors
        if (digitalRead(pinCLK) != prev)
        {
            Serial.write(static_cast<byte>(117)); // u for up
        }
        else
        {
            Serial.write(static_cast<byte>(100)); // d for down
        }
        Serial.write(static_cast<byte>(3)); // 3 -> end of text le byte 3
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
            Serial.print(name);
            Serial.write(static_cast<byte>(58));
            Serial.write(static_cast<byte>(98)); // b for button
            Serial.write(static_cast<byte>(3));
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
