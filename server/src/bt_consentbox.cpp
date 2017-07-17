#include <bt_consentbox.h>

namespace bt {

namespace {
    const int NUM_PLAYER = 2;
}


ConsentBox::~ConsentBox() {
    for (auto& voteAndPlayers : d_votes)
    {
        auto& vote = voteAndPlayers.first;
        // clean up vote content
        delete[] vote.msgContent;
    }
}


void ConsentBox::vote(int playerId, unsigned char msgId, const char *msg, size_t msgLength) {
    // store vote content
    Vote vote;
    vote.msgLength = msgLength;
    vote.msgContent = new char[msgLength];
    std::copy(msg, msg + msgLength, vote.msgContent);

    auto res = d_votes.find(vote);
    if (res != d_votes.end())
    {
        res->second.push_back(playerId);
    }
}


unsigned char ConsentBox::getNextConsensus(char *msg) {
    // go through the message and compare
    Vote consentVote;

    for (Votes::iterator it = d_votes.begin(); it != d_votes.end(); ++it)
    {
        if (it->second.size() == NUM_PLAYER)
        {
            consentVote = d_votes.erase(it)->first;
            break;
        }
    }

    std::copy(consentVote.msgContent, consentVote.msgContent + consentVote.msgLength, msg);
    // clean up vote content
    delete[] consentVote.msgContent;

    return consentVote.msgId;
}


}