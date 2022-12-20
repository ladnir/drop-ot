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

using namespace dropOt;

void tutorial(oc::CLP& cmd);
void networked(oc::CLP& cmd);
void help(oc::CLP& cmd);

int main(int argc, char** argv)
{
    oc::CLP cmd(argc, argv);

    if (cmd.isSet("u"))
        unitTests.runIf(cmd);
    else
        help(cmd);

    return 0;
}

void help(oc::CLP& cmd)
{
    std::cout << "-u to run unit tests" << std::endl;

}

//void communicate(oc::Matrix<IOHandler>& iohs);
void tutorial(oc::CLP& cmd)
{
    //auto pc = cmd.getOr("partyCount", 3ull);
    //auto t = cmd.getOr("threshold", 2ull);

    //std::cout
    //    << "====================================\n"
    //    << "#           tutorial               #\n"
    //    << "====================================" << std::endl;


    //auto ssp = 40;
    //PRNG prng(oc::sysRandomSeed());

    //std::vector<dropOt::IknpOtExtSender> p(pc);

    //// Next we need to set up some IOHandlers
    //// which we will use for facilitate communication 
    //// between all sets of parties. Party i will
    //// using the IOHandlers:
    ////
    ////   iohs(i,0), ..., iohs(i,pc-2)
    ////
    //// to communicate. Basically, iohs(i,j) will be 
    //// linked with party j. But we skip the link 
    //// with ourselves so there are pc-1 such links...
    ////
    //// We have two ways to instantiate an IOHandler. 
    ////   1) As a local buffer which we have to manually
    ////      add and remove things from.
    ////   2) As a network socket that performs the IO
    ////      automatically for us.
    //oc::Matrix<IOHandler> iohs(pc, pc - 1);

    //// During signing we will need to specify which 
    //// of the t-1 other parties will participate in
    //// signing. Will arbitrarily choose the parties
    //// to be {0,1,...,t-1}. As such, the first t parties
    //// need to have a vector of party indexes that
    //// exclude their own. 
    //oc::Matrix<u64> partyIdxs(pc, pc - 1);

    //// Now we will loop over these data structures and 
    //// initialize them as described.
    //for (u64 i = 0; i < pc; ++i)
    //{
    //    for (u64 j = 0, k = 0; j < pc; ++j)
    //    {
    //        // we skip creating an IOHandler 
    //        // with ourselves...
    //        if (i == j)
    //            continue;

    //        // only the first t parties need this.
    //        if (k < partyIdxs.cols())
    //            partyIdxs(i, k) = j;

    //        // Here we are making an IO buffer
    //        // and we will manually have to pass
    //        // the messages between the parties.
    //        iohs(i, k) = makeIOBuffer();

    //        ++k;
    //    }

    //    // each ecdsa instance needs to be initialized.
    //    //p[i].init(pc, t, i, ssp, prng);
    //}

    //oc::Timer timer;
    //timer.setTimePoint("");

    //// first thing we need to do is have the parties
    //// interactively generate a ecdsa secret key. This
    //// is done by running a four round protocol where
    //// in each round each party sends a message to
    //// all of the other parties.
    //for (u64 roundIdx = 0; roundIdx < 4; ++roundIdx)
    //{
    //    for (u64 partyIdx = 0; partyIdx < pc; ++partyIdx)
    //    {
    //        // in each round we will call the keyGen(...)
    //        // function for each party. This party will
    //        // read messages that are stored in their
    //        // set of IO buffers, i.e. iohs[partyIdx], and
    //        // they will write the messages that should be
    //        // sent back to these IO buffers.
    //        //
    //        // When we call the function the first 3 times
    //        // it will return an error_code with a value of
    //        // code::suspend. This code means that the internal
    //        // algorithm tried to read a message from one of 
    //        // the buffers but the message that it 
    //        // was not there. As such the algorithm has been
    //        // "suspended" and is waiting for you the caller
    //        // to send the messages that were 
    //        // generated to the other parties and receive new
    //        // ones in return.
    //        //
    //        // On the four call, this function will return
    //        // the error_code code::success which indicates
    //        // that the operation has completed.
    //         
    //        auto ec = p[partyIdx].send(msg,prng,iohs[partyIdx]);

    //        if (ec == code::success)
    //        {
    //            // this will happen on the fourth round.
    //            assert(roundIdx == 3);
    //        }
    //        else if (ec == code::suspend)
    //        {
    //            // this will happen on the first three rounds.
    //            assert(roundIdx < 3);
    //        }
    //        else if (ec == code::ioError)
    //        {
    //            std::cout << "Some io error happened. Should abort and retry." << std::endl;
    //            throw std::exception();
    //        }
    //        else if (ec == code::secuirtyViolation)
    //        {
    //            std::cout << "One of the other parties was caught cheating." << std::endl;
    //            throw std::exception();
    //        }
    //        else
    //        {
    //            std::cout << "An unknown error occurred." << std::endl;
    //            throw std::exception();
    //        }
    //    }

    //    // now that one round has been performed, 
    //    // we need to communicate the messages 
    //    // between the parties. 
    //    communicate(iohs);
    //}

    //timer.setTimePoint("keyGen");


    //// Optional: Users of the code do not need to 
    //// perform the following loop. We do it here as to show 
    //// the time that it requires to generate base OTs.
    //// This is a one time operation that needs to
    //// be performed between each pair of parties.
    //// The signPreprocess protocol will automatically 
    //// perform these.
    //for (u64 roundIdx = 0; roundIdx < 2; ++roundIdx)
    //{
    //    for (u64 partyIdx = 0; partyIdx < t; ++partyIdx)
    //    {
    //        auto ec = p[partyIdx].genBaseOTs(partyIdxs[partyIdx].subspan(0,t-1), iohs[partyIdx].subspan(0,t-1));
    //        if ((roundIdx == 0 && ec != code::suspend) ||
    //            (roundIdx == 1 && ec != code::success))
    //            throw std::runtime_error(LOCATION);
    //    }

    //    communicate(iohs);
    //}

    //// do some printing.
    //timer.setTimePoint("baseOTs...");

    //// We have established a distributed secret key.
    //// Now we can sign messages. To do this we will 
    //// first need to iteratively preprocess some
    //// "correlated randomness". In particular, each 
    //// time a set of t parties wish to sign a message,
    //// they can consume a single set of "correlated 
    //// randomness" and locally generate a t-out-of-t
    //// share of the final signature.
    ////
    //// Therefore in a preprocessing phase the parties
    //// can generate many sets of these "correlated 
    //// randomnesses" and then use them on demand.
    ////
    //// Alternatively, if the parties can generate
    //// a set of "correlated randomness" only when its 
    //// actually needed but this will require the parties
    //// to interact each time they wish to sign a message.
    //// We will see both methods.

    //// One complication of running all the parties
    //// locally is that they done always finish at the 
    //// same time. We handle this by keeping track of 
    //// who is done.
    //bool allDone = false;
    //std::vector<bool> done(pc, false);

    //// This denote how many preprocesed sets of 
    //// "correlated randomness" we want to generate.
    //u64 batchSize = cmd.getOr("bs", 1ull);

    //// while not everyone is done.
    //while (allDone == false)
    //{
    //    allDone = true;
    //    for (u64 i = 0; i < t; ++i)
    //    {
    //        // if this party is not done, 
    //        // call signPreprocess(...)
    //        if (!done[i])
    //        {
    //            // Since we are running this between the
    //            // first t parties, we will only need the 
    //            // first t-1 IOHandlers.
    //            auto ioh = iohs[i].subspan(0, t - 1);
    //            auto parties = partyIdxs[i].subspan(0, t - 1);

    //            // next we call preprocess (again) and check 
    //            // the error_code to see what we should do next.
    //            auto ec = p[i].signPreprocess(
    //                parties,
    //                ioh,
    //                batchSize);

    //            // check if this party is done or has an error.
    //            if (ec == code::success)
    //                done[i] = true;
    //            else if (ec == code::suspend)
    //                allDone = false;
    //            else if (ec == code::ioError)
    //            {
    //                std::cout << "Some io error happened. Should abort and retry." << std::endl;
    //                throw std::exception();
    //            }
    //            else if (ec == code::secuirtyViolation)
    //            {
    //                std::cout << "One of the other parties was caught cheating." << std::endl;
    //                throw std::exception();
    //            }
    //            else
    //            {
    //                std::cout << "An unknown error occured." << std::endl;
    //                throw std::exception();
    //            }
    //        }
    //    }

    //    // perform the communication for this round of the protocol.
    //    communicate(iohs);
    //}

    //timer.setTimePoint("preprocess");

    //// Now we can sign an actual message. 
    //// Say we have the following message.
    //std::string msg_ = "peter rindal";

    //// Then we will need to construct a span<u8>
    //// that points at the message data. A span<u8>
    //// is nothing more than a wrapper around a
    //// unsigned char* pointer and a size and encodes that 
    //// the pointer points at the specified number
    //// of bytes.
    //span<const u8> msg((u8*)msg_.data(), msg_.size());

    //// Next, each party can non-interactively generate
    //// a share of the final sigature.
    //std::vector<EcdsaSig> sigma(t);
    //for (u64 i = 0; i < t; ++i)
    //{
    //    auto parties = partyIdxs[i].subspan(0, t - 1);
    //    if (p[i].preprocessedCount(parties) == 0)
    //    {
    //        // We must not call sign if there are
    //        // zero sets of preprocessed correlated 
    //        // randomness.
    //        throw std::exception();
    //    }

    //    sigma[i] = p[i].sign(msg, parties);
    //}

    //// given these share we need to add them
    //// together. This will give use the final 
    //// signature. 
    //auto sig = sigma[0];
    //for (u64 i = 1; i < t; ++i)
    //{
    //    sig += sigma[i];
    //}

    //// It is important to verify the final signature
    //// to ensure that it is correct.
    //if (p[0].verify(msg, sig) != code::success)
    //    throw std::exception();

    //timer.setTimePoint("sign");

    //// Alternatively we can call the iterative version 
    //// of sign.
    //allDone = false;
    //done = std::vector<bool>(t, false);

    //// while not everyone is done.
    //while (allDone == false)
    //{
    //    allDone = true;
    //    for (u64 i = 0; i < t; ++i)
    //    {
    //        if (!done[i])
    //        {
    //            // this perform the preprocessing and signing.
    //            auto ec = p[i].sign(
    //                msg,
    //                partyIdxs[i].subspan(0, t - 1), 
    //                iohs[i].subspan(0, t - 1),
    //                sigma[i]);

    //            // check if this party is done or has an error.
    //            if (ec == code::success)
    //                done[i] = true;
    //            else if (ec == code::suspend)
    //                allDone = false;
    //            else
    //                throw std::exception();
    //        }
    //    }
    //    communicate(iohs);
    //}

    //// We can then again aggregate and verify
    //// the signature.
    //sig = sigma[0];
    //for (u64 i = 1; i < t; ++i)
    //    sig += sigma[i];
    //if (p[0].verify(msg, sig) != code::success)
    //    throw std::exception();


    //timer.setTimePoint("preprocess & sign");
    //
    //std::cout << timer << std::endl;
}


