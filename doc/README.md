# Documentation

### Board
Ce projet peut fonctionner avec toute les boards Arduino, cependant si vous voulez avoir plusieurs rotary encoder,
il exsite des limitations sur les boards les plus simples
([voir les explications sur la doc arduino](ttps://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)).

La version Arduino DUE offre un nombre important de digital IO et permet d'utiliser chacune de ses IO pour
les `attachInterrupt`

### Simple
![Branchement simple](https://raw.githubusercontent.com/kappuccino/box-simulator/main/doc/cablage-simple.png)

Pour la version la plus simple vous pouvez simplement brancher un rotary encoder sur 2 Pin de la board. Par exemple
`22` et `26`. Pensez à définir votre code tel que:


```
// ROT_A est notre premier/seul rotary encoder
#define ROT_A_DT  26
#define ROT_A_CLK 22

...

//
Encoder e1("rotary+1", ROT_A_DT, ROT_A_CLK);
``` 

Faites tourner le bouton pour faire envoyer des messages sur le port série. Depuis l'IDE arduino vous pouvez afficher
un monitoring du port de la board pour voir les messages
