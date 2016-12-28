#ifndef INCLUDED_BT_MESSAGE
#define INCLUDED_BT_MESSAGE
namespace bt {
// server message
enum MessageType {
    TankMove,
    TankFire, 
    TankExplode,
    TankStateInfo
};

// message data
struct MsgTankMove {
    unsigned char tankId; 
    unsigned char x; 
    unsigned char y; 
}; 


}
#endif
