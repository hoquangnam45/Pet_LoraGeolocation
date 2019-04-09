//Time syncing test
#include "heltec.h"
#include "src/mylib/PacketHandle.h"
#define BAND 433E6  //you can set band here directly,e.g. 868E6,915E6
#define MSG_SIZE 256
#define localAddress 0xFF

PacketHandle PacketHandler(localAddress);

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

}

void getReceiveTimeStamp(int packetSize){
  unsigned long temp_receiveTimeStamp = ESP.getCycleCount();
  //
  // get offset
  unsigned long start = ESP.getCycleCount();
  int offsetBase = ESP.getCycleCount() - start;
  
  unsigned long receiveTimeStamp = temp_receiveTimeStamp - offsetBase;
  
  char bufferReceive[MSG_SIZE];
  char bufferSend[MSG_SIZE];
  PacketHeader receiveHeader;// = PacketHandler.parseHeader(bufferReceive);
  int test = receiveHeader.getSrcAddr();
  if(!PacketHandler.validatePacket(receiveHeader, bufferReceive)) return;
  if (strcmp(bufferReceive, "Sync request")) return;
  
  PacketHandler.returnMessage(receiveHeader, "Sync reply");
  unsigned long temp_sendTimeStamp = ESP.getCycleCount();
  unsigned long sendTimeStamp = temp_sendTimeStamp - offsetBase;

  sprintf(bufferSend, "Master timestamp receive - %lu; Master timestamp send - %lu", receiveTimeStamp, sendTimeStamp);
  PacketHandler.returnMessage(receiveHeader, bufferSend);
}

