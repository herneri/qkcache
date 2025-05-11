#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "qkc_db.h"
#include "qkc_index.h"

struct index_entry *qkc_recent_index_entry(struct qkc_database *database) {
	struct index_entry *loaded_entry = malloc(sizeof(struct index_entry));
	if (loaded_entry == NULL) {
		fprintf(stderr, "qkcache: Not enough memory to load recent index entry\n");
		exit(2);
	} else if (database->entry_count == 0) {
		return NULL;
	}

	fseek(database->index_file, sizeof(int), 1);
	fseek(database->index_file, sizeof(struct index_entry), database->entry_count - 1);

	fread(&loaded_entry->entry_number, 1, sizeof(int), database->index_file);
	fread(&loaded_entry->start_bytes, 1, sizeof(float), database->index_file);
	fread(&loaded_entry->end_bytes, 1, sizeof(float), database->index_file);

	rewind(database->index_file);
	return loaded_entry;
}

bool qkc_append_index_entry(struct qkc_database *database, const float data_size) {
	struct index_entry *recent_entry = qkc_recent_index_entry(database);
	struct index_entry new_entry;

	new_entry.entry_number = database->entry_count;
	new_entry.start_bytes = (recent_entry != NULL) ? recent_entry->end_bytes + 1 : sizeof(int);
	new_entry.end_bytes = new_entry.start_bytes + data_size;

	fseek(database->index_file, sizeof(int), 1);
	fseek(database->index_file, sizeof(struct index_entry), database->entry_count);

	fwrite(&new_entry.entry_number, 1, sizeof(int), database->index_file);
	fwrite(&new_entry.start_bytes, 1, sizeof(float), database->index_file);
	fwrite(&new_entry.end_bytes, 1, sizeof(float), database->index_file);

	rewind(database->index_file);
	free(recent_entry);
	return true;
}
