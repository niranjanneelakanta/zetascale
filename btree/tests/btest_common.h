//----------------------------------------------------------------------------
// ZetaScale
// Copyright (c) 2016, SanDisk Corp. and/or all its affiliates.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published by the Free
// Software Foundation;
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License v2.1 for more details.
//
// A copy of the GNU Lesser General Public License v2.1 is provided with this package and
// can also be found at: http://opensource.org/licenses/LGPL-2.1
// You should have received a copy of the GNU Lesser General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA.
//----------------------------------------------------------------------------

/************************************************************************
 * 
 *  btest_common.h  May. 17, 2013   Harihara Kadayam
 * 
 *  Built-in self-Test program for btree package.
 * 
 * NOTES: xxxzzz
 * 
 ************************************************************************/

#ifndef __BTEST_COMMON_H
#define __BTEST_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include "../btree_raw.h"

typedef struct {
	/* All user input configs */
	char          *program;
	uint64_t      n_test_keys;
	uint64_t      n_test_iters;
	uint32_t      n_partitions;
	uint32_t      flags; 
	uint32_t      max_key_size;
	uint32_t      min_keys_per_node;
	uint32_t      nodesize;
	uint32_t      n_l1cache_buckets;
	uint32_t      max_datalen;

	/* Generated ones */
	struct btree  *bt;
	struct Map    *kmap;
	uint64_t      *keys;
	uint32_t      *keylens;
	char          **datas;
	uint32_t      *datalens;
	char          *keysuffix;
} btest_cfg_t;

typedef int (*btest_parse_fn)(btest_cfg_t *, int, char **);

btest_cfg_t *btest_init(int argc, char **argv, char *program, btest_parse_fn parse_fn);
void btest_common_usage(char *program);
int btest_basic_parse(btest_cfg_t *cfg, int argc, char **argv);
void btest_rand_data_gen(btest_cfg_t *cfg);
void btest_serial_data_gen(btest_cfg_t *cfg);
void btest_life_cycle(btest_cfg_t *cfg);
#endif
