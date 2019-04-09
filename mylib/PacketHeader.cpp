#include "PacketHeader.h"

void PacketHeader::setHeader(byte dstAddress, byte srcAddress, byte msgLength){
    this->dstAddress = dstAddress;
    this->srcAddress = dstAddress;
    this->msgLength = msgLength;
};
byte PacketHeader::getLength(){
    return msgLength;
};
byte PacketHeader::getSrcAddr(){
    return srcAddress;
};
byte PacketHeader::getDstAddr(){
    return dstAddress;
};
