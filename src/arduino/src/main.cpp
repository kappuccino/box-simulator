#include <Arduino.h>
#include "Led.h"
#include "Button.h"
#include "encoder.h"
#include <Keyboard.h>

void tickE1();
void tickE2();
void tickE3();
void tickE4();
void tickE5();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des PIN physique //////////////////////////////////////////////////////////////////////////////////
//

// Encoder 1 (E1)
const int ROT_E1_CLK{22};
const int ROT_E1_DT{23};
const int ROT_E1_SW{24};

// Encoder 2 (E2)
const int ROT_E2_CLK{25};
const int ROT_E2_DT{26};
const int ROT_E2_SW{27};

// Encoder 3 (E3)
const int ROT_E3_CLK{28};
const int ROT_E3_DT{29};
const int ROT_E3_SW{30};

// Encoder 4 (E4)
const int ROT_E4_CLK{31};
const int ROT_E4_DT{32};
const int ROT_E4_SW{33};

// Encoder 5 (E5)
const int ROT_E5_CLK{34};
const int ROT_E5_DT{35};
const int ROT_E5_SW{36};

/// https://roboticsbackend.com/arduino-object-oriented-programming-oop/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Déclaration de nos instances /////////////////////////////////////////////////////////////////////////////////
//
// * Le premier argument est le nom humain que l'on donne à nos instances pour les identifier simplement sur les
//   sortie du port série. C'est plus simple de gérer B1 pour le bouton 1, plutot que sur son numéro de PIN
//

unsigned long lastButtonPress = 0;

uint8_t shortcuts[5][3][2]= {
  {
    {178,101},{178,114},{178,116}    
  },{
    {178,121},{178,117},{178,105}    
  },{
    {178,111},{178,112},{178,113}    
  },{
    {178,115},{178,100},{178,102}    
  },{
    {178,225},{178,231},{178,227}    
  },
};


// encodeurs rotatifs
Encoder e1(ROT_E1_DT, ROT_E1_CLK, ROT_E1_SW, shortcuts[0]);
Encoder e2(ROT_E2_DT, ROT_E2_CLK, ROT_E2_SW, shortcuts[1]);
Encoder e3(ROT_E3_DT, ROT_E3_CLK, ROT_E3_SW, shortcuts[2]);
Encoder e4(ROT_E4_DT, ROT_E4_CLK, ROT_E4_SW, shortcuts[3]);
Encoder e5(ROT_E5_DT, ROT_E5_CLK, ROT_E5_SW, shortcuts[4]);

void setup()
{
  Keyboard.begin();
  delay(2000);
  attachInterrupt(digitalPinToInterrupt(e1.getPinDT()), tickE1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(e2.getPinDT()), tickE2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(e3.getPinDT()), tickE3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(e4.getPinDT()), tickE4, CHANGE);
  attachInterrupt(digitalPinToInterrupt(e5.getPinDT()), tickE5, CHANGE);
}

void loop()
{
  // Encoders buttons
  e1.btnClicked();
  e2.btnClicked();
  e3.btnClicked();
  e4.btnClicked();
  e5.btnClicked();
}

// Proxy pour e1.changement
void tickE1()
{
  e1.tick();
}

void tickE2()
{
  e2.tick();
}

void tickE3()
{
  e3.tick();
}

void tickE4()
{
  e4.tick();
}

void tickE5()
{
  e5.tick();
}