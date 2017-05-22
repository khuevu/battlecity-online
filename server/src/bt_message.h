#ifndef INCLUDED_BT_MESSAGE
#define INCLUDED_BT_MESSAGE

#include <bt_map.h>

namespace bt {
// server message
enum MessageType {
    MsgTypeConfig, // s->c, configuration information
    MsgTypeGameReady, // s->c, when two player joined
    MsgTypeRequestLevelStart, // c->s, when player request to start a level
    MsgTypeLevelMapData, // s->c, map data
    MsgTypeLevelReady, // c->s, after client has received map data
    MsgTypeLevelStart, // s->c, tell client it should start level
    MsgTypeTankCreation,
    MsgTypeTankMovement,
    MsgTypeTankAction,
    MsgTypeTankStateInfo
};

struct MsgConfig {
    unsigned char position;
};

struct MsgRequestGameStart {
    unsigned char levelNumber;
};

struct MsgMapData {
    char map[Map::GRID_SIZE][Map::GRID_SIZE];
};

struct MsgLevelStart {
    unsigned long startTime; 
};

struct MsgTankCreation {
    unsigned char tankId;
    unsigned char type;
    unsigned char direction; 
    double x; 
    double y;
    double speed;
    double health;
    double power;
};

// message data
struct MsgTankMovement {
    unsigned char tankId; 
    unsigned char direction;
    unsigned char moving;
    double x; 
    double y; 
};

struct MsgTankAction {
    unsigned char tankId;
    unsigned char action;
};



}
#endif
