#include "Packet.h"
PacketHeader& Packet::getHeader(){
    return header;
}
char* Packet::getMsg(){
    return content;
}