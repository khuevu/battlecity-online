#ifndef INCLUDED_BT_MESSAGE
#define INCLUDED_BT_MESSAGE

#include <bt_map.h>

namespace bt {
// server message
enum MessageType {
    MsgTypeGameReady, // s->c, when two player joined
    MsgTypeRequestLevelStart, // c->s, when player request to start a level
    MsgTypeLevelMapData, // s->c, map data
    MsgTypeLevelReady, // c->s, after client has received map data
    MsgTypeLevelStart, // s->c, tell client it should start level
    MsgTypeTankMove,
    MsgTypeTankFire, 
    MsgTypeTankExplode,
    MsgTypeTankStateInfo
};

struct MsgRequestGameStart {
    unsigned char levelNumber;
};

struct MsgMapData {
    char map[Map::SIZE][Map::SIZE]; 
};

struct MsgLevelStart {
    unsigned long startTime; 
};

// message data
struct MsgTankMove {
    unsigned char tankId; 
    unsigned char x; 
    unsigned char y; 
}; 



}
#endif
