#include "vm/page.h"
#include <hash.h>


/*hash function for supplemental page table (a hash table)*/
static unsigned hash_spte (const struct hash_elem *e, void *aux UNUSED) {
  struct supplemental_pte *spte = hash_entry (e, struct supplemental_pte, elem);
  return hash_int ((int)spte->uaddr);
}

/*hash less function for supplemental page table (a hash table)*/
static bool hash_less_spte (const struct hash_elem *a, const struct hash_elem *b,
		void *aux UNUSED) {
  struct supplemental_pte *spte1 = hash_entry (a, struct supplemental_pte, elem);
  struct supplemental_pte *spte2 = hash_entry (b, struct supplemental_pte, elem);
  return (spte1->uaddr < spte2->uaddr);
}


void spt_init(struct thread *t) {
	hash_init (&t->supplemental_pt, &hash_spte, &hash_less_spte, NULL);
}

