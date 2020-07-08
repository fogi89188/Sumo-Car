#include <IRremote.h>
#define RECV_PIN 4
//Motor A
#define motorPin1 5
#define motorPin2 6
//Motor B
#define motorPin3 10
#define motorPin4 9
#define trigger 7
#define echo 7

int mode = 1;
int cm = 0;
int forwardBool = 0;
int rightBool = 0;
int reverseBool = 0;
int leftBool = 0;
int stopBool = 0;

//Declare instances of objects where we keep the results
IRrecv irrecv(RECV_PIN);
decode_results results;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void right(){
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
}
void left(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
}
void forward(){
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
}
void reverse(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, HIGH);
    digitalWrite(motorPin4, LOW);
}
void stop(){
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, LOW);
}

void setup()
{
  Serial.begin(9600);
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
	irrecv.enableIRIn(); // start the receiver
}

void loop()
{
  // measure the cm
  //cm is equal to the time (in microseconds) that it takes for the sound to send and then go back again.
  //since the sound traveles the distance twice, we then need to devide it by 2
  //speed of sound is aprox. 340m/s which is 0.034cm/us
  //0.034us divided by 2 (as stated above) is 0.017
  //in order to get the distance we need to multiply the time by 0.017
  cm = 0.017 * readUltrasonicDistance(trigger, echo);
  
  Serial.println(cm);
  //do auto if mode is turned on
  //[mode = 1 -> auto]
  //[mode = 0 -> manual]
  if(cm <= 50 && mode == 1){
    //forward
      forwardBool = 1;
      reverseBool = 0;
      leftBool = 0;
      rightBool = 0;
      stopBool = 0;
  }
  else if(mode == 1){
    //right
      forwardBool = 0;
      reverseBool = 0;
      leftBool = 0;
      rightBool = 1;
      stopBool = 0;
  }
  
  //execute the commands if a button is pressed
  if(forwardBool == 1){
   forward(); 
  }
  if(reverseBool == 1){
   reverse(); 
  }
  if(rightBool == 1){
   right(); 
  }
  if(leftBool == 1){
   left(); 
  }
  if(stopBool == 1){
   stop(); 
  }
  
  //prints the mode we are currently using (1 = auto, 0 = manual)
  Serial.println(mode);
  
  //IR remote
  //decode results from remote
  //IF we have received an IR signal, then do:
  if (irrecv.decode(&results)){
   Serial.println(results.value, HEX);
    irrecv.resume(); //receive next value
    
    //if button 2 is pushed, go forward
    if(results.value==0xFD8877 && mode == 0){
      Serial.println("forward");
      forwardBool = 1;
      reverseBool = 0;
      leftBool = 0;
      rightBool = 0;
      stopBool = 0;
    }
    //if button 4 is pushed, go left
    if(results.value==0xFD28D7 && mode == 0){
      Serial.println("left");
      forwardBool = 0;
      reverseBool = 0;
      leftBool = 1;
      rightBool = 0;
      stopBool = 0;
    }
    //if button 6 is pushed, go right
    if(results.value==0xFD6897 && mode == 0){
      Serial.println("right");
      forwardBool = 0;
      reverseBool = 0;
      leftBool = 0;
      rightBool = 1;
      stopBool = 0;
    }
    //if button 8 is pushed, go backwards
    if(results.value==0xFD9867 && mode == 0){
      Serial.println("reverse");
      forwardBool = 0;
      reverseBool = 1;
      leftBool = 0;
      rightBool = 0;
      stopBool = 0;
    }
    //if button 5 is pushed, stop all movements
    if(results.value==0xFDA857 && mode == 0){
      Serial.println("stop");
      forwardBool = 0;
      reverseBool = 0;
      leftBool = 0;
      rightBool = 0;
      stopBool = 1;
    }
    //if ON/OFF button is pushed, change auto modes
    if(results.value==0xFD00FF){
      Serial.println("switch mode");
      forwardBool = 0;
      reverseBool = 0;
      leftBool = 0;
      rightBool = 0;
      stopBool = 1;
      if(mode == 1) { mode = 0; }
      else { mode = 1; }
    }
  }
}