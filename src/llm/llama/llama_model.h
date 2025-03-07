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

#include <memory>
#include "llyn/nn/module.h"
#include "llyn/nn/embedding.h"
#include "llm/common/model_for_generation.h"
#include "llm/llama/decoder_layer.h"
#include "llm/llama/llama_config.h"

namespace libllm {
namespace llama {

class LlamaModel : public llyn::nn::Module {
 public:
  static constexpr char Llama[] = "llama";
  static constexpr char RoPE[] = "rope";

  static std::shared_ptr<LlamaModel> create(const llyn::Context &ctx, LlamaConfig config);
  void initParameters(const llyn::StateMap &stateDict) override;

  llyn::Tensor forward(llyn::StateMap &past, llyn::Tensor input) const;
  llyn::Tensor forwardHidden(llyn::Tensor hidden) const;

 private:
  llyn::Context _ctx;
  LlamaConfig _config;
  std::shared_ptr<llyn::nn::Embedding> _embedding;
  std::shared_ptr<llyn::nn::RMSNorm> _norm;
  std::vector<std::shared_ptr<DecodeLayer>> _layers;
  llyn::Tensor _wOutput;

  LlamaModel() = default;
};

}  // namespace llama
}  // namespace libllm
