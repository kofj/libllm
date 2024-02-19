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

#include "libllm/qwen.h"

namespace libllm {
namespace qwen {

QwenModelForGeneration::QwenModelForGeneration() : _imStartId(-1), _imEndId(-1) {}

std::shared_ptr<QwenModelForGeneration> QwenModelForGeneration::fromConfig(
    const Context &ctx,
    const lut::IniConfig &config) {
  std::shared_ptr<QwenModelForGeneration> model{new QwenModelForGeneration()};
  model->init(ctx, config);

  std::string modelType = config.getSection(ModelSection).getString(ModelTypeField);
  const lut::IniSection &qwenSection = config.getSection(modelType);

  model->_imStartId = qwenSection.getInt("im_start_token_id");
  model->_imEndId = qwenSection.getInt("im_end_token_id");

  return model;
}

bool QwenModelForGeneration::isStopToken(int tokenId) const {
  if (llama::LlamaModelForGeneration::isStopToken(tokenId) ||
      tokenId == _imEndId ||
      tokenId == _imStartId) {
    return true;
  } else {
    return false;
  }
}

}  // namespace qwen
}  // namespace libllm
