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

#include "llm/chatglm2/chatglm2_config.h"


namespace libllm {
namespace chatglm2 {

constexpr char ChatGLM2Config::kSection[];

ChatGLM2Config::ChatGLM2Config()
    : hiddenSize(0),
      vocabSize(0),
      kvChannels(0),
      seqLength(0),
      hiddenSizePerAttentionHead(0),
      multiQueryGroupNum(0),
      normEps(0.0f),
      numLayers(0),
      symbolGMask(0),
      symbolSOP(0),
      symbolEOS(0) {}

ChatGLM2Config ChatGLM2Config::loadConfig(const ly::IniConfig &ini) {
  const ly::IniSection &section = ini.getSection(kSection);

  ChatGLM2Config config;
  config.hiddenSize = section.getInt("hidden_size");
  config.vocabSize = section.getInt("vocab_size");
  config.kvChannels = section.getInt("kv_channels");
  config.seqLength = section.getInt("seq_length");
  config.hiddenSizePerAttentionHead = section.getInt("hidden_size_per_attention_head");
  config.multiQueryGroupNum = section.getInt("multi_query_group_num");
  config.normEps = section.getFloat("norm_eps");
  config.ffnHiddenSize = section.getFloat("ffn_hidden_size");
  config.numLayers = section.getFloat("num_layers");
  config.symbolGMask = section.getFloat("symbol_gmask");
  config.symbolSOP = section.getFloat("symbol_sop");
  config.symbolEOS = section.getFloat("symbol_eos");

  return config;
}

}  // namespace chatglm2
}  // namespace libllm
