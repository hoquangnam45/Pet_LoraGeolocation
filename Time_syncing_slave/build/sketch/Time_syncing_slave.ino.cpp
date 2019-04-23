#include <Arduino.h>
#line 1 "/home/hoquangnam/Arduino/Project_HTN/Time_syncing_slave/Time_syncing_slave.ino"
#line 1 "/home/hoquangnam/Arduino/Project_HTN/Time_syncing_slave/Time_syncing_slave.ino"
/* 
  Basic test program, send date at the BAND you seted.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "heltec.h"
#include "src/mylib/PacketHandle.h"
#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define MSG_SIZE 256
#define localAddress 0x01

PacketHandle PacketHandler(localAddress);

int counter = 0;

#line 21 "/home/hoquangnam/Arduino/Project_HTN/Time_syncing_slave/Time_syncing_slave.ino"
void setup();
#line 32 "/home/hoquangnam/Arduino/Project_HTN/Time_syncing_slave/Time_syncing_slave.ino"
void loop();
#line 21 "/home/hoquangnam/Arduino/Project_HTN/Time_syncing_slave/Time_syncing_slave.ino"
void setup() {
  
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  // register the receive callback
  //LoRa.onReceive(printReply);
  //LoRa.receive();
  
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // send packet
  PacketHeader sendHeader;
  sendHeader.setHeader('F', 'A');
  PacketHandler.sendMessage(sendHeader, "Sync request");
  
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

// void printReply(int packetSize){
//   //char bufferReceive[MSG_SIZE];
//   char bufferReceive[MSG_SIZE];
//   PacketHeader receiveHeader = PacketHandler.parseHeader(bufferReceive);
//   if(!PacketHandler.validatePacket(receiveHeader, bufferReceive)) return;
//   Serial.print(PacketHeader);
//   Serial.print(bufferReceive);
// }
