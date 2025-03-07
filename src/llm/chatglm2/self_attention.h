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

#include "llyn/llyn.h"
#include "llm/chatglm2/chatglm2_config.h"

namespace libllm {
namespace chatglm2 {

class SelfAttention : public llyn::nn::Module {
 public:
  static std::unique_ptr<SelfAttention> create(const llyn::Context &ctx, ChatGLM2Config config);

  // initialize the module from context
  void initParameters(const llyn::StateMap &stateDict) override;

  llyn::Tensor forward(llyn::StateMap &past, llyn::Tensor input, llyn::Tensor roPE) const;

 private:
  std::unique_ptr<llyn::nn::Linear> _qkvProj;
  llyn::Tensor _denseWeight;
  llyn::Context _ctx;

  int _kvProjDim;
  int _qProjDim;
  int _hiddenSizePerHead;

  std::string _namePastK;
  std::string _namePastV;
  std::string _namePastLength;

  SelfAttention() = default;

  int getCtxLength(llyn::StateMap *past) const;
};

}  // namespace chatglm2
}  // namespace libllm
