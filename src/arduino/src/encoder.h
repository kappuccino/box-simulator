#ifndef Encoder_h
#define Encoder_h

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

public:
  Encoder(const String name, byte pinDT, byte pinCLK, byte pinSW);

  void tick();

  void btnClicked();

  byte getPinDT();

  byte getPinCLK();
};

#endif
