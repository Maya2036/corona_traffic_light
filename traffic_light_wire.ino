/*Traffic light to control the flow of people. Uses HC-SRO4 to trigger green light for people*/
#define SENSOR 0 //

//pin setup
//if (SENSOR){
#define ECHO2_PIN 2
#define ECHO1_PIN 3
#define TRIG2_PIN 4
#define TRIG1_PIN 5
//}

//else {
  #define SENSOR1_PIN 2
  #define SENSOR2_PIN 3
//}
  



#define Y2_PIN 6
#define Y1_PIN 7
#define GREEN1_PIN 8
#define RED1_PIN 9
#define GREEN2_PIN 10
#define RED2_PIN 11

//int output_pin_list [9]={TRIG1_PIN, TRIG2_PIN, Y1_PIN, Y2_PIN, GREEN1_PIN, RED1_PIN, GREEN2_PIN, RED2_PIN};
//if (!SENSOR){
  int output_pin_list [7]={ Y1_PIN, Y2_PIN, GREEN1_PIN, RED1_PIN, GREEN2_PIN, RED2_PIN};
//}


//constants
#define SPEED_OF_SOUND 0.0343  // (cm/microsec)

#define TRIGGER_DISTANCE 10    // how close you need to be before button is triggered (cm)
#define DELAY_TIME 1*1000      // how long time between red on one light to green on the other light (milliseconds)
#define ON_TIME 3*1000        // how long period light should stay green after button is pressed (milliseconds)

//variables

/* -1  delay mode
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
int button_state=0;

unsigned long current_time;
unsigned long button_pressed_time;



void setup() {


  Serial.begin(9600);
  
  // put your setup code here, to run once:
 // if (SENSOR){
  //pinMode(ECHO1_PIN, INPUT);
  //pinMode(ECHO2_PIN, INPUT);}
  //else {
    pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  //}

  for(int i: output_pin_list){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  digitalWrite(RED1_PIN, HIGH);
  digitalWrite(RED2_PIN, HIGH);

  
}

/*
int distance(int sensor=1){
  //default read sensor 1.
  int trig=TRIG1_PIN;
  int echo=ECHO1_PIN;
  if (sensor!=1){
    trig=TRIG2_PIN;
    echo=ECHO2_PIN;
    }
  
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
*/

bool object_detected(int sensor =1){
  if (sensor == 1){
    if (digitalRead(SENSOR1_PIN)==HIGH) {return 0;}
    else {return 1;}
  }
  else{
    if (digitalRead(SENSOR2_PIN==HIGH)) {return 0;}
    else {return 1;}
  }
}


bool check_1(){
  //if (SENSOR){return distance(1)<TRIGGER_DISTANCE;}
   {return object_detected(1);}
}

bool check_2(){
  //if (SENSOR){return distance(2)<TRIGGER_DISTANCE;}
   {return object_detected(2);}
}
void check_buttons(){
  
  if (digitalRead(SENSOR1_PIN)==LOW){
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
  if (digitalRead(SENSOR2_PIN)==LOW){
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
  //Serial.print("B: ");
  //Serial.print(button_state);
}

bool check_button1_state(){
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
bool check_button2_state(){
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

void check_current_state(){
  
  if (current_state==0){
    //Serial.print(", c:");
    //Serial.print(current_state);
    //Serial.println();
    if (!check_button1_state()){
      check_button2_state();
    }
  }
    else if (current_state==1){
      if ((current_time-button_pressed_time)>ON_TIME){
        if (!check_button1_state()){
          current_state=-1;
          button_pressed_time=current_time;
          digitalWrite(RED1_PIN, HIGH);
          //digitalWrite(RED2_PIN, HIGH);
          digitalWrite(GREEN1_PIN, LOW);
          //digitalWrite(GREEN2_PIN, LOW);
        }
      }
     }
    else if (current_state==2){
      if ((current_time-button_pressed_time)>ON_TIME){
        if (!check_button2_state()){
          current_state=-1;
          button_pressed_time=current_time;
          //digitalWrite(RED1_PIN, HIGH);
          digitalWrite(RED2_PIN, HIGH);
          //digitalWrite(GREEN1_PIN, LOW);
          digitalWrite(GREEN2_PIN, LOW);
        }
      }
     }
    else if (current_state==-1){
      if ((current_time-button_pressed_time)>DELAY_TIME){ 
        current_state=0; 
      }
    }  
  }



void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(2));
  Serial.print(digitalRead(3));
  current_time=millis();
  check_buttons();
  check_current_state();
  delay(300);

}
  
