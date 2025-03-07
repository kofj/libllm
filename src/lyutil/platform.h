// The MIT License (MIT)
//
// Copyright (c) 2023 Xiaoyang Chen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// OS ot platform dependent functions.
#pragma once

#include <stdint.h>

#ifdef __APPLE__
#define LY_PLATFORM_APPLE
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define LY_PLATFORM_LINUX
#elif defined(WIN32) || defined(__WIN32__) || defined(_MSC_VER) || \
      defined(_WIN32) || defined(__MINGW32__)
#define LY_PLATFORM_WINDOWS
#else
#error unknown platform
#endif

namespace ly {

bool isAvx512Available();
bool isAvx2Available();
void *alloc32ByteAlignedMem(int64_t nbytes);
void free32ByteAlignedMem(void *);
const char *getPathDelim();

} // namespace ly
