#ifndef Encoder_h
#define Encoder_h

#include <Keyboard.h>

class Encoder
{
private:
  byte pinDT;
  byte pinCLK;
  byte pinSW;
  byte prev;
  unsigned long time;

  // keyboard mod
  uint8_t shortcuts[3][2];

  void presskey(uint8_t *keys);

public:
  Encoder( byte pinDT, byte pinCLK, byte pinSW, uint8_t shortcuts[3][2]);

  void tick();

  void btnClicked();

  byte getPinDT();

  byte getPinCLK();
};

#endif