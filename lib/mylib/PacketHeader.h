#include <Arduino.h>

class PacketHeader{
    public:
        void setHeader(byte dstAddress, byte srcAddress, byte msgLength);
        void setHeader(byte dstAddress, byte srcAddress);
        byte getLength();
        byte getSrcAddr();
        byte getDstAddr();
        bool operator == (const PacketHeader& b);
    private:
        //bool lenFlag = false;
        byte srcAddress;
        byte dstAddress;
        byte msgLength;
};
