/* 
  Basic test program, send date at the BAND you seted.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "src/Heltec_ESP32_Dev-Boards/src/heltec.h"
#include "src/mylib/PacketHandle.h"

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define MSG_SIZE 255
#define localAddress 0x01
#define WAIT_TIME_MS 2000
#define OVERHEAD_IN_CYCLE_COUNT

PacketHandle PacketHandler(localAddress);
Packet packet;
volatile uint8_t waitForReply = 0, continueFlag = 0;
volatile uint32_t receiveTimeStamp = 0, sendTimeStamp = 0;
volatile uint32_t svr_receiveTimeStamp = 0, svr_sendTimeStamp = 0;
char Buffer[MSG_SIZE];
long offsetCPUCount = 0;

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  LoRa.enableCrc();
  LoRa.setCodingRate4(8);
  LoRa.setSpreadingFactor(7);
}

void loop() {
  PacketHeader sendHeader;
  sendHeader.setHeader(0x00, localAddress);
  Serial.println("Sending packet: ");
  PacketHandler.sendMessage(sendHeader, "Sync request");
  sendTimeStamp = LoRa.getSendTimeStamp();
  waitForReply = 1;
  
  LoRa.onReceive(getResponse);
  LoRa.receive();
  unsigned long startTime = millis();
  while(millis() - startTime < WAIT_TIME_MS && continueFlag == 0);
  if (!continueFlag){
    Serial.println("Failed wait 1");
    return;
  }
  continueFlag = 0;

  delay(10); // delay 10ms for the other to ready receive message
  PacketHandler.sendMessage(sendHeader, "Received");
  LoRa.onReceive(getResponse);
  LoRa.receive();
  startTime = millis();
  while(millis()- startTime  < WAIT_TIME_MS && continueFlag == 0);
  if (!continueFlag){
    Serial.println("Failed wait 2");
    return;
  }
  continueFlag = 0;
  
  sprintf(Buffer, "Send timestamp: %010lu; Server receive timestamp: %010lu; ", sendTimeStamp, svr_receiveTimeStamp);
  Serial.println(Buffer);
  sprintf(Buffer, "Server send timestamp: %010lu; Receive timestamp: %010lu; ", svr_sendTimeStamp, receiveTimeStamp);
  Serial.println(Buffer);
  Serial.println(getOffset(sendTimeStamp, svr_receiveTimeStamp, svr_sendTimeStamp, receiveTimeStamp));
  delay(2000);
}

void getResponse(int packetSize){
  if (waitForReply == 1){
    receiveTimeStamp = LoRa.getReceiveTimeStamp();
    PacketHandler.parsePacket(packet);
    PacketHandler.printPacket(packet);
    if (!PacketHandler.validatePacket(packet)) return;
    if (strcmp(packet.getMsg(), "Sync reply")) return;
    continueFlag = 1;
    waitForReply = 2;
    LoRa.onReceive(NULL);
  }
  else if (waitForReply == 2){
    PacketHandler.parsePacket(packet);
    PacketHandler.printPacket(packet);
    if (!PacketHandler.validatePacket(packet)) return;
    if (sscanf(packet.getMsg(), "Receive timestamp: %10lu; Send timestamp: %10lu", &svr_receiveTimeStamp, &svr_sendTimeStamp) != 2) return;
    continueFlag = 1;
    waitForReply = 0;
    LoRa.onReceive(NULL);
  }  
}

long getOffset(unsigned long sendTimeStamp, unsigned long svr_receiveTimeStamp, unsigned long svr_sendTimeStamp, unsigned long receiveTimeStamp){
  return floor(((double)(receiveTimeStamp - sendTimeStamp - (svr_sendTimeStamp - svr_receiveTimeStamp)) / 2) - 163900);
}

//unsigned long shiftTimeStamp(unsigned long timestamp, unsigned long svr_timestamp, unsigned long offset){
//  return timestamp - svr_timestamp + offset;
//}

