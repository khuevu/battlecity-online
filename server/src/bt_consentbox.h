#ifndef INCLUDED_BT_CONSENTBOX
#define INCLUDED_BT_CONSENTBOX

#include <bt_message.h>
#include <utility>
#include <map>
#include <vector>
#include <algorithm>
#include <array>


namespace bt {

/**
 * @brief: ConsentBox: stores messages from clients as votes on game events
 * When a message have all votes from clients, it will be pushed out by the box
 */
class ConsentBox {

public:

    ~ConsentBox();

    void vote(int playerId, unsigned char msgId, const char* msg, size_t msgLength);

    unsigned char getNextConsensus(char msg[]);

private:

//    using Vote = std::pair<unsigned char, const char*>;
    struct Vote {
        unsigned char msgId;
        char* msgContent;
        size_t msgLength;
    };

    struct VoteComparator {
        bool operator() (const Vote& lv, const Vote& rv) const
        {
            if (lv.msgId != rv.msgId)
            {
                std::cout << "msgid or msglength doesn't match" << std::endl;
                return lv.msgId < rv.msgId;
            }
            else if (lv.msgLength != rv.msgLength)
            {
                return lv.msgLength < rv.msgLength;
            }
            else if (!std::equal(lv.msgContent, lv.msgContent + lv.msgLength, rv.msgContent))
            {
                return lv.msgContent < rv.msgContent; // we don't really care about the inequality order here
            }
            else
            {
                return false;
            }
        }
    };

    using Votes = std::map<Vote, std::vector<int>, VoteComparator>;
    Votes d_votes;


};


}
#endif //INCLUDED_BT_CONSENTBOX
