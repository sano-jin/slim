/*
 * syntax.h - syntax tree  of the Intermediate Language
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
 * $Id: syntax.h,v 1.4 2008/09/29 05:23:40 taisuke Exp $
 */

#ifndef LMN_SYNTAX_H
#define LMN_SYNTAX_H

#include "lmntal.h"
#include "vm/vm.h"
#include "element/element.h"

/**
 * @ingroup  Loader
 * @defgroup Syntax
 * @{
 */

/* 型名の解決の為に上に持ってきた */
typedef struct __InstList *InstList;

typedef struct InstrArg *InstrArgRef;

typedef struct __VarList *VarList;

/* List of instrction variables */


/* Functor */

enum FunctorType {STX_SYMBOL, INT_FUNC, FLOAT_FUNC, STRING_FUNC, STX_IN_PROXY, STX_OUT_PROXY, STX_UNIFY};

typedef struct Functor *FunctorRef;

/* Instruction Argument */


/* List of arguments */

typedef struct __ArgList *ArgList;

/* Instruction */

typedef struct Instruction *InstructionRef;

/* List of instructions */
/* amatch, memmatchなど、命令をまとめたもの */

typedef struct InstBlock *InstBlockRef;
/* Rule */

typedef struct Rule *RuleRef;

/* List of rules */

typedef struct __RuleList *RuleList;

/* Rule set */

typedef struct RuleSet *RuleSetRef;

/* List of rule sets */

typedef struct __RuleSets *RuleSets;
/* Module, モジュール名とルールセットの対応 */

typedef struct Module *ModuleRef;

/* Root of the IL syntax tree */

typedef struct IL *ILRef;

void stx_rule_free(RuleRef rule);

/* @} */

#endif
