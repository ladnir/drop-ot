#pragma once
// © 2016 Peter Rindal.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <cryptoTools/Common/Defines.h>
#include <cryptoTools/Common/MatrixView.h>
#include <cassert>
namespace osuCrypto {




    void eklundh_transpose128(block* inOut);
    inline void eklundh_transpose128(std::array<block, 128>& inOut) { eklundh_transpose128(inOut.data()); }

#ifdef OC_ENABLE_AVX2
    void avx_transpose128(block* inOut);
#endif
#ifdef OC_ENABLE_SSE2
    void sse_transpose128(block* inOut);
    inline void sse_transpose128(std::array<block, 128>& inOut) { sse_transpose128(inOut.data()); };
#endif
    void transpose(const MatrixView<block>& in, const MatrixView<block>& out);
    void transpose(const MatrixView<u8>& in, const MatrixView<u8>& out);


    // Input must be given the alignment of an AlignedBlockArray, i.e. 32 bytes with AVX or 16 bytes
    // without.
    inline void transpose128(block* inOut)
    {
#if defined(OC_ENABLE_AVX2)
        assert((u64)inOut % 32 == 0);
        avx_transpose128(inOut);
#elif defined(OC_ENABLE_SSE2)
        assert((u64)inOut % 16 == 0);
        sse_transpose128(inOut);
#else
        eklundh_transpose128(inOut);
#endif
    }

    inline void transpose128(std::array<block, 128>& inOut) { transpose128(inOut.data()); };
}
