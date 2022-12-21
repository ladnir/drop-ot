#include "drop-ot/UnitTests.h"
#include "cryptoTools/Network/IOService.h"
#include "cryptoTools/Network/Session.h"
#include "cryptoTools/Common/Timer.h"
#include "cryptoTools/Common/CLP.h"
#include "cryptoTools/Common/Log.h"
#include "cryptoTools/Common/Matrix.h"
#include <cassert>
#include "drop-ot/IknpOtExt.h"
#include "drop-ot/MasnyRindal.h"
#include <fstream>

using namespace dropOt;

void tutorial(oc::CLP& cmd);
void networked(oc::CLP& cmd);
void help(oc::CLP& cmd);

int main(int argc, char** argv)
{
    oc::CLP cmd(argc, argv);

    if (cmd.isSet("u"))
        unitTests.runIf(cmd);
    else if (cmd.isSet("tut"))
        tutorial(cmd);
    else
        help(cmd);

    return 0;
}

void help(oc::CLP& cmd)
{
    std::cout << "-u to run unit tests" << std::endl;
    std::cout << "-tut to run tutorial" << std::endl;

}

std::vector<u8> sender_Setup1(std::string stateDir)
{
    MasnyRindal mr;
    PRNG prng(oc::sysRandomSeed());


    std::vector<u8> ioBuffer;
    oc::BitVector choices(128);
    choices.randomize(prng);
    mr.receiveRoundOne(choices, prng, ioBuffer);

    std::ofstream out;
    out.open(stateDir + "/senderState.bin", std::ios::trunc | std::ios::binary | std::ios::out);
    mr.serialize(out);

    return ioBuffer;

}


void sender_Setup2(
    std::string stateDir,
    span<u8> ioBuffer)
{
    MasnyRindal mr;
    PRNG prng(oc::sysRandomSeed());

    std::ifstream in;
    in.open(stateDir + "/senderState.bin", std::ios::binary | std::ios::in);
    mr.deserialize(in);
    in.close();

    auto choices = mr.mChoices;
    std::remove((stateDir + "/senderState.bin").c_str());

    std::vector<block> keys(choices.size());
    mr.receiveRoundTwo(keys, prng, ioBuffer);

    IknpOtExtSender sender;
    sender.setUniformBaseOts(keys, choices);
    std::ofstream out;
    out.open(stateDir + "/senderState.bin", std::ios::trunc | std::ios::binary | std::ios::out);
    sender.serialize(out);
}




std::vector<u8> recver_Setup(
    std::string stateDir,
    span<u8> inMessage)
{
    MasnyRindal mr;
    PRNG prng(oc::sysRandomSeed());

    std::vector<std::array<block, 2>> keys(128);

    std::vector<u8> ioBuffer;
    mr.sendRoundOne(128, prng, ioBuffer);
    mr.sendRoundTwo(keys, prng, inMessage);

    IknpOtExtReceiver recver;
    recver.setUniformBaseOts(keys);

    std::ofstream out;
    out.open(stateDir + "/recverState.bin", std::ios::trunc | std::ios::binary | std::ios::out);
    recver.serialize(out);

    return ioBuffer;
}

std::vector<u8> recver_generateKeys(std::string stateDir, std::vector<block>& recverKeys, BitVector& choices)
{
    if (recverKeys.size() != choices.size())
        throw RTE_LOC;

    std::ifstream in;
    in.open(stateDir + "/recverState.bin", std::ios::binary | std::ios::in);

    IknpOtExtReceiver recver;
    recver.deserialize(in);



    PRNG prng(oc::sysRandomSeed());
    std::vector<u8> ioBuffer;
    recver.receiveRoundOne_s(choices, recverKeys, prng, ioBuffer);

    std::ofstream out;
    out.open(stateDir + "/recverState.bin", std::ios::trunc | std::ios::binary | std::ios::out);
    recver.serialize(out);

    return ioBuffer;
}


void sender_generateKeys(std::string stateDir, span<u8>inMessages, std::vector<std::array<block, 2>>& senderKeys)
{
    std::ifstream in;
    in.open(stateDir + "/senderState.bin", std::ios::binary | std::ios::in);

    IknpOtExtSender sender;
    sender.deserialize(in);

    PRNG prng(oc::sysRandomSeed());
    sender.sendRoundOne_r(senderKeys, prng, inMessages);

    std::ofstream out;
    out.open(stateDir + "/senderState.bin", std::ios::trunc | std::ios::binary | std::ios::out);
    sender.serialize(out);
}

void tutorial(oc::CLP& cmd)
{
    auto stateDir = cmd.getOr<std::string>("stateDir", ".");
    auto n = cmd.getOr<u64>("n", 100);
    auto trials = cmd.getOr<u64>("t", 2);


    //////////////////////////////
    // setup

    auto buffer = sender_Setup1(stateDir);

    buffer = recver_Setup(stateDir, span<u8>(buffer));

    sender_Setup2(stateDir, span<u8>(buffer));


    //////////////////////////////
    // main

    for (u64 t = 0; t < trials; ++t)
    {
        // these will be input in the real code...
        oc::BitVector choices(n);
        for (u64 i = 0; i < choices.size(); ++i)
            choices[i] = i % 2;

        // output keys
        std::vector<block> recverKeys(n);

        buffer = recver_generateKeys(stateDir, recverKeys, choices);

        std::vector<std::array<block, 2>> senderKeys(n);
        sender_generateKeys(stateDir, span<u8>(buffer), senderKeys);


        std::cout << "generated OT keys" << std::endl;
        for (u64 i = 0; i < n; ++i)
        {
            std::cout << "sender {" << senderKeys[i][0] << ", " << senderKeys[i][1] << "}, recver{" << recverKeys[i] << ", " << choices[i] << "}" << std::endl;
        }
    }
}

