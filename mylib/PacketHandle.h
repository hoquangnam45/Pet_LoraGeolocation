#include <Arduino.h>
#include "PacketHeader.h"
class PacketHandle{
    public:
        PacketHandle(byte localAddress);
        PacketHeader parseHeader(char* msg);
        bool validatePacket(PacketHeader receiveHeader, char* receiveMsg);
        void sendMessage(PacketHeader sendHeader, char* msg);
        void returnMessage(PacketHeader receiveHeader, char* msg);
    private:
        bool setFlag = false;
        byte localAddress = 0xFF;
};
