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

#include "llyn/context.h"

#include "lyutil/log.h"

namespace llyn {

Context::Context() {}

Context Context::withName(const std::string &name) const {
  CHECK(!name.empty());
  Context ctx;
  ctx._device = _device;
  ctx._ns = this->name(name);

  return ctx;
}

std::string Context::joinName(const std::string &left, const std::string &right) {
  std::string join = left;
  join += ".";
  join += right;
  return join;
}

std::string Context::name(const std::string &name) const {
  std::string ns = _ns;

  if (ns.empty()) {
    ns = name;
  } else {
    ns = joinName(ns, name);
  }

  return ns;
}

}  // namespace llyn
