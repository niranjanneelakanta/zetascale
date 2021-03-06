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

/**********************************************************************
 *
 *  Test program for checking formulas that set nslabs.
 *  Brian O'Krafka  11/4/10
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

typedef struct SDFNewCache {
    uint64_t              size_limit;
    uint64_t              nbuckets;
    uint64_t              nslabs;
    uint64_t              slabsize;
    uint64_t              pages_per_slab;
    uint64_t              mem_used;
    uint64_t              max_object_size;
    uint32_t              max_key_size;
    uint64_t              max_obj_entry_size;
    uint32_t              page_size;
    uint32_t              page_data_size;
} SDFNewCache_t;

static uint64_t check_nslabs(SDFNewCache_t *pc, uint64_t nslabs_in)
{
    uint64_t      npages;
    uint64_t      nslabs;
    uint64_t      max_nslabs;

    for (nslabs=nslabs_in; nslabs>0; nslabs--) {
	npages     = (pc->size_limit - 112000000 - nslabs*392 + 8)/pc->page_size;
	max_nslabs = npages/ceil(((double) pc->max_obj_entry_size)/((double) pc->page_data_size));
	if (max_nslabs >= nslabs) {
	    break;
	}
    }
    if (nslabs == 0) {
        // There is insufficient memory for even a single slab!
        fprintf(stderr, "There is insufficient memory for even a single slab!  "
		     "There must be at least enough memory to hold one max sized object.  "
		     "Try increasing the cache size (SDF_CC_MAXCACHESIZE).");
	abort();
    }

    return(nslabs);
}


static void fastcc_init_object_alloc(SDFNewCache_t *pc)
{
    uint64_t             mem_slabs;

    /*  Take remaining free cache memory and carve it into
     *  pages and distribute them among the slabs.
     */

    pc->pages_per_slab = (pc->size_limit - pc->mem_used)/pc->page_size/pc->nslabs;
    mem_slabs = pc->pages_per_slab*pc->page_size*pc->nslabs;

    if ((pc->pages_per_slab*pc->page_data_size) < pc->max_obj_entry_size) {
        fprintf(stderr, "Not enough memory for cache: must have at least enough memory for a single maximum sized object per slab (%"PRIu64" required, %"PRIu64" available). Try increasing SDF_CC_MAXCACHESIZE or reducing SDF_CC_NSLABS", pc->max_obj_entry_size, pc->pages_per_slab*pc->page_data_size);
        abort();
    }
}

main()
{
    SDFNewCache_t   cache;

    cache.size_limit         = 13613033062ULL;
    cache.nbuckets           = 1000000;
    cache.mem_used           = 112573496;
    cache.max_object_size    = 8388608;
    cache.max_key_size       = 256;
    cache.max_obj_entry_size = 8388964;
    cache.page_size          = 88;
    cache.page_data_size     = 80;

    cache.nslabs = check_nslabs(&cache, 10000);
    fastcc_init_object_alloc(&cache);

    fprintf(stderr, "nslabs = %"PRIu64"\n", cache.nslabs);
}
