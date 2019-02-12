/*
 * statespace.h
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

/** @author Masato Gocho
 *  Closed Address Hash Table / Parallel Hash Table for State Management Table
 */

#ifndef LMN_STATESPACE_H
#define LMN_STATESPACE_H

#include "../lmntal.h"
#include "element/element.h"

#include "state.hpp"

#include <memory>
#include <set>

/* the member "tbl_type" in struct StateSpace */
#define SS_MEMID_MASK (0x01U)
#define SS_REHASHER_MASK (0x01U << 1)

struct MemIdHashSkeleton {
  void add_hash(unsigned long hash) {}
  bool contains_hash(unsigned long hash) { return false; }
};

struct MemIdHash {
  std::set<unsigned long> memid_hashes;

  void add_hash(unsigned long hash) { memid_hashes.insert(hash); }
  /* hashが膜のIDを計算しているハッシュならば真、そうでなければ偽を返す */
  bool contains_hash(unsigned long hash) {
    return memid_hashes.find(hash) != memid_hashes.end();
  }
};

struct StateSpace : public std::conditional<slim::config::profile, MemIdHash,
                                            MemIdHashSkeleton>::type {
  StateSpace();
  StateSpace(AutomataRef a, Vector *psyms) : StateSpace() {
    this->end_states = std::vector<std::vector<State *>>(thread_num);
    this->property_automata = a;
    this->propsyms = psyms;
    this->make_table();
  }
  /* for parallel model checker mode */
  StateSpace(int thread_num, AutomataRef a, Vector *psyms) : StateSpace() {
    this->thread_num = thread_num;
    this->property_automata = a;
    this->propsyms = psyms;
    this->end_states = std::vector<std::vector<State *>>(thread_num);
    this->make_table();
  }

  void add_memid_hash(unsigned long hash);
  State *insert(State *s);
  State *insert_delta(State *s, struct MemDeltaRoot *d);
  void add_direct(State *s);
  void set_init_state(State *init_state, BOOL enable_binstr);
  void dump() const;
  void dump_all_states() const;
  void dump_all_transitions() const;
  void dump_all_labels() const;
  void format_states();
  unsigned long space() const;
  void dump_ends() const;
  unsigned long num() const;
  unsigned long num_raw() const;
  unsigned long dummy_num() const;
  unsigned long num_of_ends() const;
  State *initial_state() { return init_state; }
  void mark_as_end(State *);

  StateTable &accept_tbl() { return *mhash_table.acc; }
  StateTable &accept_memid_tbl() { return *memid_table.acc; }

  void make_table();

  bool use_memenc() const { return ((this)->tbl_type & SS_MEMID_MASK); }
  void set_memenc() { ((this)->tbl_type |= SS_MEMID_MASK); }
  void set_rehasher() { ((this)->tbl_type |= SS_REHASHER_MASK); }
  bool is_formatted() const { return this->is_formated; }
  bool has_property() const { return this->property_automata; }
  AutomataRef automata() { return this->property_automata; }
  Vector *prop_symbols() { return this->propsyms; }

  FILE *output() { return out; }

  std::vector<State *> all_states() const;

private:
  BYTE tbl_type; /* なんらかの特殊操作を行うためのフラグフィールド */
  BOOL is_formated; /* ハッシュ表の並びを崩した整列を行った場合に真 */
  /* 2bytes alignment */
  unsigned int thread_num; /* 本テーブルの操作スレッド数 */

  FILE *out;         /* dump先 */
  State *init_state; /* 初期状態 */

  AutomataRef property_automata; /* Never Clainへのポインタ */
  Vector *propsyms;              /* 命題記号定義へのポインタ */

  /* それぞれ全状態と受理状態を管理する表の組 */
  struct TablePair {
    std::unique_ptr<StateTable> tbl;
    std::unique_ptr<StateTable> acc;
  };

  void make_table_pair(TablePair &t);
  void make_table_pair(TablePair &t, TablePair &rehasher);

  /* mhash値をkeyに, 状態のアドレスを登録する状態管理表 */
  TablePair mhash_table;
  /* memid_hashをkeyに, 状態のアドレスを登録する状態管理表 */
  TablePair memid_table;

  std::vector<std::vector<State *>> end_states; /* 最終状態の集合 */

  std::unique_ptr<StateTable> &insert_destination(State *s, unsigned long hashv);
  std::unique_ptr<StateTable> &resize_destination(std::unique_ptr<StateTable> &def, State *ret, State *s);
};

#endif
