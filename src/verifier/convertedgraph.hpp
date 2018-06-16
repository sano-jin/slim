/*
 * convertedgraph.hpp
 *
 *   Copyright (c) 2008, Ueda Laboratory LMNtal Group
 *                                         <lmntal@ueda.info.waseda.ac.jp>
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
 * $Id$
 */
#ifndef LMN_CONVERTEDGRAPH_HPP
#define LMN_CONVERTEDGRAPH_HPP
#include "lmntal.h"
#include "vm/atomlist.hpp"
#include "vm/vm.h"
#include <vector>

typedef enum {
  convertedNone,
  convertedAtom,
  convertedHyperLink,
  convertedNull
} ConvertedGraphVertexType;

struct ConvertedGraphVertex {
  ConvertedGraphVertexType type;
  int ID;
  char name[256];
  bool isPushedIntoDiffInfoStack;
  bool isVisitedInBFS;
};

struct ConvertedGraph {
  std::vector<ConvertedGraphVertex> atoms;
  std::vector<ConvertedGraphVertex> hyperlinkatoms;

  ConvertedGraphVertex convert_atom(LmnSymbolAtomRef atom) {
    ConvertedGraphVertex cv;
    return cv;
  }

  std::vector<ConvertedGraphVertex> convert_atoms(LmnMembraneRef mem) {
    std::vector<ConvertedGraphVertex> cv;
    AtomListEntryRef ent;
    EACH_ATOMLIST(mem, ent, ({
                    LmnSymbolAtomRef atom;
                    EACH_ATOM(atom, ent,
                              ({ cv.push_back(convert_atom(atom)); }));
                  }));
    return cv;
  }

  std::vector<ConvertedGraphVertex> convert_hyperlinks(LmnMembraneRef mem) {}

  ConvertedGraph(LmnMembraneRef mem) {
    atoms = convert_atoms(mem);
    // hyperlinkatoms = convert_hyperlinks(mem);
  }
};

#endif
