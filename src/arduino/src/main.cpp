#include <Arduino.h>
#include "Led.h"
#include "Button.h"
#include "Encoder.h"

void tickE1();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des PIN physique //////////////////////////////////////////////////////////////////////////////////
//

// Encoder A ou 1 (Connecter +5v et GND)
#define ROT_A_DT 24
#define ROT_A_CLK 22
#define ROT_A_SW 26


/// https://roboticsbackend.com/arduino-object-oriented-programming-oop/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Déclaration de nos instances /////////////////////////////////////////////////////////////////////////////////
//
// * Le premier argument est le nom humain que l'on donne à nos instances pour les identifier simplement sur les
//   sortie du port série. C'est plus simple de gérer B1 pour le bouton 1, plutot que sur son numéro de PIN
//

// encodeurs rotatifs
Encoder e1("E1", ROT_A_DT, ROT_A_CLK);
//Encoder e2("E2", ROT_B_DT, ROT_B_CLK);

void setup()
{
  Serial.begin(115200);

  // encoderA et encoderB sont des proxy pour lancer la fonction `changement` sur les instances
  // on ne peut pas attaquer ces methodes directement ici (limitation de attachInterrupt sur arduino)
  attachInterrupt(digitalPinToInterrupt(e1.getPinDT()), tickE1, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(e2.getPinDT()), changementE2, CHANGE);
}

void loop()
{
}

// Proxy pour e1.changement
void tickE1()
{
  e1.tick();
}

// Proxy pour e2.changement
/*void changementE2(){
  e2.changement();
}*/