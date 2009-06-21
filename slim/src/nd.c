/*
 * nd.c
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

#include "nd.h"
#include "por.h"
#include "task.h"
#include "dumper.h"

st_table *States;

static Vector *expand_sub(struct ReactCxt *rc, LmnMembrane *cur_mem);
static BOOL react_all_rulesets(struct ReactCxt *rc,
                               LmnMembrane *cur_mem);
static void nd_loop(State *init_state);
static int kill_States_chains(st_data_t _k,
                              st_data_t state_ptr,
                              st_data_t rm_tbl_ptr);
static void do_nd(LmnMembrane *world_mem);
static BOOL expand_inner(struct ReactCxt *rc,
                         LmnMembrane *cur_mem);

/* 状態stateから１ステップで遷移する状態のベクタを返す。
   返される状態のベクタには、重複はない */
Vector *nd_expand(State *state)
{
  Vector *r;

  if (lmn_env.por) { r = ample(state); }
  else  {
    struct ReactCxt rc;
  
    nd_react_cxt_init(&rc, DEFAULT_STATE_ID);
    RC_SET_GROOT_MEM(&rc, state_mem(state));
    r = expand_sub(&rc, state_mem(state));
    nd_react_cxt_destroy(&rc);
  }
  return r;
}

static Vector *expand_sub(struct ReactCxt *rc, LmnMembrane *cur_mem)
{
  int i;
  Vector *expanded_roots;
  Vector *expanded;
  st_table *s;
  
  expand_inner(rc, cur_mem);
  expanded_roots = RC_EXPANDED(rc);

  s = st_init_table(&type_memhash);
  expanded = vec_make(32);
  for (i = 0; i < vec_num(expanded_roots); i++) {
    State *state;
    LmnMembrane *root = (LmnMembrane *)vec_get(expanded_roots, i);
    st_data_t t;

    if (st_lookup(s, (st_data_t)root, &t)) {
      lmn_mem_free(root);
    } else {
      t = (st_data_t)root;
    }
    state = state_make_for_nd((LmnMembrane *)t,
                              (LmnRule)vec_get(RC_EXPANDED_RULES(rc), i));
    vec_push(expanded, (LmnWord)state);
  }

  st_free_table(s);
  return expanded;
}

/*
 * 状態を展開する
 * 引数として与えられた膜と、その膜に所属する全てのアクティブ膜に対して
 * ルール適用検査を行う
 * 子膜からルール適用を行っていく
 */
/* must_be_activatedはこの膜がstableである可能性がなくなったときにTRUEを代入する
 * 子膜と自身のルール適応テストが終了したときFALSEならこの膜はstableである
 */
static BOOL expand_inner(struct ReactCxt *rc,
                         LmnMembrane *cur_mem)
{
  BOOL ret_flag = FALSE;
  for (; cur_mem; cur_mem = cur_mem->next) {

    /* 代表子膜に対して再帰する */
    if (expand_inner(rc, cur_mem->child_head)) {
      ret_flag = TRUE;
    }

    if (cur_mem->is_activated && react_all_rulesets(rc, cur_mem)) {
      ret_flag = TRUE;
    }

    if(!ret_flag){
      cur_mem->is_activated = FALSE;
    }
  }
  return ret_flag;
}

/*
 * nondeterministic execution
 * 深さ優先で全実行経路を取得する
 */
static void nd_loop(State *init_state) {
  Vector *stack;
  unsigned long i;
  
  stack = vec_make(2048);
  vec_push(stack, (LmnWord)init_state);

  while (vec_num(stack) != 0) {
    /* 展開元の状態。popはしないでsuccessorの展開が終わるまで
       スタックに積んでおく */
    State *s = (State *)vec_peek(stack); 
    Vector *expanded;
    unsigned long expanded_num;
    
    if (is_expanded(s)) {
      /* 状態が展開済みである場合，スタック上から除去してフラグを解除する */
      vec_pop(stack);
      unset_open(s);
      continue;
    }      

    expanded = nd_expand(s); /* 展開先をexpandedに格納する */
    expanded_num = vec_num(expanded);

    state_succ_init(s, vec_num(expanded));
    for (i = 0; i < expanded_num; i++) {
      State *src_succ = (State *)vec_get(expanded, i);
      State *succ = insert_state(States, src_succ);

      if (succ == src_succ) { /* succが状態空間に追加された */
        vec_push(stack, (LmnWord)src_succ);
        /* set ss to open, i.e., on the search stack */
        set_open(src_succ);
      } else { /* src_succは追加されなかった（すでに同じ状態がある) */
        state_free(src_succ);
      }
      vec_push(&s->successor, (LmnWord)succ);
    }
    vec_free(expanded);
    set_expanded(s); /* sに展開済みフラグを立てる */
  }

  vec_free(stack);
}

/* 非決定実行を行う */
void run_nd(LmnRuleSet start_ruleset)
{
  LmnMembrane *mem;
  struct ReactCxt init_rc;

  stand_alone_react_cxt_init(&init_rc);

  /* make global root membrane */
  mem = lmn_mem_make();
  RC_SET_GROOT_MEM(&init_rc, mem);
  lmn_react_ruleset(&init_rc, mem, start_ruleset);
  stand_alone_react_cxt_destroy(&init_rc);
  
  activate_ancestors(mem);

  do_nd(mem);

#ifdef PROFILE
  calc_hash_conflict(States);
#endif

  /* finalize */
  state_space_free(States);
  
  free_por_vars();
}

static void do_nd(LmnMembrane *world_mem)
{
  State *initial_state;

  /**
   * initialize containers
   */
  States = st_init_table(&type_statehash);
  init_por_vars();

  /* 初期プロセスから得られる初期状態を生成 */
  initial_state = state_make_for_nd(world_mem, ANONYMOUS);
/*   mc_flags.initial_state = initial_state; */
  st_add_direct(States, (st_data_t)initial_state, (st_data_t)initial_state);

/*   /\* --nd_dumpの実行 *\/ */
/*   else if(lmn_env.nd_dump){ */
/*     nd_dump_exec(); */
/*   } */
  /* --ndの実行（非決定実行後に状態遷移グラフを出力する） */
/*   else{ */
  nd_loop(initial_state);
  dump_state_transition_graph(initial_state, stdout);
/*   } */
 
  fprintf(stdout, "# of States = %d\n", st_num(States));
}

/**
 * 非決定(--nd または --nd_result)実行終了時に状態遷移グラフを出力する．
 * 高階関数st_foreach(c.f. st.c)に投げて使用．
 */
static int print_state_transition_graph(st_data_t _k, st_data_t state_ptr, st_data_t _a) {
  unsigned int j = 0;
  State *tmp = (State *)state_ptr;

  fprintf(stdout, "%lu::", (long unsigned int)tmp); /* dump src state's ID */
  while (j < vec_num(&tmp->successor)) { /* dump dst state's IDs */
    fprintf(stdout, "%lu", vec_get(&tmp->successor, j++));
    if (j < vec_num(&tmp->successor)) {
      fprintf(stdout,",");
    }
  }
  fprintf(stdout, "::");
  lmn_dump_cell_stdout(tmp->mem); /* dump src state's global root membrane */
  return ST_CONTINUE;
}

/* cur_memと、その子孫膜に存在するルールすべてに対して適用を試みる */
static BOOL react_all_rulesets(struct ReactCxt *rc,
                               LmnMembrane *cur_mem)
{
  unsigned int i;
  struct Vector rulesets = cur_mem->rulesets; /* 本膜のルールセットの集合 */
  BOOL ok = FALSE;

  for (i = 0; i < vec_num(&rulesets); i++) {
    ok = ok || lmn_react_ruleset(rc, cur_mem,
                                 (LmnRuleSet)vec_get(&rulesets, i));
  }

  if (!ok) { /* 通常のルールセットが適用できなかった場合 */
    /* システムルールセットの適用 */
    if (lmn_react_ruleset(rc, cur_mem, system_ruleset)) {
      ok = TRUE;
    }
  }

  return ok;
}

void state_space_free(StateSpace states)
{
  HashSet rm_tbl; /* LTLモデル検査モード時に二重解放を防止するため */

  hashset_init(&rm_tbl, 16);
  st_foreach(states, kill_States_chains, (st_data_t)&rm_tbl);
  hashset_destroy(&rm_tbl);
  st_free_table(states);
}

/**
 * 非決定実行 or LTLモデル検査終了後にStates内に存在するチェインをすべてfreeする
 * 高階関数st_foreach(c.f. st.c)に投げて使用
 */
static int kill_States_chains(st_data_t _k, st_data_t state_ptr, st_data_t rm_tbl_ptr)
{
  State *tmp = (State *)state_ptr;
  HashSet *rm_tbl = (HashSet *)rm_tbl_ptr;

  if(hashset_contains(rm_tbl, (HashKeyType)tmp->mem)) {
    vec_destroy(&tmp->successor);
    LMN_FREE(tmp);
  }
  else {
    hashset_add(rm_tbl, (HashKeyType)tmp->mem);
    state_free(tmp);
  }
  return ST_CONTINUE;
}

State *insert_state(StateSpace states, State *s)
{
  State *t;
  if (st_lookup(states, (st_data_t)s, (st_data_t *)&t)) {
    return t;
  } else {
    st_add_direct(states, (st_data_t)s, (st_data_t)s); /* 状態空間に追加 */
    return s;
  }
}

void dump_state_transition_graph(State *init_state, FILE *file)
{
  fprintf(file, "init:%lu\n", (long unsigned int)init_state);
  st_foreach(States, print_state_transition_graph, 0);
  fprintf(file, "\n");
}
