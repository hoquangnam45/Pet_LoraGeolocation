#include <Arduino.h>
#include "Packet.h"

//extern const int MSG_SIZE;


class PacketHandle{
    public:
        PacketHandle(byte localAddress);
        void parsePacket(Packet& packet);
        //Packetvoid parsePacket(Packet& packet);
        bool validatePacket(Packet& receivePacket);
        //unsigned long sendMessage(PacketHeader& sendHeader, char* msg);
        void sendMessage(PacketHeader& sendHeader, char* msg);
        //unsigned long returnMessage(PacketHeader& receiveHeader, char* msg);
        void returnMessage(PacketHeader& sendHeader, char* msg);
        void printPacket(Packet& packet);
        void printPacketHeader(PacketHeader& header);
        void dumpAllReceived();
    private:
        //bool setFlag = false;
        byte localAddress = 0xFF;
};
