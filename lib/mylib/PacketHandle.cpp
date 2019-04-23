#include "../Heltec_ESP32_Dev-Boards/src/heltec.h"
#include "PacketHandle.h"

extern char Buffer[];

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
    int boundary = packet.BUFFER_SIZE - 1;
    while(LoRa.available()){
        if (i < boundary) {
            msg[i] = (char) LoRa.read();
            i++;
        }
        else LoRa.read();
    }
    msg[i] = '\0'; // null-terminated message
}

void PacketHandle::dumpAllReceived(){
    while(LoRa.parsePacket())
        while(LoRa.available())
            LoRa.read();
}

bool PacketHandle::validatePacket(Packet& packet){
    if (packet.getHeader().getLength() != strlen(packet.getMsg())) return false;
    if (packet.getHeader().getDstAddr() != localAddress && packet.getHeader().getDstAddr() != 0xFF) return false;
    return true;
}

void PacketHandle::sendMessage(PacketHeader& sendHeader, char* msg){
    LoRa.beginPacket();
    LoRa.write(sendHeader.getDstAddr());
    LoRa.write(sendHeader.getSrcAddr());
    LoRa.write(strlen(msg));
    LoRa.print(msg);
    LoRa.endPacket();
}

void PacketHandle::returnMessage(PacketHeader& receiveHeader, char* msg){
    LoRa.beginPacket();
    LoRa.write(receiveHeader.getSrcAddr());
    LoRa.write(receiveHeader.getDstAddr());
    LoRa.write(strlen(msg));
    LoRa.print(msg);
    LoRa.endPacket();
}
void PacketHandle::printPacket(Packet& packet){
    Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    sprintf(Buffer, "Receive address: 0x%02x", packet.getHeader().getDstAddr());
    Serial.println(Buffer);
    sprintf(Buffer, "Source address: 0x%02x", packet.getHeader().getSrcAddr());
    Serial.println(Buffer);
    sprintf(Buffer, "Msg length by header: %d", packet.getHeader().getLength());
    Serial.println(Buffer);
    sprintf(Buffer, "Msg length: %d", strlen(packet.getMsg()));
    Serial.println(Buffer);
    Serial.println("Msg: " + String(packet.getMsg()));
    Serial.println("========================================");
    Serial.println();
}
void PacketHandle::printPacketHeader(PacketHeader& header){
    Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    sprintf(Buffer, "Receive address: 0x%02x", header.getDstAddr());
    Serial.println(Buffer);
    sprintf(Buffer, "Source address: 0x%02x", header.getSrcAddr());
    Serial.println(Buffer);
    sprintf(Buffer, "Msg length by header: %d", header.getLength());
    Serial.println(Buffer);
    Serial.println("========================================");
    Serial.println();
}
