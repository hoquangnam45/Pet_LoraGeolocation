//Time syncing test
#include "heltec.h"
#define BAND 433E6  //you can set band here directly,e.g. 868E6,915E6

volatile bool readFlag = false;
volatile unsigned long receiveTimeStamp = 0, sendTimeStamp = 0;
volatile int incomingPacketSize = 0;

const byte localAddress = 0x00;

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
  //Serial.println(offsetBase);
  if (readFlag){
    readFlag = false;
    
    char requestBuffer[255];

    // read packet header bytes:
    byte recipient = LoRa.read();          // recipient address
    byte sender = LoRa.read();            // sender address
    byte incomingLength = LoRa.read();    // incoming msg length

    while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }
    // read packet
    for (int i = 0; i < incomingPacketSize; i++)
      requestBuffer[i] = (char)LoRa.read();
    requestBuffer[incomingPacketSize] = '\n';
    if (strcmp(requestBuffer, "Sync request")) return;


    LoRa.beginPacket();
    LoRa.print("Master receive timestamp: " + receiveTimeStamp);
    LoRa.endPacket();
    
    unsigned long temp_sendTimeStamp = ESP.getCycleCount();
  
    // get offset
    unsigned long start = ESP.getCycleCount();
    int offsetBase = ESP.getCycleCount() - start;
    sendTimeStamp = temp_sendTimeStamp - offsetBase;

    LoRa.beginPacket();
    LoRa.print("Master send timestamp: " + sendTimeStamp);
    LoRa.endPacket();
  }
}

void getReceiveTimeStamp(int packetSize){
  unsigned long temp_receiveTimeStamp = ESP.getCycleCount();
  
  // get offset
  unsigned long start = ESP.getCycleCount();
  int offsetBase = ESP.getCycleCount() - start;
  
  if (readFlag) {
    LoRa.beginPacket();
    LoRa.print("Wait a minute");
    LoRa.endPacket();
    return;
  }
  readFlag = true;
  receiveTimeStamp = temp_receiveTimeStamp - offsetBase;
  incomingPacketSize = packetSize;
}

void sendMessage(String outgoing, byte destination) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

