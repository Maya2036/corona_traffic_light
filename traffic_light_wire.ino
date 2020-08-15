/*Traffic light to control the flow of people. Uses GP2Y0D810Z0F sensor to trigger green light. Sensor distance is ca 10cm. Only uses one sensor, so that its default green on the side without a sensor.*/

//pin setup
#define SENSOR1_PIN 2
#define Y1_PIN 3 //yellow light indicating that sensor has been activated 
#define GREEN1_PIN 4
#define RED1_PIN 5
#define GREEN2_PIN 6
#define RED2_PIN 7


int output_pin_list [7]={Y1_PIN, GREEN1_PIN, RED1_PIN, GREEN2_PIN, RED2_PIN}; //to simplify pin setup



//constants
#define DELAY_TIME 1*1000      // how long time between red on one light to green on the other light (milliseconds)
#define ON_TIME 3*1000        // how long period light should stay green after button is pressed (milliseconds)

//variables
/* -1  delay mode
 *  0  default (led1 red, led2 green)
 *  1  led1 green, led2 red 
 */
int current_state=0 ;

/*  0   none next
 *  1   led1 will be green next
 */
int button_state=0;

unsigned long current_time;
unsigned long button_pressed_time;



void setup() {
  //setup pins 
  pinMode(SENSOR1_PIN, INPUT);

  for(int i: output_pin_list){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  
  digitalWrite(RED1_PIN, HIGH);
  digitalWrite(GREEN2_PIN, HIGH);
}


bool check_button(){ 
  //check if button has been pressed
  if (digitalRead(SENSOR1_PIN)==LOW){ // sensor return LOW when object detected (range about 10cm)
      button_state=1;
      digitalWrite(Y1_PIN, HIGH);
      return true;
    }
  return false;
}

void delay_state(){ //both lights are red 
  current_state=-1;
  button_pressed_time=current_time;
  digitalWrite(RED1_PIN, HIGH);
  digitalWrite(RED2_PIN, HIGH);
  digitalWrite(GREEN1_PIN, LOW);
  digitalWrite(GREEN2_PIN, LOW);
}

void green_state(){ //led1 is green (led1 is the one connected to the sensor)
  current_state=1;
  button_pressed_time=current_time;
  button_state=0;
  digitalWrite(Y1_PIN, LOW);
  digitalWrite(RED1_PIN, LOW);
  digitalWrite(RED2_PIN, HIGH);
  digitalWrite(GREEN1_PIN, HIGH);
  digitalWrite(GREEN2_PIN, LOW);
}

void red_state(){ //led2 green (default)
  current_state=0;
  button_pressed_time=current_time;
  digitalWrite(RED1_PIN, HIGH);
  digitalWrite(RED2_PIN, LOW);
  digitalWrite(GREEN1_PIN, LOW);
  digitalWrite(GREEN2_PIN, HIGH);
  digitalWrite(Y1_PIN, LOW);
}

void check_current_state(){ 
  if (current_state==0){ //default mode
    if(button_state==1){ //if button has been pressed
      delay_state(); 
    }
  }
  else if (current_state==1){ //button pressed and led1 is green
    if ((current_time-button_pressed_time)>ON_TIME){
      if (button_state==1){ //stay green if button is pressed continuously 
        green_state(); 
      }
      else {
        delay_state();
      }
    }
  }
  else if (current_state==-1){
    if ((current_time-button_pressed_time)>DELAY_TIME){
      if(button_state==1){
        green_state(); 
      }
      else{
        red_state();
      }
    }
  }  
}



void loop() {
  current_time=millis();
  check_button();
  check_current_state();
  delay(300);
}
  
