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

#include <unordered_map>
#include "lyutil/reader.h"
#include "lytok/vocab.h"

namespace lytok {

// Store tne data from sentence-piece BPE model.
class BPEModel : public Vocab,
                 private ly::NonCopyable {
 public:
  // token flags.
  static constexpr int kUnknown = 1;
  static constexpr int kControl = 2;
  static constexpr int kByte = 4;
  static constexpr int kUnused = 8;

  // Read the BpeModel from file. It could not read a SPM model directly, instead, we need to
  // convert the SPM model using tokenizer_exporter.py.
  static std::unique_ptr<BPEModel> create(const std::string &filename);

  // implement interface Vocab
  int findToken(const std::string &token) const override;
  const std::string &getTokenPiece(int token_id) const override;
  const std::string &getTokenString(int token_id) const override;
  int getVocabSize() const override;
  int getUnkId() const override;

  // given the left and right token-ids, find the merged token-id and cost, return as (id, cost).
  // If no merge record found, return (Vocab::kInvalidToken, +inf)
  std::pair<int, float> findMerge(int left, int right) const;

  // token-id for space token.
  int getSpaceId() const;

  // return true if the token is either Unknown, Control, SingleByte or Unsed.
  bool isSpecialToken(int token_id) const;

  // returns flag of a token.
  int32_t getTokenFlag(int token_id) const;

  // get token-id for a single byte.
  int getByteId(int byte) const;
  bool isByteTokenAvailable() const { return _isByteTokenAvailable; }

 private:
  struct TokenInfo;

  static constexpr int16_t kMagicNumber = 0x55aa;

  std::unordered_map<std::string, const TokenInfo *> _tokenDict;
  std::vector<TokenInfo> _tokens;

  // standalone tokens for byte.
  std::array<int, 256> _byteId;
  bool _isByteTokenAvailable;

  int _unkId;
  int _spaceId;
  
  BPEModel();

  // read model from fp
  void readModel(ly::ReadableFile *fp);
  void readMagicNumber(ly::ReadableFile *fp);
  TokenInfo readRecord(ly::ReadableFile *fp);
  void initModel();
  void checkModel();
};

struct BPEModel::TokenInfo {
  int id;
  float weight;
  std::string tokenPiece;
  std::string tokenString;
  int8_t flag;

  constexpr bool isSpecialToken() const { return flag != 0; }
};

}  // namespace lytok