#include "PacketHandle.h"
#include "heltec.h"

PacketHandle::PacketHandle(byte localAddress){
    this->localAddress = localAddress;
}

PacketHeader PacketHandle::parseHeader(char* msg){
    // get header
    PacketHeader tmp_header;
    byte dstAddr = LoRa.read();
    byte srcAddr = LoRa.read();
    byte msgLen = LoRa.read();
    tmp_header.setHeader(dstAddr, srcAddr, msgLen);

    // get message
    int i = 0;
    int boundary = sizeof(msg) / sizeof(char) - 1;
    while(LoRa.available()){
        if (i < boundary) {
            msg[i] = (char) LoRa.read();
            i++;
        }
    }
    msg[i] = '\n'; // null-terminated message
}

bool PacketHandle::validatePacket(PacketHeader receiveHeader, char* receiveMsg){
    if (receiveHeader.getLength() != strlen(receiveMsg)) return false;
    if (receiveHeader.getDstAddr() != localAddress || receiveHeader.getDstAddr() != 0xFF) return false;
    return true;
}

void PacketHandle::sendMessage(PacketHeader sendHeader, char* msg){
    LoRa.beginPacket();
    LoRa.write(sendHeader.getDstAddr());
    LoRa.write(sendHeader.getSrcAddr());
    LoRa.write(strlen(msg));
    LoRa.print(msg);
    LoRa.endPacket();
}

void PacketHandle::returnMessage(PacketHeader receiveHeader, char* msg){
    LoRa.beginPacket();
    LoRa.write(receiveHeader.getSrcAddr());
    LoRa.write(receiveHeader.getDstAddr());
    LoRa.write(strlen(msg));
    LoRa.print(msg);
    LoRa.endPacket();
}
