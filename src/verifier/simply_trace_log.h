/*
 * simply_trace_log.h
 *
 *   Copyright (c) 2018, Ueda Laboratory LMNtal Group <lmntal@ueda.info.waseda.ac.jp>
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
 */

#ifndef SIMPLY_TRACE_LOG
#define SIMPLY_TRACE_LOG

#include "vm/vm.h"

#include "simply_process_table.h"
#include "trace_log.h"

/** ------
 *  SimpleTraceLog
 */

struct SimplyTraceLog {
  struct SimplyProcTbl tbl; /* Process IDをkey, 訪問済みか否かの真偽値をvalueとしたテーブル */
  struct LogTracker tracker;
};

#define STRACE_TRUE   (!SPROC_TBL_INIT_V)

typedef struct SimplyTraceLog *SimplyLog;

/**
 * Function ProtoTypes
 */

void simplylog_init(SimplyLog trc);
void simplylog_init_with_size(SimplyLog trc, unsigned long size);
void simplylog_destroy(SimplyLog trc);

void simplylog_put(SimplyLog l, LmnWord key);
void simplylog_put_atom(SimplyLog l, LmnSymbolAtomRef atom);
void simplylog_put_mem(SimplyLog l, LmnMembraneRef mem);
BOOL simplylog_contains_atom(SimplyLog l, LmnSymbolAtomRef atom);
BOOL simplylog_contains_mem(SimplyLog l, LmnMembraneRef mem);
void simplylog_backtrack(SimplyLog l);
void simplylog_set_btpoint(SimplyLog l);
void simplylog_continue_trace(SimplyLog l);


#endif /* SIMPLY_TRACE_LOG */

