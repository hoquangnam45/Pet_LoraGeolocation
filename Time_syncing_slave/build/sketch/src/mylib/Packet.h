#include <Arduino.h>
#include "PacketHeader.h"
class Packet{
    public:
        PacketHeader& getHeader();
        char* getMsg();
    private:
        PacketHeader header;
        char content[256];
};
