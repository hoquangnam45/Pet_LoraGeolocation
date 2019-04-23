#include <Arduino.h>
#include "Packet.h"
class PacketHandle{
    public:
        PacketHandle(byte localAddress);
        void parsePacket(Packet& packet);
        bool validatePacket(Packet& receivePacket);
        unsigned long sendMessage(PacketHeader& sendHeader, char* msg);
        unsigned long returnMessage(PacketHeader& receiveHeader, char* msg);
        void printPacket(Packet& packet);
        void printPacketHeader(PacketHeader& header);
    private:
        bool setFlag = false;
        byte localAddress = 0xFF;
};
