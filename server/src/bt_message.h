#ifndef INCLUDED_BT_MESSAGE
#define INCLUDED_BT_MESSAGE

#include <bt_map.h>

namespace bt {
// server message
enum MessageType {
    MsgTypeGameReady,
    MsgTypeMapData,
    MsgTypePlayerReady,
    MsgTypeTankMove,
    MsgTypeTankFire, 
    MsgTypeTankExplode,
    MsgTypeTankStateInfo
};

struct MsgMapData {
    char map[Map::SIZE][Map::SIZE]; 
};

// message data
struct MsgTankMove {
    unsigned char tankId; 
    unsigned char x; 
    unsigned char y; 
}; 


}
#endif
