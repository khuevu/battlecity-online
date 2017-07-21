#include <bt_consentbox.h>

namespace bt {

namespace {
    const int NUM_PLAYER = 2;
}

ConsentBox::Vote::Vote(unsigned char id, const char *content, size_t length)
        : msgId(id), msgContent(content), msgLength(length)
{

}

ConsentBox::~ConsentBox() {
    std::cout << "Calling consentbox destructor " << std::endl;
    for (auto& voteAndPlayers : d_votes)
    {
        auto& vote = voteAndPlayers.first;
        // clean up vote content
        std::cout << "Delete vote in consent box" << std::endl;
        delete[] vote.msgContent;
    }
}


void ConsentBox::vote(int playerId, unsigned char msgId, const char *msg, size_t msgLength) {
    // store vote content
    Vote vote(msgId, msg, msgLength);


    auto res = d_votes.find(vote);
    if (res == d_votes.end())
    {
        // make a copy of vote and store it
        char* msgCopy = new char[msgLength];
        std::copy(msg, msg + msgLength, msgCopy);
        res = d_votes.emplace(Vote(msgId, msgCopy, msgLength), std::vector<int>()).first;
    }

    res->second.push_back(playerId);
}


unsigned char ConsentBox::getNextConsensus(char *msg) {
    // go through the message and compare
    for (Votes::iterator it = d_votes.begin(); it != d_votes.end();)
    {
        if (it->second.size() == NUM_PLAYER)
        {
            Vote consentVote = it->first;
            it = d_votes.erase(it);
//            std::cout << "consent vote message length " << consentVote.msgLength << std::endl;
            std::copy(consentVote.msgContent, consentVote.msgContent + consentVote.msgLength, msg);
            // clean up vote content
            delete[] consentVote.msgContent;

            return consentVote.msgId;
        }
        else
        {
            ++it;
        }
    }
    return -1;
}


}