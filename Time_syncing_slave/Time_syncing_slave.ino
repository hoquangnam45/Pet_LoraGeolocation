//Time syncing test
#include "heltec.h"
#include "../lib/Packet.h"
#define BAND 433E6  //you can set band here directly,e.g. 868E6,915E6

volatile bool readFlag = false;
volatile unsigned long receiveTimeStamp = 0, sendTimeStamp = 0;
volatile int incomingPacketSize = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  // register the receive callback
  LoRa.onReceive(getReceiveTimeStamp);
  LoRa.receive();
}

void loop() {
  // put your main code here, to run repeatedly:

}
