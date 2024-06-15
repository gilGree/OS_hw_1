#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "os.h"

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
	int curr_entry;
	int i;
	uint64_t* curr_table = phys_to_virt(pt<<12);

	for (i = 0; i < 4; i++) {
		curr_entry = (vpn >> (36-i*9)) & 0x1ff;
		if ((curr_table[curr_entry] & 1) == 0) {
			if (ppn == NO_MAPPING) {
				return;
			}
			curr_table[curr_entry] = (alloc_page_frame()<<12)+1; 
		}
		curr_table = phys_to_virt(curr_table[curr_entry]-1);
	}

	curr_entry = (vpn) & 0x1ff;
	if (ppn == NO_MAPPING) {
		curr_table[curr_entry] = 0;
		return;
	}
	curr_table[curr_entry] = (ppn<<12)+1;
	return;
}


uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
	int curr_entry,i;
	uint64_t* curr_table = phys_to_virt(pt<<12);

	for (i = 0;i < 5;i++) {
		curr_entry = (vpn >> (36-i*9)) & 0x1ff;
		if ((curr_table[curr_entry] & 1)==0) { 
			return NO_MAPPING;
		}
		if (i != 4) {
			curr_table = phys_to_virt(curr_table[curr_entry]-1);
		}
	}

	return curr_table[curr_entry]>>12;
}