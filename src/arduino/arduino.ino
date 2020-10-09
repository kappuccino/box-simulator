#include "Led.h"
#include "Button.h"
#include "Encoder.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Définition des PIN physique //////////////////////////////////////////////////////////////////////////////////
//

// Encoder A ou 1 (Connecter +5v et GND)
#define ROT_A_DT  24
#define ROT_A_CLK 22
#define ROT_A_SW  26

// Encoder B ou 2
#define ROT_B_DT  25
#define ROT_B_CLK 23
#define ROT_B_SW  27

// Boutons
#define BUT_A 4
#define BUT_B 5

// Led
#define LED_A 6
#define LED_B 7

/// https://roboticsbackend.com/arduino-object-oriented-programming-oop/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Déclaration de nos instances /////////////////////////////////////////////////////////////////////////////////
//
// * Le premier argument est le nom humain que l'on donne à nos instances pour les identifier simplement sur les
//   sortie du port série. C'est plus simple de gérer B1 pour le bouton 1, plutot que sur son numéro de PIN
//

// led
Led l1("L1", LED_A);
Led l2("L2", LED_B);

// boutons
Button b1("B1", BUT_A);
Button b2("B2", BUT_B);

// encoder rotatif
Encoder e1("E1", ROT_A_DT, ROT_A_CLK);
Encoder e2("E2", ROT_B_DT, ROT_B_CLK);


void setup() {
  Serial.begin (115200);

  // On attache les led à chaque bouton, (ils décideront quoi faire avec plus tard)
  b1.setup(&l1);
  b2.setup(&l2);

  // encoderA et encoderB sont des proxy pour lancer la fonction `changement` sur les instances
  // on ne peut pas attaquer ces methodes directement ici (limitation de attachInterrupt sur arduino)
  attachInterrupt(digitalPinToInterrupt(e1.getPinDT()), changementE1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(e2.getPinDT()), changementE2, CHANGE);
}


String command;
                 
void loop(){
  
  b1.loop();
  b2.loop();

  if(Serial.available() > 0){
    char c = Serial.read();

    if(c == '\n'){

      String component = command.substring(0, command.indexOf(" "));
      String state = command.substring(command.indexOf(" ") + 1);
  
      Serial.println("Response: \""+ component +"\":\"" + state + "\"");

      l1.update(component, state);
      l2.update(component, state);

      command = "";
    }else{
      command += c;
    }

  }
}


// Proxy pour e1.changement
void changementE1(){
  e1.changement();
}

// Proxy pour e2.changement
void changementE2(){
  e2.changement();
}
