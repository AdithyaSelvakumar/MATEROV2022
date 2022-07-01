/*
 * Organization: LHS Horizon
 * Modified by: Adithya Selvakumar et al.
 * 
 * Last modified 4/26/2022 -- changed direction of horizontal motors 2 & 3, switched GREEN and RED LEDs
 */

#include <PololuMaestro.h>
//#include <Pushbutton.h>
#include <Servo.h>

/* Variable Declaration */

/* INPUTS */

//Joystick & Potentiometer Configuration
int JS_PIN_1 = 0;
int JS_PIN_2 = 1;
int JS_PIN_3 = 2;
int JS_PIN_4 = 3;

int POT_PIN_1 = 4;
int POT_PIN_2 = 5;

int jsVal1 = 0;
int jsVal2 = 0;
int jsVal3 = 0;
int jsVal4 = 0;

int potVal1 = 0;
int potVal2 = 0;

int JS_CENTER_1 = 501;
int JS_CENTER_2 = 492;
int JS_CENTER_3 = 496;
int JS_CENTER_4 = 499;

int AXIS_1 = 1;
int AXIS_2 = 1;
int AXIS_3 = 1;
int AXIS_4 = 1;

int POT_RANGE = 128;

//Button Configuration
int BTN_PIN_1 = 44;
int BTN_PIN_2 = 46;
int BTN_PIN_3 = 48;
int BTN_PIN_4 = 50;
int BTN_PIN_5 = 52;


//LED and SOS Configuration
int LED_PIN_1 = 36; //YELLOW
int LED_PIN_2 = 40; //GREEN
int LED_PIN_3 = 42; //BLUE
int LED_PIN_4 = 34; //
int LED_PIN_5 = 38; //RED

int SOS_PIN = 22;

/* OUTPUTS */

//Motor Configuration
const float MOTOR_DEFAULT = 1500;
float horizSpeed = 100;
float vertSpeed = 100;
float MIN_HORIZ_SPEED = 50;
float MIN_VERT_SPEED = 50;
float MAX_HORIZ_SPEED = 200;
float MAX_VERT_SPEED = 200;

float forwardCommand = MOTOR_DEFAULT;
float turnCommand = MOTOR_DEFAULT;
float vertCommand = MOTOR_DEFAULT;
float pitchCommand = MOTOR_DEFAULT;

float horizMtr1Val = MOTOR_DEFAULT;
float horizMtr2Val = MOTOR_DEFAULT;
float horizMtr3Val = MOTOR_DEFAULT;
float horizMtr4Val = MOTOR_DEFAULT;
float vertMtr1Val = MOTOR_DEFAULT;
float vertMtr2Val = MOTOR_DEFAULT;

//Servo Configuration
int SERVO_UPPER_BOUND = 2000;
int SERVO_LOWER_BOUND = 10000;

int servo1Val = SERVO_LOWER_BOUND;
int servo2Val = SERVO_LOWER_BOUND;
int servo3Val = SERVO_LOWER_BOUND;
String servo1State = "CLOSED";
String servo2State = "CLOSED";
int servo3State = 0;

//Maestro Configuration
int HORIZ_MTR1_CHANNEL = 12;
int HORIZ_MTR2_CHANNEL = 13;
int HORIZ_MTR3_CHANNEL = 16;
int HORIZ_MTR4_CHANNEL = 17;
int VERT_MTR1_CHANNEL = 14;
int VERT_MTR2_CHANNEL = 15;
int SERVO1_CHANNEL = 6;
int SERVO2_CHANNEL = 7;
int SERVO3_CHANNEL = 8;


//Misc. Variables (incl. autonomous mode)
int pastMillis = 0;

float DEADBAND = 75;

float TURN_DEADBAND = 0.15;
float PITCH_DEADBAND = 0.3;

float TURN_SPEED_REDUCTION = 0.25;
float turnSpeed = horizSpeed * (1 - TURN_SPEED_REDUCTION);

float deltaX = 50;
int pastCamAngle = 7000;

float AUTO_TIME = 15000;
float AUTO_SPEED_HORIZ = 150;
float AUTO_SPEED_VERT = 0;

boolean isAuto = false;
boolean isBluetooth = false;

/* Object Declaration */

MiniMaestro maestro(Serial3);

/*
Pushbutton btn1(BTN_PIN_1);
Pushbutton btn2(BTN_PIN_2);
Pushbutton btn3(BTN_PIN_3);
Pushbutton btn4(BTN_PIN_4);
Pushbutton btn5(BTN_PIN_5);
*/

void setup()
{
  //Initiate port for comm. w/ Serial Monitor
  Serial.begin(9600); 
  //Initiate port for comm. w/ Maestro
  Serial3.begin(9600);

  maestro.setTarget(HORIZ_MTR1_CHANNEL, MOTOR_DEFAULT * 4.0);
  maestro.setTarget(HORIZ_MTR2_CHANNEL, MOTOR_DEFAULT * 4.0);
  maestro.setTarget(HORIZ_MTR3_CHANNEL, MOTOR_DEFAULT * 4.0);
  maestro.setTarget(HORIZ_MTR4_CHANNEL, MOTOR_DEFAULT * 4.0);
  maestro.setTarget(VERT_MTR1_CHANNEL, MOTOR_DEFAULT * 4.0);
  maestro.setTarget(VERT_MTR2_CHANNEL, MOTOR_DEFAULT * 4.0);

  pinMode(BTN_PIN_1, INPUT_PULLUP);
  pinMode(BTN_PIN_2, INPUT_PULLUP);
  pinMode(BTN_PIN_3, INPUT_PULLUP);
  pinMode(BTN_PIN_4, INPUT_PULLUP);
  pinMode(BTN_PIN_5, INPUT_PULLUP);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  pinMode(LED_PIN_5, OUTPUT);
  pinMode(SOS_PIN, INPUT);

  SOS_CHECK();

  digitalWrite(LED_PIN_1, HIGH);
  digitalWrite(LED_PIN_2, HIGH);

  delay(1000);

  for (int i = 1; i <= 3; i++) {
    
    digitalWrite(LED_PIN_1, LOW);
    digitalWrite(LED_PIN_2, LOW);
    
    delay(1000);

    digitalWrite(LED_PIN_1, HIGH);
    digitalWrite(LED_PIN_2, HIGH);

    delay(1000);
  
  }

  pastMillis = millis();
  
}

void loop()
{

  SOS_CHECK();

  if (digitalRead(BTN_PIN_4) == LOW) {

    maestro.setTarget(HORIZ_MTR1_CHANNEL, ((MOTOR_DEFAULT - AUTO_SPEED_HORIZ) * 4.0));
    maestro.setTarget(HORIZ_MTR2_CHANNEL, ((MOTOR_DEFAULT - AUTO_SPEED_HORIZ) * 4.0));
    maestro.setTarget(HORIZ_MTR3_CHANNEL, ((MOTOR_DEFAULT + AUTO_SPEED_HORIZ) * 4.0));
    maestro.setTarget(HORIZ_MTR4_CHANNEL, ((MOTOR_DEFAULT + AUTO_SPEED_HORIZ) * 4.0));
    maestro.setTarget(VERT_MTR1_CHANNEL, ((MOTOR_DEFAULT + AUTO_SPEED_VERT) * 4.0));
    maestro.setTarget(VERT_MTR2_CHANNEL, ((MOTOR_DEFAULT + AUTO_SPEED_VERT) * 4.0));
    
  } else {
 
      jsVal1 = analogRead(JS_PIN_1);
      jsVal2 = analogRead(JS_PIN_2);
      jsVal3 = analogRead(JS_PIN_3);
      jsVal4 = analogRead(JS_PIN_4);
      
      potVal1 = analogRead(POT_PIN_1);
      potVal2 = analogRead(POT_PIN_2);

      if (digitalRead(BTN_PIN_5) == LOW) {
        
        horizSpeed = 0;
        vertSpeed = 250;
        turnSpeed = 0;
        
      } else {
        
        horizSpeed = map(potVal2, 0, 1023, MIN_HORIZ_SPEED, MAX_HORIZ_SPEED);
        vertSpeed = map(potVal2, 0, 1023, MIN_VERT_SPEED, MAX_VERT_SPEED);
        turnSpeed = horizSpeed * (1 - TURN_SPEED_REDUCTION);

      }

      
        forwardCommand = map(jsVal1, JS_CENTER_1 - POT_RANGE, JS_CENTER_1 + POT_RANGE, -horizSpeed * AXIS_1, horizSpeed * AXIS_1);
        turnCommand = map(jsVal2, JS_CENTER_2 - POT_RANGE, JS_CENTER_2 + POT_RANGE, -horizSpeed * AXIS_2, horizSpeed * AXIS_2);
        vertCommand = map(jsVal3, JS_CENTER_3 - POT_RANGE, JS_CENTER_3 + POT_RANGE, -vertSpeed * AXIS_3, vertSpeed * AXIS_3);
        pitchCommand = map(jsVal4, JS_CENTER_4 - POT_RANGE, JS_CENTER_4 + POT_RANGE, -vertSpeed * AXIS_4, vertSpeed * AXIS_4);
      
      
      if (abs(turnCommand) < horizSpeed * TURN_DEADBAND) {
        turnCommand = 0;
      }

      if (abs(pitchCommand) < vertSpeed * PITCH_DEADBAND) {
        pitchCommand = 0;
      }
    
      horizMtr1Val = MOTOR_DEFAULT - forwardCommand + turnCommand;
      horizMtr2Val = MOTOR_DEFAULT - forwardCommand - turnCommand;
      horizMtr3Val = MOTOR_DEFAULT + forwardCommand - turnCommand;
      horizMtr4Val = MOTOR_DEFAULT + forwardCommand + turnCommand;
      vertMtr1Val = MOTOR_DEFAULT + vertCommand + pitchCommand;
      vertMtr2Val = MOTOR_DEFAULT + vertCommand - pitchCommand;
    
      if (abs(horizMtr1Val - MOTOR_DEFAULT) <= DEADBAND) {
        horizMtr1Val = MOTOR_DEFAULT;
      }
      if (abs(horizMtr2Val - MOTOR_DEFAULT) <= DEADBAND) {
        horizMtr2Val = MOTOR_DEFAULT;
      }
      if (abs(horizMtr3Val - MOTOR_DEFAULT) <= DEADBAND) {
        horizMtr3Val = MOTOR_DEFAULT;
      }
      if (abs(horizMtr4Val - MOTOR_DEFAULT) <= DEADBAND) {
        horizMtr4Val = MOTOR_DEFAULT;
      }
      if (abs(vertMtr1Val - MOTOR_DEFAULT) <= DEADBAND) {
        vertMtr1Val = MOTOR_DEFAULT;
      }
      if (abs(vertMtr2Val - MOTOR_DEFAULT) <= DEADBAND) {
        vertMtr2Val = MOTOR_DEFAULT;
      }
      
      /*
      if (btn1.getSingleDebouncedRelease()) {
    
        Serial.println("Click detected (button 1).");
        
        if (servo1Val == SERVO_LOWER_BOUND) {
          servo1Val = SERVO_UPPER_BOUND;
          servo1State = "OPEN";
          Serial.println("1 OPEN");
        } else {
          servo1Val = SERVO_LOWER_BOUND;
          servo1State = "CLOSED";
          Serial.println("1 CLOSED");
        }
      }
    
      if (btn2.getSingleDebouncedRelease()) {
        
        Serial.println("Click detected (button 2).");
        
        if (servo2Val == SERVO_LOWER_BOUND) {
          servo2Val = SERVO_UPPER_BOUND;
          servo2State = "OPEN";
          Serial.println("2 OPEN");
        } else {
          servo2Val = SERVO_LOWER_BOUND;
          servo2State = "CLOSED";
          Serial.println("2 CLOSED");
        }
      }
    
      servo3Val = map(potVal1, 0, 1023, SERVO_LOWER_BOUND, SERVO_UPPER_BOUND);
      servo3State = map(potVal1, 0, 1023, 0, 180);
      */
    
      /*
    
      if (millis() - pastMillis >= 2000) {
        pastMillis = millis();
    
        if (servo1Val == SERVO_LOWER_BOUND) {
          servo1Val = SERVO_UPPER_BOUND;
        } else {
          servo1Val = SERVO_LOWER_BOUND;
        }
        
        if (servo2Val == SERVO_LOWER_BOUND) {
          servo2Val = SERVO_UPPER_BOUND;
        } else {
          servo2Val = SERVO_LOWER_BOUND;
        }
    
        if (servo3Val == SERVO_LOWER_BOUND) {
          servo3Val = SERVO_UPPER_BOUND;
        } else {
          servo3Val = SERVO_LOWER_BOUND;
        }
      }
    
      */
    
      if (digitalRead(BTN_PIN_1) == HIGH) {
        servo1Val = SERVO_UPPER_BOUND;
      } else {
        servo1Val = SERVO_LOWER_BOUND;
      }
      if (digitalRead(BTN_PIN_2) == HIGH) {
        servo2Val = SERVO_UPPER_BOUND;
      } else {
        servo2Val = SERVO_LOWER_BOUND;
      }
      
      servo3Val = map(potVal1, 0, 1023, SERVO_LOWER_BOUND, SERVO_UPPER_BOUND);

      if (abs(servo3Val - pastCamAngle) < deltaX) {
        servo3Val = pastCamAngle;
      }

      pastCamAngle = servo3Val;
      
      servo3State = map(potVal1, 0, 1023, 0, 180);
      
      // Set the target of channel 0 to 1500 us and channel 1 to 1750 us.
      
      maestro.setTarget(HORIZ_MTR1_CHANNEL, horizMtr1Val * 4);
      maestro.setTarget(HORIZ_MTR2_CHANNEL, horizMtr2Val * 4);
      maestro.setTarget(HORIZ_MTR3_CHANNEL, horizMtr3Val * 4);
      maestro.setTarget(HORIZ_MTR4_CHANNEL, horizMtr4Val * 4);
      maestro.setTarget(VERT_MTR1_CHANNEL, vertMtr1Val * 4);
      maestro.setTarget(VERT_MTR2_CHANNEL, vertMtr2Val * 4);
    
      maestro.setTarget(SERVO1_CHANNEL, servo1Val);
      maestro.setTarget(SERVO2_CHANNEL, servo2Val);
      maestro.setTarget(SERVO3_CHANNEL, servo3Val);
    
      
      
      Serial.println("----TROUBLESHOOTING MENU----");
      Serial.println();
      Serial.println("--AXIS INPUT VALUES--");
      Serial.print("1: "); Serial.println(jsVal1);
      Serial.print("2: "); Serial.println(jsVal2);
      Serial.print("3: "); Serial.println(jsVal3);
      Serial.print("4: "); Serial.println(jsVal4);
      Serial.print("5: "); Serial.println(potVal1);
      Serial.print("6: "); Serial.println(potVal2);
      Serial.println();
      Serial.println("--MOTOR OUTPUT VALUES--");
      Serial.print("L1: "); Serial.println(horizMtr1Val);
      Serial.print("R1: "); Serial.println(horizMtr2Val);
      Serial.print("L2: "); Serial.println(horizMtr3Val);
      Serial.print("R2: "); Serial.println(horizMtr4Val);
      Serial.print("V1: "); Serial.println(vertMtr1Val);
      Serial.print("V2: "); Serial.println(vertMtr2Val);
      Serial.println();
      Serial.println("--SERVO OUTPUT VALUES--");
      Serial.print("1: "); Serial.println(servo1Val);
      Serial.print("2: "); Serial.println(servo2Val);
      Serial.print("3: "); Serial.println(servo3Val);
      Serial.println();
  
      pastMillis = millis();

  }

}

void SOS_CHECK() {

  digitalWrite(LED_PIN_5, digitalRead(SOS_PIN));

  if (digitalRead(SOS_PIN) == 1) {
    //Serial.println("LEAK DETECTED");
  }
  
}
