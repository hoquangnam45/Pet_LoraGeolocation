#include <Arduino.h>
#include "PacketHeader.h"
class Packet{
    public:
        PacketHeader& getHeader();
        char* getMsg();
        static const uint8_t BUFFER_SIZE = 255;
    private:
        PacketHeader header;
        char content[BUFFER_SIZE];
};
