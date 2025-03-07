# The MIT License (MIT)
#
# Copyright (c) 2023 Xiaoyang Chen
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import codecs
from . import interop
from .interop import AutoPtr, LL_TRUE

class Completion:
    """A completion task that given a llm Model and complete the input prompt."""
    def __init__(self, compl_c_ptr: AutoPtr) -> None:
        self._compl_c_ptr = compl_c_ptr

    def __iter__(self):
        utf8_decoder = codecs.getincrementaldecoder("utf-8")()
        while interop.llm_compl_is_active(self._compl_c_ptr.get()) == LL_TRUE:
            chunk_c_ptr = AutoPtr(interop.llm_compl_next_chunk(self._compl_c_ptr.get()),
                                  interop.llm_chunk_destroy)
            chunk_token = interop.llm_chunk_get_text(chunk_c_ptr.get())
            chunk_text = utf8_decoder.decode(chunk_token)
            if chunk_text:
                yield Chunk(text=chunk_text)

        # remaining bytes in utf8_decoder
        chunk_text = utf8_decoder.decode(b"", final=True)
        if chunk_text:
            yield Chunk(text=chunk_text)

class Model:
    """Model in libllm."""
    def __init__(self, model_file) -> None:
        self._model_c_ptr = AutoPtr(interop.llm_model_init(model_file.encode("utf-8")),
                                    interop.llm_model_destroy)

    @property
    def name(self):
        model_name = interop.llm_model_get_name(self._model_instance)
        return model_name.decode("utf-8")

    def complete(self, input_text: str, top_k: int = 50, top_p: float = 0.8,
                 temperature: float = 1.0) -> None:
        compl_opt = AutoPtr(interop.llm_compl_opt_init(), interop.llm_compl_opt_destroy)
        interop.llm_compl_opt_set_top_k(compl_opt.get(), top_k)
        interop.llm_compl_opt_set_top_p(compl_opt.get(), top_p)
        interop.llm_compl_opt_set_temperature(compl_opt.get(), temperature)
        interop.llm_compl_opt_set_prompt(compl_opt.get(), input_text.encode("utf-8"))

        compl_c_ptr = AutoPtr(interop.llm_model_complete(self._model_c_ptr.get(), compl_opt.get()),
                              interop.llm_compl_destroy)
        return Completion(compl_c_ptr)

class Chunk:
    """a chunk of text generated by llm (in Completion)."""
    def __init__(self, text: str) -> None:
        self.text = text
