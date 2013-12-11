/** @file hash.h
 *  @brief FDF translation(key <-> addr) module.
 *
 *  This contains function declarations and datastructures for 
 *  key to addr translation and vice-versa
 *
 *  @author Tomy Cheru (tcheru)
 *  SanDisk Proprietary Material, © Copyright 2012 SanDisk, all rights reserved.
 *  http://www.sandisk.com
 */

#include <sdftcp/locks.h>
#include <common/fdftypes.h>
#include <platform/logging.h>
#include <fth/fthLock.h>
#include <platform/alloc.h>

#define BTREE_HACK

/*
 * Macros to print messages.
 */

#define log_msg(id, args...)    plat_log_msg( id, \
                    PLAT_LOG_CAT_SDF_APP_MEMCACHED, ##args )
#define dbg_msg(...)        plat_log_msg( PLAT_LOG_ID_INITIAL, \
                    PLAT_LOG_CAT_SDF_APP_MEMCACHED, __VA_ARGS__ )
/*
 * Macros to allocate memory space
 */

#define plat_alloc_large        plat_alloc_steal_from_heap
#define plat_free_large         plat_free

/*
 * incore hash constants
 */

/* 
 * for enumeration we need to preallocate space
 * if we have more hash_entry in any bucket than 
 * OSD_HASH_MAX_CHAIN enumeration outsome will not be complete
 * might need to increase OSD_HASH_MAX_CHAIN in such case.
 */
#define OSD_HASH_MAX_CHAIN 100

#define OSD_HASH_ENTRY_PER_BUCKET_ENTRY 4 
#define OSD_HASH_BUCKET_SIZE 16

#define OSD_HASH_SYN_SIZE 16
#define OSD_HASH_SYN_SHIFT 48

#ifdef BTREE_HACK
#define OSD_HASH_ENTRY_HOPED_SIZE 18
#else
#define OSD_HASH_ENTRY_HOPED_SIZE 10
#endif

#define OSD_HASH_LOCK_BUCKETS 262144
#define OSD_HASH_LOCKBKT_MINSIZE 256

#ifndef FALSE
enum boo{
    FALSE,
    TRUE,
};
#endif

enum mode{
    FIFO, //fifo mode
    SLAB, //slab mode
};

enum mode1{
    SYN, //key to addr
    ADDR, //addr to key
};

/*
 * In-core structures
 */
typedef struct hash_entry {
    unsigned int used:1;
    unsigned int deleted:1;
    unsigned int referenced:1;
    unsigned int reserved:1;
    unsigned int blocks:12;
    hashsyn_t    syndrome;
    baddr_t      address;
    cntr_id_t    cntr_id;
#ifdef BTREE_HACK
    uint64_t	 key;
#endif
}
    __attribute__ ((packed))
    __attribute__ ((aligned (2)))
hash_entry_t;

typedef struct bucket_entry {
    hash_entry_t hash_entry[OSD_HASH_ENTRY_PER_BUCKET_ENTRY];
    uint32_t   next;
}
    __attribute__ ((packed))
    __attribute__ ((aligned (2)))
bucket_entry_t;

typedef struct hash_handle {
    struct mcd_osd_shard    * shard;
    int                       bkti_l2_size;
    uint64_t                  bkti_l2_mask;
    uint32_t                * addr_table;
    uint64_t                  hash_size;
    bucket_entry_t          * hash_table;
    uint32_t                  hash_table_idx;
    uint32_t                * hash_buckets;
    int                       lock_bktsize;
    uint64_t                  lock_buckets;
    fthLock_t               * bucket_locks;
    uint32_t                * bucket_locks_free_list;
    uint64_t                * bucket_locks_free_map;
    uint64_t                  total_alloc;
} hash_handle_t;

hash_handle_t *
hash_table_init ( uint64_t total_size, uint64_t max_nobjs, int mode);

void
hash_table_cleanup ( hash_handle_t *hdl);

hash_entry_t *
hash_table_get (void *context, hash_handle_t *hdl, char *key, 
                    int key_len, cntr_id_t cntr_id);

void
hash_entry_copy ( hash_entry_t *dst, hash_entry_t *src);

void
hash_entry_delete ( hash_handle_t *hdl, hash_entry_t *he, 
                            uint32_t hash_idx);

hash_entry_t *
hash_entry_insert_by_key(hash_handle_t *hdl, uint64_t syndrome);

hash_entry_t *
hash_entry_insert_by_addr(hash_handle_t *hdl, uint32_t addr, 
                                uint64_t syndrome);

fthLock_t *
hash_table_find_lock ( hash_handle_t *hdl, uint64_t hint, int mode );

struct mcd_rec_flash_object; // to satisfy the compiler
hash_entry_t *
hash_entry_recovery_insert(hash_handle_t *hdl, 
        struct mcd_rec_flash_object *obj, uint64_t blk_offset);

struct mcd_osd_meta;
int
obj_valid( hash_handle_t *hdl, struct mcd_osd_meta *meta, uint32_t addr);