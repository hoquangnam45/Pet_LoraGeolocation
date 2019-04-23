//Time syncing test
#include "src/Heltec_ESP32_Dev-Boards/src/heltec.h"
#include "src/mylib/PacketHandle.h"

#define BAND 433E6  //you can set band here directly,e.g. 868E6,915E6
#define localAddress 0x00
#define WAIT_TIME_MS 2000
#define MSG_SIZE 300

PacketHandle PacketHandler(localAddress);
Packet packet, packetConfirm;
volatile uint8_t continueFlag = 0, receiveFlag = 0;
volatile uint32_t receiveTimeStamp = 0, sendTimeStamp = 0;
char Buffer[MSG_SIZE];

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  LoRa.enableCrc();
  LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(7);
}

void loop() {
  receiveFlag = 1;
  LoRa.onReceive(receiveSyncRequest);
  LoRa.receive();
  while(!continueFlag);
  continueFlag = 0;
  delay(10); // delay 10ms for the other to ready receive message
  PacketHandler.returnMessage(packet.getHeader(), "Sync reply");
  sendTimeStamp = LoRa.getSendTimeStamp();

  LoRa.onReceive(receiveSyncRequest);
  LoRa.receive();
  uint32_t startTime = millis();
  while(millis() - startTime < WAIT_TIME_MS && !continueFlag);// Wait for received confirmation
  if (!continueFlag){
    Serial.println("Failed wait");
    return;
  }
  continueFlag = 0;
  
  sprintf(Buffer, "Receive timestamp: %010lu; Send timestamp: %010lu", receiveTimeStamp, sendTimeStamp);
  delay(10); // delay 10ms for the other to ready receive message
  PacketHandler.returnMessage(packet.getHeader(), Buffer);
  Serial.println(Buffer);
  Serial.println();
  Serial.println();
}

void receiveSyncRequest(int packetSize){
  if (receiveFlag == 1){
    receiveTimeStamp = LoRa.getReceiveTimeStamp();
    PacketHandler.parsePacket(packet);
    PacketHandler.printPacket(packet);
    if (!PacketHandler.validatePacket(packet)) return;
    if (strcmp(packet.getMsg(), "Sync request")) return;
    continueFlag = 1;
    receiveFlag = 2;
    LoRa.onReceive(NULL);
  }
  else if (receiveFlag == 2){
    PacketHandler.parsePacket(packetConfirm);
    PacketHandler.printPacket(packetConfirm);
    if (!(packetConfirm.getHeader() == packet.getHeader())) return;
    if(strcmp(packetConfirm.getMsg(), "Received")) return;
    continueFlag = 1;
    receiveFlag = 0;
    LoRa.onReceive(NULL);
  }
}


