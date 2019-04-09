#include "Arduino.h"
class Packet{
    public:
        Packet()
    private:
        byte destination;
        byte localAddress;
        byte msgID;
        byte msgLength;
        char msg[255];
}

