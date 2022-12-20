#pragma once
#include "Defines.h"

namespace dropOt
{
    // This is the Base OT protocl of Masny, Rindal 2019.
    // See https://eprint.iacr.org/2019/706.
    class MasnyRindal
    {
    public:

        enum State
        {
            RoundOne,
            RoundTwo
        };

        // The secret keys of each party.
        std::vector<Number> mSk;

        // The current state of the protocol.
        State mState = State::RoundOne;
        void resetState() { 
            mState = State::RoundOne; 
            mSk.resize(0);
        }


        // Perform round 1 of the OT-receiver
        // protocol. Will return a dropOt::code
        // which encodes {success, error, ...}.
        // @choices, input: the choice bits of the messages.
        // @prng, input: the source of randomness.
        // @chl, input: the location the io should be send/recv.
        void receiveRoundOne(
            const BitVector& choices,
            PRNG& prng,
            std::vector<u8>& ioBuffer);

        // Perform round 2 of the OT-receiver
        // protocol. Will return a dropOt::code
        // which encodes {success, suspend, error, ...}.
        // If suspend is returned the caller should 
        // perform io and call the function again.
        // @choices, input: the choice bits of the messages.
        // @messages, output: the location that the random messages will be written to.
        // @prng, input: the source of randomness.
        // @chl, input: the location the io should be send/recv.
        void receiveRoundTwo(
            const BitVector& choices,
            span<block> messages,
            PRNG& prng,
            span<u8>& ioBuffer);

        // Perform round 1 of the OT-sender
        // protocol. Will return a dropOt::code
        // which encodes {success, error, ...}.
        // @n, input: the number of OTs to perfom.
        // @prng, input: the source of randomness.
        // @chl, input: the location the io should be send/recv.
        void sendRoundOne(
            u64 n,
            PRNG& prng,
            std::vector<u8>& ioBuffer);


        // Perform round 2 of the OT-sender
        // protocol. Will return a dropOt::code
        // which encodes {success, suspend, error, ...}.
        // If suspend is returned the caller should 
        // perform io and call the function again.
        // @messages, output: the location that the random messages will be written to.
        // @prng, input: the source of randomness.
        // @chl, input: the location the io should be send/recv.
        void sendRoundTwo(
            span<std::array<block, 2>> messages,
            PRNG& prng,
            span<u8>& ioBuffer);
    };

    namespace tests {
        void Bot_MasnyRindal_Buff_test();
    }

}
