#include "MaxLoRa.h"
#include <SoftwareSerial.h>

#define RESET 9
#define TX 8
#define RX 7
#define FREQPLAN FP_US915AB
#define debugSerial Serial

SoftwareSerial mySerial(RX,TX); 
MaxLoRa LoRa(mySerial, debugSerial, RESET, FREQPLAN);


void setup() {

  Serial.begin(115200);
  mySerial.begin(9600);
  Serial.println("Start");
  LoRa.reset();
  LoRa.showStatus();
  while(1){}
}

void loop(){
  while(1){} //do nothing
}
