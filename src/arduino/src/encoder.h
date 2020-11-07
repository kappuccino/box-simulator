#ifndef Encoder_h
#define Encoder_h

#include <Keyboard.h>

class Encoder
{
private:
  String name;
  byte pinDT;
  byte pinCLK;
  byte pinSW;
  byte prev;
  int value;
  unsigned long time;

  // keyboard mod
  uint8_t keyboardKeyUp;
  uint8_t keyboardKeyDown;
  uint8_t keyboardKeyPress;

  void presskey(uint8_t);

public:
  Encoder(const String name, byte pinDT, byte pinCLK, byte pinSW, uint8_t, uint8_t, uint8_t);

  void tick();

  void btnClicked();

  byte getPinDT();

  byte getPinCLK();
};

#endif