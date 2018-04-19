/*
 * syntax.c - syntax tree  of the Intermediate Language
 *
 *   Copyright (c) 2008, Ueda Laboratory LMNtal Group <lmntal@ueda.info.waseda.ac.jp>
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are
 *   met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *    3. Neither the name of the Ueda Laboratory LMNtal Group nor the
 *       names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior
 *       written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: syntax.c,v 1.5 2008/09/29 05:23:40 taisuke Exp $
 */

#include "syntax.hpp"

#include "byte_encoder.hpp"

namespace il {
  namespace functor {
    void in_proxy::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
    void out_proxy::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
    void unify::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
    void integer::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
    void real::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
    void string::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
    void symbol::visit(ByteEncoder &encoder) {
      encoder.load(*this);
    }
  }
}


namespace il {
  namespace instr_arg {
    void var::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void label::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void string::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void lineno::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void functor::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void ruleset::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void var_list::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
    void inst_list::visit(ByteEncoder &encoder) const {
      encoder.load(*this);
    }
  }
}

extern "C" void stx_rule_free(Rule *rule);

void stx_rule_free(Rule *rule)
{
  delete rule;
}
