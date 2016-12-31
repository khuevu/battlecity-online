#include <bt_process.h>

#include <bt_level.h>

#include <bt_message.h>
 
namespace bt {

Process::Process(Level& level) : d_level(level) {}

void Process::update() {
    // read all messages from player buffer 
    for (std::vector<Player>::iterator it = d_level.d_players.begin(); it != d_level.d_players.end(); ++it) {

        unsigned char msgId; 
        char msg[256]; 

        while (it->readNextMsgReceived(&msgId, msg) == Player::SUCCESS) {
            process(it->id(), msgId, msg);   
        }
    }
}

void Process::process(int playerId, unsigned char msgId, const char* msg) {
    switch(msgId) {
        case TankFire:
            //TODO
            break;

        case TankMove:
            //TODO: 
            break;

        case TankExplode:
            //TODO
            break;

        case TankStateInfo:
            //TODO:
            break;

        default:
            break;

    }
}

}
