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

#include <vector>
#include "lyutil/span.h"
#include "llyn/cpu/internal.h"

namespace llyn {
namespace cpu {

template<typename T>
class SubtensorList : public ly::NonCopyable {
 public:
  SubtensorList(ly::Span<const Shape> shape, std::vector<T *> &&l)
      : _shape(shape),
        _list(std::move(l)) {}

  SubtensorList(SubtensorList<T> &&l)
      : _shape(l._shape),
        _list(std::move(l._list)) {}

  ly::Span<const Shape> getShape() const { return _shape; }
  int getShape(int d) const { return _shape[d].shape; }
  int getSize() const { return _list.size(); }
  ly::Span<T *const> getDataPtrList() const { return ly::makeConstSpan(_list); }

  Subtensor<T> getSubtensor(int index) const {
    Subtensor<T> tensor;
    tensor.data = _list[index];
    tensor.shape = _shape;

    return tensor;
  }

 private:
  ly::Span<const Shape> _shape;
  std::vector<T *> _list;
};

template<typename T>
void getSubtensors(Subtensor<T> tensor, int subtensorDim, std::vector<T *> *l) {
  CHECK(tensor.rank() >= subtensorDim);

  if (tensor.rank() == subtensorDim) {
    l->push_back(tensor.data);
  } else {
    for (int i = 0; i < tensor.dimension(0); ++i) {
      getSubtensors(tensor.subtensor(i), subtensorDim, l);
    }
  }
}

template<typename T>
bool isShapeMatch(Subtensor<T> A, Subtensor<T> B) {
  if (A.rank() != B.rank())
    return false;

  for (int d = 0; d < A.rank(); ++d) {
    if (A.dimension(d) != B.dimension(d))
      return false;
  }

  return true;
}

template<typename T>
bool isShapeMatchBroadcastB(Subtensor<T> A, Subtensor<T> B) {
  if (A.rank() < B.rank())
    return false;
  
  if (A.rank() > B.rank()) {
    A.shape = A.shape.subspan(A.rank() - B.rank());
  }

  return isShapeMatch(A, B);
}

template<typename T>
SubtensorList<T> getVectorList(Subtensor<T> tensor) {
  std::vector<T *> l;
  getSubtensors(tensor, 1, &l);

  ly::Span<const Shape> vecShape = tensor.shape.subspan(tensor.rank() - 1);
  return SubtensorList<T>(vecShape, std::move(l));
}

template<typename T>
SubtensorList<T> getMatrixList(Subtensor<T> tensor) {
  std::vector<T *> l;
  getSubtensors(tensor, 2, &l);

  ly::Span<const Shape> matrixShape = tensor.shape.subspan(tensor.rank() - 2);
  return SubtensorList<T>(matrixShape, std::move(l));
}

template<typename T>
SubtensorList<T> getTensorList(Subtensor<T> tensor, int tensorDim) {
  std::vector<T *> l;
  getSubtensors(tensor, tensorDim, &l);

  ly::Span<const Shape> tensorShape = tensor.shape.subspan(tensor.rank() - tensorDim);
  return SubtensorList<T>(tensorShape, std::move(l));
}

}  // cpu
}  // flint
