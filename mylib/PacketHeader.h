#include <Arduino.h>

class PacketHeader{
    public:
        void setHeader(byte dstAddress, byte srcAddress, byte msgLength);
        byte getLength();
        byte getSrcAddr();
        byte getDstAddr();
    private:
        byte srcAddress;
        byte dstAddress;
        byte msgLength;
};
