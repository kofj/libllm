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

#include "llm/chatglm2/chatglm2_model.h"

#include "llyn/llyn.h"
#include "lyutil/error.h"
#include "lyutil/strings.h"
#include "lytok/lytok.h"

using llyn::Context;
using llyn::Tensor;
using llyn::StateMap;
using llyn::nn::Embedding;
using llyn::nn::RMSNorm;

namespace F = llyn::functional;

namespace libllm {
namespace chatglm2 {

constexpr char ChatGLM2Model::ChatGlm2[];
constexpr char ChatGLM2Model::Embd[];
constexpr char ChatGLM2Model::RoPE[];
constexpr char ChatGLM2Model::Block[];
constexpr char ChatGLM2Model::FinalNorm[];
constexpr char ChatGLM2Model::OutputWeight[];

ChatGLM2Model::ChatGLM2Model() {}

std::unique_ptr<ChatGLM2Model> ChatGLM2Model::create(const Context &rootCtx,
                                                     ChatGLM2Config config) {
  std::unique_ptr<ChatGLM2Model> model{new ChatGLM2Model()};
  Context ctx = rootCtx.withName(ChatGlm2);
  model->_config = config;
  model->_ctx = ctx;
  model->_embedding = Embedding::create(ctx.withName(Embd), config.hiddenSize, config.vocabSize);
  model->_finalNorm = RMSNorm::create(ctx.withName(FinalNorm), config.hiddenSize, config.normEps);
  for (int i = 0; i < config.numLayers; ++i) {
    model->_blocks.emplace_back(
        GLMBlock::create(ctx.withName(ly::sprintf("%s%d", Block, i)), config));
  }

  if (config.kvChannels % 4 != 0) {
    throw ly::AbortedError("invalid kv_channels");
  }

  return model;
}

void ChatGLM2Model::initParameters(const StateMap &stateDict) {
  _embedding->initParameters(stateDict);
  _finalNorm->initParameters(stateDict);

  _rope = stateDict.getTensor(_ctx.name(RoPE));
  _rope.throwIfInvalidShape({_config.seqLength, _config.kvChannels / 4, 2});
  _rope = _rope.view({_config.seqLength, 1, _config.kvChannels / 2});

  _output = stateDict.getTensor(_ctx.name(OutputWeight));
  _output.throwIfInvalidShape({_config.vocabSize, _config.hiddenSize});

  for (int i = 0; i < _config.numLayers; ++i) {
    _blocks[i]->initParameters(stateDict);
  }
}

llyn::Tensor ChatGLM2Model::forwardHidden(llyn::Tensor hiddenState) const {
  return F::matmul(hiddenState, _output.transpose(0, 1));
}

Tensor ChatGLM2Model::forward(StateMap &past, Tensor input) const {
  Tensor x = _embedding->forward(input);
  for (int i = 0; i < _config.numLayers; ++i) {
    x = _blocks[i]->forward(past, x, _rope);
  }
  x = _finalNorm->forward(x);

  return x;
}

}  // namespace chatglm2
}
