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

#pragma once

#include <stdint.h>
#include "lymath/common.h"

namespace lymath {

struct SGEMVArgs {
  typedef float VecType;

  bool transA;
  int M;
  int N;
  const float *A;
  int lda;
  const float *x;
  int incX;
  float *y;
  int incY;
};

struct QGEMVInt4AArgs {
  typedef float VecType;

  bool transA;
  int M;
  int N;
  const Q4x2 *A;
  const Fp16 *scaleA;
  const float *x;
  int incX;
  float *y;
  int incY;
};

struct Q4GemvArgs {
  typedef float VecType;

  bool transA;
  int M;
  int N;
  PCQ4x2 A;
  PCFp16 scaleA;
  PCInt8 zeroPointA;
  PCFp32 x;
  int incX;
  PFp32 y;
  int incY;
};

struct QGemmNQNInt4AArgs {
  bool transA;
  bool transB;
  int M;
  int N;
  int K;
  PCFp32 A;
  int lda;
  PCQ4x2 B;
  PCFp16 scaleB;
  PFp32 C;
  int ldc;
};

struct Q4GemmArgs {
  bool transA;
  bool transB;
  int M;
  int N;
  int K;
  PCFp32 A;
  int lda;
  PCQ4x2 B;
  PCFp16 scaleB;
  PCInt8 zeroPointB;
  PFp32 C;
  int ldc;
};

}  // namespace lymath
