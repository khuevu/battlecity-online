#include <gtest/gtest.h>
#include <bt_consentbox.h>
#include <bt_message.h>
#include <bt_tank.h>


using namespace bt;


TEST(ConsentBox, TwoConsentVotes_InOrder)
{

    ConsentBox consentBox;
    MsgTankAction tankAction;
    tankAction.action = Tank::EXPLODE;
    tankAction.tankId = 15;
    consentBox.vote(0, MsgTypeTankAction, (const char*) &tankAction, sizeof(tankAction));

    MsgTankAction tankAction2;
    tankAction2.action = Tank::EXPLODE;
    tankAction2.tankId = 15;
    consentBox.vote(1, MsgTypeTankAction, (const char*) &tankAction2, sizeof(tankAction2));

    char msg[1024];
    auto msgId = consentBox.getNextConsensus(msg);

    EXPECT_EQ(MsgTypeTankAction, msgId);
    EXPECT_EQ(tankAction, *(MsgTankAction*) msg);
    EXPECT_EQ(tankAction2, *(MsgTankAction*) msg);

}


TEST(ConsentBox, TwoConsentVotes_OutOfOrder)
{

    ConsentBox consentBox;

    MsgTankAction tankAction;
    tankAction.action = Tank::EXPLODE;
    tankAction.tankId = 15;
    consentBox.vote(0, MsgTypeTankAction, (const char*) &tankAction, sizeof(tankAction));

    MsgTankAction differentAction;
    differentAction.action = Tank::ITEM;
    differentAction.tankId = 20;
    consentBox.vote(1, MsgTypeTankAction, (const char*) &differentAction, sizeof(differentAction));

    MsgTankAction tankAction2;
    tankAction2.action = Tank::EXPLODE;
    tankAction2.tankId = 15;
    consentBox.vote(1, MsgTypeTankAction, (const char*) &tankAction2, sizeof(tankAction2));

    char msg[1024];
    auto msgId = consentBox.getNextConsensus(msg);

    EXPECT_EQ(MsgTypeTankAction, msgId);
    EXPECT_EQ(tankAction, *(MsgTankAction*) msg);
    EXPECT_EQ(tankAction2, *(MsgTankAction*) msg);
    EXPECT_FALSE(differentAction == *(MsgTankAction*) msg);
}