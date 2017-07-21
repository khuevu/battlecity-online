#include <bt_consentbox.h>

namespace bt {

namespace {
    const int NUM_PLAYER = 2;
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
    Vote vote;
    std::cout << "vote message length " << msgLength << std::endl;

    vote.msgId = msgId;
    vote.msgLength = msgLength;
    vote.msgContent = new char[msgLength];
    std::cout << "Allocate new content at " << static_cast<void*>(vote.msgContent) << std::endl;
    std::copy(msg, msg + msgLength, vote.msgContent);

    auto res = d_votes.find(vote);
    if (res == d_votes.end())
    {
        std::cout << "New vote " << std::endl;
        res = d_votes.emplace(vote, std::vector<int>()).first;
    }

    std::cout << "vote message length after insert " << res->first.msgLength << std::endl;
    std::cout << "vote message content addr after insert " << static_cast<void*>(res->first.msgContent) << std::endl;
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
            std::cout << "consent vote message length " << consentVote.msgLength << std::endl;
            std::copy(consentVote.msgContent, consentVote.msgContent + consentVote.msgLength, msg);
            // clean up vote content
            std::cout << "To delete msg at " << static_cast<void*>(consentVote.msgContent) << std::endl;
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