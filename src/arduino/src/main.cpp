#include <Arduino.h>
#include "Led.h"
#include "Button.h"
#include "Encoder.h"

void tickE1();
void tickE2();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des PIN physique //////////////////////////////////////////////////////////////////////////////////
//

// Encoder 1 (E1) 
#define ROT_E1_CLK 22
#define ROT_E1_DT 23
#define ROT_E1_SW 24

// Encoder 2 (E2)
#define ROT_E2_CLK 25
#define ROT_E2_DT 26
#define ROT_E2_SW 27

/// https://roboticsbackend.com/arduino-object-oriented-programming-oop/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Déclaration de nos instances /////////////////////////////////////////////////////////////////////////////////
//
// * Le premier argument est le nom humain que l'on donne à nos instances pour les identifier simplement sur les
//   sortie du port série. C'est plus simple de gérer B1 pour le bouton 1, plutot que sur son numéro de PIN
//

// encodeurs rotatifs
Encoder e1("E1", ROT_E1_DT, ROT_E1_CLK);
Encoder e2("E2", ROT_E2_DT, ROT_E2_CLK);

void setup()
{
  Serial.begin(115200);

  // encoderA et encoderB sont des proxy pour lancer la fonction `changement` sur les instances
  // on ne peut pas attaquer ces methodes directement ici (limitation de attachInterrupt sur arduino)
   attachInterrupt(digitalPinToInterrupt(e1.getPinDT()), tickE1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(e2.getPinDT()), tickE2, CHANGE);
}

void loop()
{
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

