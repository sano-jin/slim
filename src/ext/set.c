/*
 * set.c - set implementation
 *
 *   Copyright (c) 2017, Ueda Laboratory LMNtal Group
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

#include "set.h"
#include "vm/vm.h"
#include "verifier/verifier.h"

/**
 * @ingroup  Ext
 * @struct LmnSet set.c "ext/set.c"
 */
struct LmnSet{
  LMN_SP_ATOM_HEADER;
  st_table_t tbl;		/* hash table */
};

/**
 * @memberof LmnSet
 * @private
 */
typedef struct LmnSet *LmnSetRef;

/**
 * @memberof LmnSet
 * @private
 */
#define LMN_SET(obj) ((LmnSetRef)(obj))


/* id set */
/**
 * @memberof LmnSet
 * @private
 */
unsigned long id_hash(st_data_t a)
{
  return (unsigned long)a;
}

/**
 * @memberof LmnSet
 * @private
 */
int id_cmp(st_data_t a, st_data_t b)
{
  return a != b;
}

/* tuple set */
/**
 * @memberof LmnSet
 * @private
 */
unsigned long tuple_hash(LmnSymbolAtomRef cons)
{
  unsigned long ret = 0;
  int i;
  for(i = 0; i < LMN_SATOM_GET_ARITY(cons) - 1; i++)
    ret +=(unsigned long)(LMN_SATOM_GET_LINK(cons, i));
  return ret;
}

/**
 * @memberof LmnSet
 * @private
 */
int tuple_cmp(LmnSymbolAtomRef cons0, LmnSymbolAtomRef cons1)
{
  int num0 = LMN_SATOM_GET_ARITY(cons0);
  int num1 = LMN_SATOM_GET_ARITY(cons1);
  int i;
  int ret = 0;
  if(num0 != num1)
    return 1;
  for(i = 0; i < num0 - 1; i++)
    ret = ret || (LMN_SATOM_GET_LINK(cons0, i) != LMN_SATOM_GET_LINK(cons1, i));
  return ret;
}

/**
 * @memberof LmnSet
 * @private
 */
LmnAtomRef find_atom_with_functor(LmnMembraneRef mem, LmnFunctor fun)
{
  AtomListEntryRef ent;
  LmnFunctor f;
  LmnAtomRef ret = NULL;
  EACH_ATOMLIST_WITH_FUNC(mem, ent, f, ({
	LmnAtomRef satom;
	EACH_ATOM(satom, ent, ({
	      if(f==fun){
		ret = satom;
	      }
	    }))
	  }));

  return ret;
}

/* mem set */
/**
 * @memberof LmnSet
 * @private
 */
LmnBinStrRef lmn_inner_mem_encode(LmnMembraneRef m)
{
  LmnAtomRef plus = find_atom_with_functor(m, LMN_UNARY_PLUS_FUNCTOR);
  LMN_ASSERT(plus != NULL);
  LmnAtomRef in = LMN_SATOM(LMN_SATOM_GET_LINK(plus, 0));
  LmnAtomRef out = LMN_SATOM(LMN_SATOM_GET_LINK(in, 0));

  mem_remove_symbol_atom(m, in);
  lmn_delete_atom(in);

  LmnAtomRef at = lmn_mem_newatom(m, lmn_functor_intern(ANONYMOUS, lmn_intern("@"), 1));
  lmn_newlink_in_symbols(plus, 0, at, 0);

  LmnBinStrRef s = lmn_mem_encode(m);

  mem_remove_symbol_atom(m, at);
  lmn_delete_atom(at);

  LmnAtomRef new_in = lmn_mem_newatom(m, LMN_IN_PROXY_FUNCTOR);
  lmn_newlink_in_symbols(in, 0, out, 0);
  lmn_newlink_in_symbols(in, 1, plus, 0);

  return s;
}

/**
 * @memberof LmnSet
 * @private
 */
int mem_cmp(LmnMembraneRef m0, LmnMembraneRef m1)
{
  LmnBinStrRef s0 = lmn_inner_mem_encode(m0);
  LmnBinStrRef s1 = lmn_inner_mem_encode(m1);
  int res = binstr_compare(s0, s1);
  lmn_binstr_free(s0);
  lmn_binstr_free(s1);
  return res;
}

/**
 * @memberof LmnSet
 * @private
 */
unsigned long mem_hash(LmnMembraneRef m)
{
  return mhash(m);
}

/**
 * @memberof LmnSet
 * @private
 */
struct st_hash_type type_id_hash =
  {
    (st_cmp_func)id_cmp,
    (st_hash_func)id_hash
  };

/**
 * @memberof LmnSet
 * @private
 */
struct st_hash_type type_mem_hash =
  {
    (st_cmp_func)mem_cmp,
    (st_hash_func)mem_hash
  };

/**
 * @memberof LmnSet
 * @private
 */
struct st_hash_type type_tuple_hash =
  {
    (st_cmp_func)tuple_cmp,
    (st_hash_func)tuple_hash
  };

/**
 * @memberof LmnSet
 * @private
 */
static int set_atom_type; /* special atom type */

/**
 * @brief Internal Constructor
 * @memberof LmnSet
 * @private
 */
static LmnSetRef make_set(struct st_hash_type *ht)
{
  LmnSetRef s = LMN_MALLOC(struct LmnSet);
  LMN_SP_ATOM_SET_TYPE(s, set_atom_type);
  LMN_SET(s)->tbl = st_init_table(ht);
  return s;
}

/* cb_set_free内で使用する関数のプロトタイプ宣言 */
int inner_set_free(st_data_t, st_data_t, st_data_t);

/**
 * @brief Internal Constructor
 * @memberof LmnSet
 * @private
 */
void lmn_set_free(LmnSetRef set)
{
  st_table_t tbl = LMN_SET(set)->tbl;
  if(tbl->type != &type_id_hash)
    st_foreach(tbl, (int)inner_set_free, tbl->type);
  st_free_table(tbl);
}

/**
 * @memberof LmnSet
 * @private
 */
int inner_set_free(st_data_t key, st_data_t rec, st_data_t arg)
{
  if(arg == &type_mem_hash)
    lmn_mem_free_rec(key);
  else
    free_symbol_atom_with_buddy_data(key);
  return ST_DELETE;
}
/*----------------------------------------------------------------------
 * Callbacks
 */

/*
 * 解放
 *
 * +a0: 集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_free(LmnReactCxtRef rc,
		 LmnMembraneRef mem,
		 LmnAtomRef a0, LmnLinkAttr t0)
{
  lmn_set_free(LMN_SET(a0));
  lmn_mem_remove_data_atom(mem, a0, t0);
}

/*
 * 挿入
 *
 * +a0: 集合
 * +a1: 要素
 * -a2: 新集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_insert(LmnReactCxtRef rc,
		   LmnMembraneRef mem,
		   LmnAtomRef a0, LmnLinkAttr t0,
		   LmnAtomRef a1, LmnLinkAttr t1,
		   LmnAtomRef a2, LmnLinkAttr t2)
{
  if(t0 != LMN_SP_ATOM_ATTR) {
    lmn_mem_delete_atom(mem, a0, t0);
    t0 = LMN_SP_ATOM_ATTR;
    if(LMN_INT_ATTR == t1)
      a0 = (LmnAtomRef)make_set(&type_id_hash);
    else if(LMN_SATOM_GET_FUNCTOR(a1) == LMN_OUT_PROXY_FUNCTOR)
      a0 = (LmnAtomRef)make_set(&type_mem_hash);
    else
      a0 = (LmnAtomRef)make_set(&type_tuple_hash);
  }
  st_table_t tbl = LMN_SET(a0)->tbl;
  st_data_t v = (tbl->type == &type_mem_hash) ? LMN_PROXY_GET_MEM(LMN_SATOM_GET_LINK(a1, 0)) : a1;
  st_insert(tbl, v, v);
  if(tbl->type == &type_tuple_hash) {
    mem_remove_symbol_atom_with_buddy_data(mem, a1);
  } else {
    lmn_mem_remove_atom(mem, a1, t1);
    if(tbl->type == &type_mem_hash)
      lmn_mem_remove_mem(mem, v);
  }
  lmn_mem_newlink(mem,
                  a0, t0, LMN_ATTR_GET_VALUE(t0),
                  a2, t2, LMN_ATTR_GET_VALUE(t2));
}

/*
 * 検索
 *
 * +a0: 集合
 * +a1: 要素
 * -a2: some/none
 * -a2: 新集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_find(LmnReactCxtRef *rc,
		 LmnMembraneRef mem,
		 LmnAtomRef a0, LmnLinkAttr t0,
		 LmnAtomRef a1, LmnLinkAttr t1,
		 LmnAtomRef a2, LmnLinkAttr t2,
		 LmnAtomRef a3, LmnLinkAttr t3)
{
  st_table_t tbl = LMN_SET(a0)->tbl;
  st_data_t key = (tbl->type == &type_mem_hash) ? LMN_PROXY_GET_MEM(LMN_SATOM_GET_LINK(a1, 0)) : a1;
  st_data_t entry;
  int res = st_lookup(tbl, key, &entry);
  if(tbl->type == &type_mem_hash)
      lmn_mem_delete_mem(mem, key);
  lmn_interned_str s = (res) ? lmn_intern("some") : lmn_intern("none");
  LmnAtomRef result = lmn_mem_newatom(mem, lmn_functor_intern(ANONYMOUS, s, 1));
  lmn_mem_delete_atom(mem, a1, t1);
  lmn_mem_newlink(mem,
                  a0, t0, LMN_ATTR_GET_VALUE(t0),
                  a3, t3, LMN_ATTR_GET_VALUE(t3));
  lmn_mem_newlink(mem,
                  a2, t2, LMN_ATTR_GET_VALUE(t2),
                  (LmnAtom)result, LMN_ATTR_MAKE_LINK(0), 0);
}


/* inner_set_to_listで使用するためだけの構造体 */
struct InnerToList{
  LmnMembraneRef mem;
  LmnAtomRef cons;
  LmnAtomRef prev;
  struct st_hash_type *ht;
};

typedef struct InnerToList *InnerToListRef;

/* cb_set_to_list内で使用する関数のプロトタイプ宣言 */
int inner_set_to_list(st_data_t, st_data_t, st_data_t);

/*
 * リストへの変換
 *
 * +a0: 集合
 * -a1: リスト
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_to_list(LmnReactCxtRef rc,
		    LmnMembraneRef mem,
		    LmnAtomRef a0, LmnLinkAttr t0,
		    LmnAtomRef a1, LmnLinkAttr t1)
{
  LmnAtomRef cons = lmn_mem_newatom(mem, LMN_LIST_FUNCTOR);
  lmn_mem_newlink(mem,
		  a1, t1, LMN_ATTR_GET_VALUE(t1),
		  cons, LMN_ATTR_MAKE_LINK(2), 2);
  st_table_t tbl = LMN_SET(a0)->tbl;
  InnerToListRef itl = LMN_MALLOC(struct InnerToList);
  itl->cons = cons;
  itl->mem = mem;
  itl->ht = LMN_SET(a0)->tbl;
  st_foreach(tbl, inner_set_to_list, (st_data_t)itl);

  lmn_mem_delete_atom(itl->mem, itl->cons, LMN_ATTR_MAKE_LINK(2));
  LmnAtomRef nil = lmn_mem_newatom(itl->mem, LMN_NIL_FUNCTOR);
  lmn_newlink_in_symbols(nil, 0, itl->prev, 1);
  LMN_FREE(itl);
  st_free_table(LMN_SET(a0)->tbl);
}


/**
 * @memberof LmnSet
 * @private
 */
int inner_set_to_list(st_data_t key, st_data_t rec, st_data_t obj)
{
  InnerToListRef itl = (InnerToListRef)obj;
  if(itl->ht == &type_id_hash) {
    lmn_mem_newlink(itl->mem,
		    itl->cons, LMN_ATTR_MAKE_LINK(0), 0,
		    (LmnWord)key, LMN_INT_ATTR, 0);
    lmn_mem_push_atom(itl->mem, (LmnWord)key, LMN_INT_ATTR);
  } else if(itl->ht == &type_mem_hash) {
    LmnAtomRef in = find_atom_with_functor(key, LMN_IN_PROXY_FUNCTOR);
    LMN_ASSERT(in != NULL);
    LmnAtomRef out = lmn_mem_newatom(itl->mem, LMN_OUT_PROXY_FUNCTOR);
    lmn_newlink_in_symbols(in, 0, out, 0);
    lmn_mem_newlink(itl->mem,
		    itl->cons, LMN_ATTR_MAKE_LINK(0), 0,
		    out, LMN_ATTR_MAKE_LINK(1), 1);
    lmn_mem_add_child_mem(itl->mem, (LmnMembraneRef)key);
  } else if(itl->ht == &type_tuple_hash) {
    int i;
    lmn_mem_push_atom(itl->mem, (LmnAtomRef)key, LMN_ATTR_MAKE_LINK(3));
    for(i = 0; i < LMN_SATOM_GET_ARITY(key) - 1; i++)
      lmn_mem_push_atom(itl->mem, LMN_SATOM_GET_LINK(key, i), LMN_INT_ATTR);
    lmn_mem_newlink(itl->mem,
		    itl->cons, LMN_ATTR_MAKE_LINK(0), 0,
		    (LmnAtomRef)key, LMN_ATTR_MAKE_LINK(i), i);
  }
  itl->prev = itl->cons;
  itl->cons = lmn_mem_newatom(itl->mem, LMN_LIST_FUNCTOR);
  lmn_mem_newlink(itl->mem,
		  itl->prev, LMN_ATTR_MAKE_LINK(1), 1,
		  itl->cons, LMN_ATTR_MAKE_LINK(2), 2);
  return ST_CONTINUE;
}

int inner_set_copy(st_data_t, st_data_t, st_data_t);

/*
 * 複製
 *
 * +a0: 集合
 * -a1: コピー元の集合
 * -a2: コピー集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_copy(LmnReactCxtRef rc,
		 LmnMembraneRef mem,
		 LmnAtomRef a0, LmnLinkAttr t0,
		 LmnAtomRef a1, LmnLinkAttr t1,
		 LmnAtomRef a2, LmnLinkAttr t2)
{
  LmnSetRef s;
  LmnLinkAttr at = LMN_SP_ATOM_ATTR;
  st_table_t tbl = LMN_SET(a0)->tbl;
  if(tbl->type == &type_id_hash) {
    s = make_set(tbl->type);
    LMN_SET(s)->tbl = st_copy(tbl);
  } else {
    s = make_set(tbl->type);
    st_foreach(tbl, (int)inner_set_copy, s);
  }
  lmn_mem_push_atom(mem, (LmnAtom)s, at);
  lmn_mem_newlink(mem,
		  a0, t0, LMN_ATTR_GET_VALUE(t0),
		  a1, t1, LMN_ATTR_GET_VALUE(t1));
  lmn_mem_newlink(mem,
		  a2, t2, LMN_ATTR_GET_VALUE(t2),
		  s, at, LMN_ATTR_GET_VALUE(at));
}

/**
 * @memberof LmnSet
 * @private
 */
int inner_set_copy(st_data_t key, st_data_t rec, st_data_t arg)
{
  st_table_t tbl = LMN_SET(arg)->tbl;
  st_data_t val = (tbl->type == &type_mem_hash) ? lmn_mem_copy(key) : lmn_copy_satom_with_data((LmnSymbolAtomRef)key, FALSE);
  st_insert(tbl, val, val);
  return ST_CONTINUE;
}

/*
 * 消去
 *
 * +a0: 集合
 * +a1: 要素
 * -a2: 新集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_erase(LmnReactCxtRef rc,
		  LmnMembraneRef mem,
		  LmnAtomRef a0, LmnLinkAttr t0,
		  LmnAtomRef a1, LmnLinkAttr t1,
		  LmnAtomRef a2, LmnLinkAttr t2)
{
  st_table_t tbl = LMN_SET(a0)->tbl;
  st_data_t entry;
  if(tbl->type == &type_id_hash) {
    st_delete(LMN_SET(a0)->tbl, (st_data_t)a1, &entry);
  } else if(tbl->type == &type_mem_hash) {
    LmnMembraneRef m = LMN_PROXY_GET_MEM(LMN_SATOM_GET_LINK(a1, 0));
    if(st_delete(tbl, (st_data_t)m, &entry))
      lmn_mem_free_rec((LmnMembraneRef)entry);
    lmn_mem_delete_mem(mem, m);
  } else if(tbl->type == &type_tuple_hash) {
    if(st_delete(tbl, a1, &entry))
      free_symbol_atom_with_buddy_data(entry);
  }
  lmn_mem_delete_atom(mem, a1, t1);
  lmn_mem_newlink(mem,
		  a0, t0, LMN_ATTR_GET_VALUE(t0),
		  a2, t2, LMN_ATTR_GET_VALUE(t2));
}

/* cb_set_union内で使用する関数のプロトタイプ宣言 */
int inner_set_union(st_data_t, st_data_t, st_data_t);

/*
 * 和集合
 *
 * +a0: 集合X
 * +a1: 集合Y
 * -a2: XとYの和集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_union(LmnReactCxtRef rc,
		  LmnMembraneRef mem,
		  LmnAtomRef a0, LmnLinkAttr t0,
		  LmnAtomRef a1, LmnLinkAttr t1,
		  LmnAtomRef a2, LmnLinkAttr t2)
{
  st_foreach(LMN_SET(a0)->tbl, (int)inner_set_union, a1);
  lmn_mem_newlink(mem,
		  a1, t1, LMN_ATTR_GET_VALUE(t1),
		  a2, t2, LMN_ATTR_GET_VALUE(t2));
  lmn_set_free(a0);
}

/**
 * @memberof LmnSet
 * @private
 */
int inner_set_union(st_data_t key, st_data_t rec, st_data_t arg)
{
  st_table_t tbl = LMN_SET(arg)->tbl;
  st_data_t entry;
  if(tbl->type == &type_id_hash) {
    st_insert(tbl, key, rec)     ;
  } else if(tbl->type == &type_mem_hash || tbl->type == &type_tuple_hash) {
    if(!st_lookup(tbl, key, &entry)) {
      st_insert(tbl, key, rec);
      return ST_DELETE;
    }
  }
  return ST_CONTINUE;
}

/* cb_set_intersect内で使用する関数のプロトタイプ宣言 */
int inner_set_intersect(st_data_t, st_data_t, st_data_t);

/*
 * 積集合
 *
 * +a0: 集合X
 * +a1: 集合Y
 * -a2: XとYの積集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_intersect(LmnReactCxtRef rc,
		      LmnMembraneRef mem,
		      LmnAtomRef a0, LmnLinkAttr t0,
		      LmnAtomRef a1, LmnLinkAttr t1,
		      LmnAtomRef a2, LmnLinkAttr t2)
{
  st_table_t tbl = LMN_SET(a0)->tbl;
  st_foreach(tbl, (int)inner_set_intersect, a1);
  lmn_set_free(a1);
  if(st_num(tbl)) {
    lmn_mem_newlink(mem,
		    a2, t2, LMN_ATTR_GET_VALUE(t2),
		    a0, t0, LMN_ATTR_GET_VALUE(t0));
  } else {
    LmnAtomRef empty_set = lmn_mem_newatom(mem, lmn_functor_intern(ANONYMOUS, lmn_intern("set_empty"), 1));
    lmn_mem_newlink(mem,
  		    a2, t2, LMN_ATTR_GET_VALUE(t2),
  		    (LmnAtom)empty_set, LMN_ATTR_MAKE_LINK(0), 0);
    lmn_set_free(a0);
  }
}

/**
 * @memberof LmnSet
 * @private
 */
int inner_set_intersect(st_data_t key, st_data_t rec, st_data_t arg)
{
  st_table_t tbl = LMN_SET(arg)->tbl;
  st_data_t entry;
  if(!st_lookup(tbl, key, &entry)) {
    if(tbl->type == &type_mem_hash)
      lmn_mem_free_rec(key);
    else if(tbl->type == &type_tuple_hash)
      free_symbol_atom_with_buddy_data(key);
    return ST_DELETE;
  }
  return ST_CONTINUE;
}

int inner_set_diff(st_data_t, st_data_t, st_data_t);

/*
 * 差集合
 *
 * +a0: 集合X
 * +a1: 集合Y
 * -a2: XとYの差集合
 */
/**
 * @memberof LmnSet
 * @private
 */
void cb_set_diff(LmnReactCxtRef rc,
		 LmnMembraneRef mem,
		 LmnAtomRef a0, LmnLinkAttr t0,
		 LmnAtomRef a1, LmnLinkAttr t1,
		 LmnAtomRef a2, LmnLinkAttr t2)
{
  st_table_t tbl = LMN_SET(a0)->tbl;
  st_foreach(tbl, (int)inner_set_diff, a1);
  lmn_set_free(a1);
  if(st_num(tbl)) {
    lmn_mem_newlink(mem,
		    a2, t2, LMN_ATTR_GET_VALUE(t2),
		    a0, t0, LMN_ATTR_GET_VALUE(t0));
  } else {
    LmnAtomRef empty_set = lmn_mem_newatom(mem, lmn_functor_intern(ANONYMOUS, lmn_intern("set_empty"), 1));
    lmn_mem_newlink(mem,
		    a2, t2, LMN_ATTR_GET_VALUE(t2),
		    (LmnAtom)empty_set, LMN_ATTR_MAKE_LINK(0), 0);
    lmn_set_free(a0);
  }
}

/**
 * @memberof LmnSet
 * @private
 */
int inner_set_diff(st_data_t key, st_data_t rec, st_data_t arg)
{
  st_table_t tbl = LMN_SET(arg)->tbl;
  st_data_t entry;
  if(st_lookup(tbl, key, &entry)) {
    if(tbl->type == &type_mem_hash)
      lmn_mem_free_rec(key);
    else if(tbl->type == &type_tuple_hash)
      free_symbol_atom_with_buddy_data(key);
    return ST_DELETE;
  }
  return ST_CONTINUE;
}
/*----------------------------------------------------------------------
 * Initialization
 */

/**
 * @memberof LmnSet
 * @private
 */
void *sp_cb_set_copy(void *data)
{
  return data;
}

/**
 * @memberof LmnSet
 * @private
 */
void sp_cb_set_free(void *data)
{

}

/**
 * @memberof LmnSet
 * @private
 */
void sp_cb_set_eq(void *data1, void *data2)
{

}

/**
 * @memberof LmnSet
 * @private
 */
void sp_cb_set_dump(void *set, LmnPortRef port)
{
  port_put_raw_s(port, "<set>");
}

/**
 * @memberof LmnSet
 * @private
 */
void sp_cb_set_is_ground(void *data)
{

}

/**
 * @memberof LmnSet
 * @private
 */
void init_set(void)
{
  set_atom_type = lmn_sp_atom_register("set",
                                       sp_cb_set_copy,
                                       sp_cb_set_free,
                                       sp_cb_set_eq,
                                       sp_cb_set_dump,
                                       sp_cb_set_is_ground);

  lmn_register_c_fun("cb_set_free", (void *)cb_set_free, 1);
  lmn_register_c_fun("cb_set_insert", (void *)cb_set_insert, 3);
  lmn_register_c_fun("cb_set_find", (void *)cb_set_find, 4);
  lmn_register_c_fun("cb_set_to_list", (void *)cb_set_to_list, 2);
  lmn_register_c_fun("cb_set_copy", (void *)cb_set_copy, 3);
  lmn_register_c_fun("cb_set_erase", (void *)cb_set_erase, 3);
  lmn_register_c_fun("cb_set_union", (void *)cb_set_union, 3);
  lmn_register_c_fun("cb_set_intersect", (void *)cb_set_intersect, 3);
  lmn_register_c_fun("cb_set_diff", (void *)cb_set_diff, 3);
}

