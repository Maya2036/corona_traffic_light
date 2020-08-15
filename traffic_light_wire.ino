/*Traffic light to control the flow of people. Uses HC-SRO4 to trigger green light for people. Default is red on both until sensor is triggered*/

//pin setup
#define ECHO2_PIN 2
#define ECHO1_PIN 3
#define TRIG2_PIN 4
#define TRIG1_PIN 5
#define Y2_PIN 6
#define Y1_PIN 7
#define GREEN1_PIN 8
#define RED1_PIN 9
#define GREEN2_PIN 10
#define RED2_PIN 11

//constants
#define SPEED_OF_SOUND 0.0343 // (cm/microsec)
#define TRIGGER_DISTANCE 25  // how close you need to be before button is triggered (cm)
#define DELAY_TIME 5*1000 // how long time between red on one light to green on the other light (milliseconds)
#define ON_TIME 10*1000 // how long period light should stay green after button is pressed (milliseconds)

//variables
/* -1  delay mode (like red-yellow light for traffic light. Happens after each green light)
 *  0  both red
 *  1  led1 green, led2 red 
 *  2  led1 red, led2 green
 */
int current_state=0 ;

/*  0   none next
 *  1   led1 will be green next
 *  2   led2 will be green next 
 *  12  led1 then led2 
 *  21  led2 then led1
 */
int button_state=0; //(to be able to remember which light is next in the queue)

unsigned long current_time;
unsigned long button_pressed_time;

int output_pin_list [9]={TRIG1_PIN, TRIG2_PIN, Y1_PIN, Y2_PIN, GREEN1_PIN, RED1_PIN, GREEN2_PIN, RED2_PIN}; //to simplify pin setup


void setup() {
  pinMode(ECHO1_PIN, INPUT);
  pinMode(ECHO2_PIN, INPUT);

  for(int i: output_pin_list){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  
  digitalWrite(RED1_PIN, HIGH);
  digitalWrite(RED2_PIN, HIGH);
}


int distance(int sensor=1){ //read distance from sensor
  //default read sensor 1.
  int trig=TRIG1_PIN;
  int echo=ECHO1_PIN;
  if (sensor!=1){ //sensor2
    trig=TRIG2_PIN;
    echo=ECHO2_PIN;
  }

  //code from https://www.makerguides.com/hc-sr04-arduino-tutorial/
  // Clear the trigPin by setting it LOW:
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  long duration = pulseIn(echo, HIGH);
  // Calculate and return the distance in cm
  Serial.println(round((duration * SPEED_OF_SOUND)));
  return round((duration * SPEED_OF_SOUND) / 2);
}


void check_buttons(){ //chech if sensor triggered
  if (distance(1)<TRIGGER_DISTANCE){
    if (button_state==0){
      button_state=1;
      digitalWrite(Y1_PIN, HIGH);
    }
    else if (button_state==2){
      button_state=21;
      digitalWrite(Y1_PIN, HIGH);
    }
   delay(200); 
  }
  if (distance(2)<TRIGGER_DISTANCE){
    //button2_pressed=true;
    if (button_state==0){
      button_state=2;
      digitalWrite(Y2_PIN, HIGH);
    }
    else if (button_state==1){
      button_state=12;
      digitalWrite(Y2_PIN, HIGH);
    }
  delay(200);
  }
}

bool check_button1_state(){ //check if led1 is next by reading the button_state and run appropriate code if thats the case 
  if (button_state==1){
    current_state=1;
    button_state=0;
    button_pressed_time=current_time;
    digitalWrite(Y1_PIN, LOW);
    digitalWrite(GREEN1_PIN, HIGH);
    digitalWrite(RED1_PIN, LOW);
    return true;
  }
  else if (button_state==12){
    current_state=1;
    button_pressed_time=current_time;
    button_state=2;
    digitalWrite(Y1_PIN, LOW);
    digitalWrite(GREEN1_PIN, HIGH);
    digitalWrite(RED1_PIN, LOW);
    return true;
  }
  else return false;
}
bool check_button2_state(){ //check if led2 is next by reading the button_state and run appropriate code if thats the case 
  if (button_state==2){
    current_state=2;
    button_state=0;
    button_pressed_time=current_time;
    digitalWrite(Y2_PIN, LOW);
    digitalWrite(GREEN2_PIN, HIGH);
    digitalWrite(RED2_PIN, LOW);
    return true;
  }
  else if (button_state==21){
    current_state=2;
    button_pressed_time=current_time;
    button_state=1;
    digitalWrite(Y2_PIN, LOW);
    digitalWrite(GREEN2_PIN, HIGH);
    digitalWrite(RED2_PIN, LOW);
    return true;
  }
  else return false;
}

void check_current_state(){ //read current_state and button_state to decide what to do  
  if (current_state==0){ //default (both red)
    if (!check_button1_state()){
      check_button2_state();
    }
  }
  else if (current_state==1){ //button1 pressed and led1t is now green 
    if ((current_time-button_pressed_time)>ON_TIME){
      if (!check_button1_state()){ //if next up is also led1 then skip delay and take a new period with green
        current_state=-1;
        button_pressed_time=current_time;
        digitalWrite(RED1_PIN, HIGH);
        digitalWrite(GREEN1_PIN, LOW);
      }
    }
   }
  else if (current_state==2){ //button1 pressed and led2 is now green 
    if ((current_time-button_pressed_time)>ON_TIME){
      if (!check_button2_state()){ //if next up is also led2 then skip delay and take a new period with green
        current_state=-1;
        button_pressed_time=current_time;
        digitalWrite(RED2_PIN, HIGH);
        digitalWrite(GREEN2_PIN, LOW);
      }
    }
   }
  else if (current_state==-1){ //delay state 
    if ((current_time-button_pressed_time)>DELAY_TIME){ 
      current_state=0; 
    }
  }  
}



void loop() {
  current_time=millis();
  check_buttons();
  check_current_state();
  delay(300);
}
  
