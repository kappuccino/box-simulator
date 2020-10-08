#define ROT_A_DT  24
#define ROT_A_CLK 22
#define ROT_A_SW  26

#define ROT_B_DT  25
#define ROT_B_CLK 23
#define ROT_B_SW  27

#define BUT_A 4
#define BUT_B 5

#define LED_A 6
#define LED_B 7

bool rot_a;
bool rot_a_prev;
long unsigned rot_a_time;
int rot_a_value = 0;

bool rot_a_sw;
bool rot_a_sw_prev;
bool rot_a_sw_state;

bool rot_b;
bool rot_b_prev;
long unsigned rot_b_time;
int rot_b_value = 0;

bool rot_b_sw;
bool rot_b_sw_prev;
bool rot_b_sw_state;


bool but_a;
bool but_a_prev;
bool but_a_state;

bool but_b;
bool but_b_prev;
bool but_b_state;

/// https://roboticsbackend.com/arduino-object-oriented-programming-oop/

class Led{
  private:
    byte pin;

  public:
    Led(byte pin){
      this->pin = pin;
      init();
    }

    void init(){
      pinMode(pin, OUTPUT);
    }

    void on(){
      Serial.println("ON " + String(pin));
      digitalWrite(pin, LOW);
    }

    void off(){
      Serial.println("OFF " + String(pin));
      digitalWrite(pin, HIGH);      
    }
};

class Button{
  private:
    byte pin;
    byte lastReading;

  public:
    Button(byte pin){
      this->pin = pin;
      lastReading = LOW;
      init();
    }

    void init(){
       pinMode(pin, INPUT);
    }

  
    void loop(Led *led){
      byte reading = digitalRead(pin);

      if(reading == lastReading) return void();
      lastReading = reading;

      if(reading == HIGH){
        Serial.println("Button " + String(pin)  +  " released");
        led->off();
      }else{
        Serial.println("Button " + String(pin)  +  " pressed");
        led->on();
      }
    }

    void press(){
      Serial.println("Button " + String(pin) + " pressed");
    }

    void release(){
      Serial.println("Button " + String(pin) + " released");
    }
    
};



Led l1(LED_A);
Led l2(LED_B);

Button b1(BUT_A);
Button b2(BUT_B);



void setup() {
  Serial.begin (115200);


///////////////////////////////////////

  pinMode(ROT_A_DT, INPUT);
  pinMode(ROT_A_CLK, INPUT);
  rot_a_prev = digitalRead(ROT_A_DT);
  rot_a_time = millis();

  rot_a_sw_prev = digitalRead(ROT_A_SW);
  rot_a_sw_state = rot_a_sw_prev;

//

  pinMode(ROT_B_DT, INPUT);
  pinMode(ROT_B_CLK, INPUT);
  rot_b_prev = digitalRead(ROT_B_DT);
  rot_b_time = millis();

  rot_b_sw_prev = digitalRead(ROT_B_SW);
  rot_b_sw_state = rot_b_sw_prev;


}
                                   
void loop() {

  attachInterrupt(digitalPinToInterrupt(ROT_A_DT), changementA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROT_B_DT), changementB, CHANGE);

  b1.loop(&l1);
  b2.loop(&l2);
  
  /*rot_a_sw = digitalRead(ROT_A_SW);
  if(rot_a_sw != rot_a_sw_prev){
  
    
    if(rot_a_sw == 1 && rot_a_sw_state){
      Serial.println("A");
      ledOn();
      rot_a_sw_state = false;
    }else
    if(rot_a_sw == 1 && !rot_a_sw_state){
      Serial.println("B");
      ledOff();
      rot_a_sw_state = true;
    }

    rot_a_sw_prev = rot_a_sw;
  }
  */

  
}

void ledOn(){
  singleLed(LED_A, true);
  singleLed(LED_B, true);
}

void ledOff(){
  singleLed(LED_A, false);
  singleLed(LED_B, false);
}

void singleLed(int led, bool state){
  if(state){
    digitalWrite(led, LOW);
  }else{
    digitalWrite(led, HIGH);
  }
}

void changementA(){
  // on mesure A
  rot_a = digitalRead(ROT_A_DT);
   
  // controle du temps pour eviter des erreurs 
  if( abs(millis() - rot_a_time) > 50 ){

    // Si B different de l'ancien état de A alors
    if(digitalRead(ROT_A_CLK) != rot_a_prev){
      rot_a_value--;
    }else{
      rot_a_value++;
    }

    // memorisation du temps pour A
    rot_a_time = millis();

    Serial.print("ROTARY A VALUE :");
    Serial.println(rot_a_value);
  } 

  // memorisation de l'état de A
  rot_a_prev = rot_a;
}

void changementB(){
  // on mesure B
  rot_b = digitalRead(ROT_B_DT);
   
  // controle du temps pour eviter des erreurs 
  if( abs(millis() - rot_b_time) > 50 ){

    // Si B different de l'ancien état de B alors
    if(digitalRead(ROT_B_CLK) != rot_b_prev){
      rot_b_value--;
    }else{
      rot_b_value++;
    }

    // memorisation du temps pour A
    rot_b_time = millis();

    Serial.print("ROTARY B VALUE :");
    Serial.println(rot_b_value);
  } 

  // memorisation de l'état de B
  rot_b_prev = rot_b;
}
