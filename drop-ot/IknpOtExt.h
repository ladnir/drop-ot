#pragma once
#include "drop-ot/Defines.h"
#include <array>
#include "cryptoTools/Crypto/RandomOracle.h"

namespace dropOt {


    const u64 commStepSize(512);
    const u64 superBlkSize(8);
    const u64 gOtExtBaseOtCount(128);

    class IknpOtExtSender
    {
    public:

        //std::array<PRNG, gOtExtBaseOtCount> mGens;
        u64 mPrngIdx = 0;
        oc::MultiKeyAES<gOtExtBaseOtCount> mGens;
        BitVector mBaseChoiceBits;

        std::vector<block> mMessages;


        IknpOtExtSender() = default;
        IknpOtExtSender(const IknpOtExtSender&) = delete;
        IknpOtExtSender(IknpOtExtSender&&) = default;

        IknpOtExtSender(
            span<block> baseRecvOts,
            const BitVector& choices)
        {
            setUniformBaseOts(baseRecvOts, choices);
        }

        void operator=(IknpOtExtSender&& v)
        {
            mGens = std::move(v.mGens);
            mBaseChoiceBits = std::move(v.mBaseChoiceBits);
        }

        // return true if this instance has valid base OTs. 
        bool hasBaseOts() const
        {
            return mBaseChoiceBits.size() > 0;
        }

        // Returns a independent instance of this extender which can 
        // be executed concurrently. The base OTs are derived from the
        // original base OTs.
        //IknpOtExtSender split();

        // Sets the base OTs which must be peformed before calling split or send.
        // See frontend/main.cpp for an example. 
        void setUniformBaseOts(
            span<block> baseRecvOts,
            const BitVector& choices);


        // The first round of the OT-sender protocol. This will receive a 
        // message and send a message. 
        // This function can return early with a code::suspend error in which
        // case the caller should perform io and call the function again.
        // @messages, output: the random messages that will be returned.
        // @prng, input: the randomness source.
        // @chl, input: the io buffer/socket.
        void sendRoundOne_r(
            span<std::array<block, 2>> messages,
            PRNG& prng,
            span<u8>& ioBuffer);

        // The second round of the OT-sender protocol. This will receive a 
        // message. 
        // This function can return early with a code::suspend error in which
        // case the caller should perform io and call the function again.
        // @prng, input: the randomness source.
        // @chl, input: the io buffer/socket.
        //error_code sendRoundTwo_r(
        //    PRNG& prng,
        //    IOHandler& chl);
    };


    class KosOtExtReceiver
    {
    public:


        bool mHasBase = false;
        oc::AlignedArray<oc::MultiKeyAES<gOtExtBaseOtCount>, 2> mGens;
        u64 mPrngIdx = 0;

        KosOtExtReceiver() = default;
        KosOtExtReceiver(const KosOtExtReceiver&) = delete;
        KosOtExtReceiver(KosOtExtReceiver&&) = default;
        KosOtExtReceiver(span<std::array<block, 2>> baseSendOts);

        void operator=(KosOtExtReceiver&& v)
        {
            mHasBase = std::move(v.mHasBase);
            mGens = std::move(v.mGens);
            v.mHasBase = false;
        }

        // returns whether the base OTs have been set. They must be set before
        // split or receive is called.
        bool hasBaseOts() const
        {
            return mHasBase;
        }

        // sets the base OTs.
        void setUniformBaseOts(span<std::array<block, 2>> baseSendOts);

        // returns an independent instance of this extender which can securely be
        // used concurrently to this current one. The base OTs for the new instance 
        // are derived from the orginial base OTs.
        //KosOtExtReceiver splitBase();

        // The first round of the OT-receiver protocol. This will send a message. 
        // This function can return early with a code::suspend error in which
        // case the caller should perform io and call the function again.
        // @choices, input: the choice bits that the receiver choose.
        // @messages, output: the random messages that will be returned.
        // @prng, input: the randomness source.
        // @chl, input: the io buffer/socket.
        void receiveRoundOne_s(
            const BitVector& choices,
            span<block> messages,
            PRNG& prng,
            std::vector<u8>& ioBuffer);

    };


    namespace tests
    {

        void OtExt_Iknp_Buff_test();
    }
}

