#include <MaxLoRa.h>

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#define debugSerial Serial
//Check for serial plan available in your region
#define freqPlan FP_US915AB 
#define RX 7
#define TX 8
#define RESET 9


SoftwareSerial LoRaSerial(RX, TX);
MaxLoRa buffer(LoRaSerial, debugSerial, RESET, freqPlan);

uint8_t message[]="Hello World from LoRa";

void setup()
{
  LoRaSerial.begin(57600);
  Serial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  Serial.println("-- PERSONALIZE");
  LoRa.reset();
  LoRa.personalize(devAddr, nwkSKey, appSKey);

  Serial.println("-- STATUS");
  LoRa.showStatus();
}

void loop()
{
  Serial.println("-- LOOP");.
  //Send Hello message
  LoRa.sendBytes(buffer, sizeof(buffer), 1, false, 8);

  delay(10000);
}
