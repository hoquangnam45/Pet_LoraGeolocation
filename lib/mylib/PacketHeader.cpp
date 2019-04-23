#include "PacketHeader.h"
bool PacketHeader::operator == (const PacketHeader& b){
    return (this->srcAddress == b.srcAddress && this->dstAddress == b.dstAddress);
}
void PacketHeader::setHeader(byte dstAddress, byte srcAddress, byte msgLength){
    this->dstAddress = dstAddress;
    this->srcAddress = srcAddress;
    this->msgLength = msgLength;
    //lenFlag = true;
};
void PacketHeader::setHeader(byte dstAddress, byte srcAddress){
    this->dstAddress = dstAddress;
    this->srcAddress = srcAddress;
    //lenFlag = false;
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
