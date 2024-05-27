#define pinR 3
#define pinG A2
#define pinB 2
#define inputPir A1
#define STEPPER_PIN_1 5
#define STEPPER_PIN_2 8
#define STEPPER_PIN_3 7
#define STEPPER_PIN_4 6
#define SD_pin 4 
#define PIN_M A0
#define PIN_M_D A3

#include <SD.h>
#include <TMRpcm.h>

File myFile;
TMRpcm tmrpcm;
bool statePir = false; //if motion was detected or not
int motion = 0;
int step_number = 0;
int motionIntervalStop = 0;
int distanceForwardBackwards = 0;
bool stoppedMusic = false;
float voce = 0;
int voce_digital = 0;
int change_color = 0;
int r = 23;
int g = 252;
int bl = 3;
void setup() {
  // put your setup code here, to run once:
  tmrpcm.speakerPin = 9;
  Serial.begin(9600);
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(inputPir, INPUT_PULLUP);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  pinMode(PIN_M_D, INPUT);
  if (!SD.begin(SD_pin)) {
    return;
  }
  tmrpcm.setVolume(5);
  analogWrite(pinR, r);
  analogWrite(pinG, g);
  analogWrite(pinB, bl);
}

void loop() {
  // put your main code here, to run repeatedly
  if(change_color >= 200){
    int val = random(0,30);
    if(r + val > 255){
      r = r - val;
    }else{
      r = r + val;
    }
    val = random(0,100);
    if(g + val > 255){
      g = g - val;
    }else{
      g = g + val;
    }
    val = random(0,100);
    if(bl + val > 255){
      bl = bl - val;
    }else{
      bl = bl + val;
    }
    analogWrite(pinR, r);
    analogWrite(pinG, 0);
    analogWrite(pinB, bl);
    change_color = 0;
    Serial.println(r);
    Serial.println(bl);
    Serial.println(g);
  }
  change_color = change_color + 1;
  motion = digitalRead(inputPir);
  voce_digital = digitalRead(PIN_M_D);
  //Serial.println(voce);
  if(voce_digital == 1){  
    Serial.println("Plange copilul");
    analogWrite(pinR, 254);
    analogWrite(pinG, 254);
    analogWrite(pinB, 254);
  }
  if(motion == HIGH){
    if(statePir == false){
      Serial.println("Motion detected!");
      statePir = true;
      motionIntervalStop = 0;
      if(!tmrpcm.isPlaying()){
         tmrpcm.play("mario.wav");
      }
      if(stoppedMusic == true){
        stoppedMusic = false;
    }
  }
  }else{
    if(statePir == true){
      statePir = false;
      Serial.println("Motion ended!");
    }
    motionIntervalStop = motionIntervalStop + 1;
  }
  if(motionIntervalStop >= 1000){
    tmrpcm.stopPlayback();
    stoppedMusic = true;
  }
  //Serial.println(motionIntervalStop);
  if(distanceForwardBackwards < 640){
    OneStep(true);
  }else{
    OneStep(false);
    if(distanceForwardBackwards >= 1280){
      distanceForwardBackwards = 0;
    }
  }
  distanceForwardBackwards = distanceForwardBackwards + 1;
  delay(50);
}
void OneStep(bool dir){
    if(dir){
switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
} 
  }else{
    switch(step_number){
  case 0:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
  break;
  case 1:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 2:
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
  break;
  case 3:
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
 
  
} 
  }
step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}
