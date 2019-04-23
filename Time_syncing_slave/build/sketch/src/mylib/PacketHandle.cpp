#include "heltec.h"
#include "PacketHandle.h"

char buffer[32];

PacketHandle::PacketHandle(byte localAddress){
    this->localAddress = localAddress;
}

void PacketHandle::parsePacket(Packet& packet){
    // get header
    byte dstAddr = LoRa.read();
    byte srcAddr = LoRa.read();
    byte msgLen = LoRa.read();
    packet.getHeader().setHeader(dstAddr, srcAddr, msgLen);

    // get message
    int i = 0;
    char* msg = packet.getMsg();
    int boundary = sizeof(msg) / sizeof(char) - 1;
    while(LoRa.available()){
        if (i < boundary) {
            msg[i] = (char) LoRa.read();
            i++;
        }
    }
    msg[i] = '\n'; // null-terminated message
    //return packet;
}

bool PacketHandle::validatePacket(Packet& packet){
    if (packet.getHeader().getLength() != strlen(packet.getMsg())) return false;
    if (packet.getHeader().getDstAddr() != localAddress || packet.getHeader().getDstAddr() != 0xFF) return false;
    return true;
}

unsigned long PacketHandle::sendMessage(PacketHeader& sendHeader, char* msg){
    LoRa.beginPacket();
    LoRa.write(sendHeader.getDstAddr());
    LoRa.write(sendHeader.getSrcAddr());
    LoRa.write(strlen(msg) + 36);
    LoRa.print(msg);
    LoRa.endPacket();
    return ESP.getCycleCount();
}

unsigned long PacketHandle::returnMessage(PacketHeader& receiveHeader, char* msg){
    LoRa.beginPacket();
    LoRa.write(receiveHeader.getSrcAddr());
    LoRa.write(receiveHeader.getDstAddr());
    LoRa.write(strlen(msg));
    LoRa.print(msg);
    LoRa.endPacket();
    return ESP.getCycleCount();
}
void PacketHandle::printPacket(Packet& packet){
    Serial.println("**********************************");
    sprintf(buffer, "Receive address: 0x%02x", packet.getHeader().getDstAddr());
    Serial.println(buffer);
    sprintf(buffer, "Source address: 0x%02x", packet.getHeader().getSrcAddr());
    Serial.println(buffer);
    sprintf(buffer, "Msg length by header: %d", packet.getHeader().getLength());
    Serial.println(buffer);
    sprintf(buffer, "Msg length: %d", strlen(packet.getMsg()));
    Serial.println(buffer);
    Serial.println("Msg: " + String(packet.getMsg()));
    Serial.println("**********************************");
    Serial.println();
}
void PacketHandle::printPacketHeader(PacketHeader& header){
    Serial.println("**********************************");
    sprintf(buffer, "Receive address: 0x%02x", header.getDstAddr());
    Serial.println(buffer);
    sprintf(buffer, "Source address: 0x%02x", header.getSrcAddr());
    Serial.println(buffer);
    sprintf(buffer, "Msg length by header: %d", header.getLength());
    Serial.println(buffer);
    Serial.println("**********************************");
    Serial.println();
}