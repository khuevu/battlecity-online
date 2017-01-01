#include <bt_process.h>

#include <bt_gamecontainer.h>

#include <bt_message.h>
 
namespace bt {

Process::Process(GameContainer& gameContainer) : d_container(gameContainer) {}

bool Process::loop() {
    // read all messages from player buffer 
    for (std::vector<Player>::iterator it = d_container.d_players.begin(); it != d_container.d_players.end(); ++it) {

        unsigned char msgId; 
        char msg[256]; 

        while (it->readNextMsgReceived(&msgId, msg) == Player::SUCCESS) {
            process(it->id(), msgId, msg);   
        }
    }

    // return status
    return true; 
}

void Process::process(int playerId, unsigned char msgId, const char* msg) {
    switch(msgId) {
        //case TankFire:
            ////TODO
            //break;

        //case TankMove:
            ////TODO: 
            //break;

        //case TankExplode:
            ////TODO
            //break;

        //case TankStateInfo:
            ////TODO:
            //break;

        default:
            break;

    }
}

}
