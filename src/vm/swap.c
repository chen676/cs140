#include "vm/swap.h"
#include "threads/malloc.h"
#include <debug.h>


#define BLOCKS_UNIT_NUMBER 8   /*each page corresponds to 8 blocks in disk*/

static struct list swap_space_pool;  /* swap table */
static struct lock swap_space_pool_lock; /* the lock of swap table */

struct block *swap_block;
struct swap_page_block *get_free_spb(void);


/* swap pool init */
void swap_pool_init(void) {
	lock_init(&swap_space_pool_lock);
	list_init(&swap_space_pool);

	swap_block = block_get_role (BLOCK_SWAP);
	uint32_t swap_pool_size = block_size(swap_block);
	uint32_t i;
	struct swap_page_block *spb = NULL;
	/* populate the whole swap pool */
	lock_acquire(&swap_space_pool_lock);
	for (i = 0; i < swap_pool_size; i = i+BLOCKS_UNIT_NUMBER) {
		spb = malloc(sizeof(struct swap_page_block));
		spb->block_sector_head = (block_sector_t)i;
		list_push_back(&swap_space_pool, &spb->elem);
	}
	lock_release(&swap_space_pool_lock);
}

/* retrieve a free swap_page_block from the swap pool*/
struct swap_page_block *get_free_spb(void) {
	lock_acquire(&swap_space_pool_lock);
	if (list_empty(&swap_space_pool)) {
		lock_release(&swap_space_pool_lock);
		PANIC("NO FREE SWAP BLOCK");
	}
	struct swap_page_block *result =
			list_entry(list_pop_front(&swap_space_pool),
					struct swap_page_block, elem);
	lock_release(&swap_space_pool_lock);
	return result;
}

/* put the swap_page_block back into the swap pool */
void put_back_spb(struct swap_page_block *spb) {
	lock_acquire(&swap_space_pool_lock);
	list_push_back(&swap_space_pool, &spb->elem);
	lock_release(&swap_space_pool_lock);
}


/* swap in */
void swap_in(struct frame_table_entry *fte, struct swap_page_block *spb) {
	struct supplemental_pte *spte = fte->spte;

	/* indicate the spte is not swapped */
	spte->spb = NULL;

	uint32_t i;
	fte->accessed = true;
	for (i = 0; i < BLOCKS_UNIT_NUMBER; i++) {
		block_read(swap_block, i+spb->block_sector_head,
				(void *)(fte->frame_addr+i*BLOCK_SECTOR_SIZE));
	}
	/* put the swap_page_block back into the swap pool */
	put_back_spb(spb);
}





/* swap out */
void swap_out(struct frame_table_entry *fte) {
	/* retrieve a free swap_page_block from the swap pool*/
	struct swap_page_block *spb = get_free_spb();
	struct supplemental_pte *spte = fte->spte;
	/* indicate the spte is swapped */
	spte->spb = spb;
	uint32_t i;
	fte->accessed = true;
	for (i = 0; i < BLOCKS_UNIT_NUMBER; i++) {
		block_write(swap_block, i+spb->block_sector_head,
				(void *)(fte->frame_addr+i*BLOCK_SECTOR_SIZE));
	}
}
















