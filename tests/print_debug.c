#include <stdio.h>
#include "../qkc_db.h"
#include "../qkc_index.h"

void print_index(FILE *index_file) {
	struct index_entry retrieved_entry;
	fseek(index_file, sizeof(int), SEEK_SET);

	printf("=====\n");
	while (fread(&retrieved_entry, sizeof(struct index_entry), 1, index_file) != 0) {
		printf("%d\n", retrieved_entry.entry_number);
		printf("%d -> %d\n", retrieved_entry.start_bytes, retrieved_entry.end_bytes);
		printf("------\n");
	}
	printf("=====\n");

	rewind(index_file);
	return;
}
