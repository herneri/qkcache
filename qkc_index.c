/*
	qkc_index.c: Index entry management, meant strictly for use with qkcache engine.

	Copyright (C) 2025  Eric Hernandez

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
 	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "qkc_db.h"
#include "qkc_index.h"

struct index_entry *qkc_recent_index_entry(struct qkc_database *database) {
	struct index_entry *loaded_entry = malloc(sizeof(struct index_entry));
	if (loaded_entry == NULL) {
		fprintf(stderr, "qkcache: Not enough memory to load recent index entry\n");
		exit(2);
	} else if (database->entry_count == 0) {
		free(loaded_entry);
		return NULL;
	}

	fseek(database->index_file, sizeof(int), SEEK_SET);
	fseek(database->index_file, sizeof(struct index_entry) * database->entry_count - 1, SEEK_CUR);

	fread(&loaded_entry->entry_number, sizeof(int), 1, database->index_file);
	fread(&loaded_entry->start_bytes, sizeof(int), 1, database->index_file);
	fread(&loaded_entry->end_bytes, sizeof(int), 1, database->index_file);

	rewind(database->index_file);
	return loaded_entry;
}

bool qkc_append_index_entry(struct qkc_database *database, const float data_size) {
	struct index_entry new_entry;

	new_entry.entry_number = database->entry_count;
	new_entry.start_bytes = (database->entry_count != 0) ? sizeof(int) + sizeof(struct index_entry) * database->entry_count + 1 : sizeof(int);
	new_entry.end_bytes = new_entry.start_bytes + data_size;

	fseek(database->index_file, 0, SEEK_END);
	qkc_increment_count(database);

	fwrite(&new_entry.entry_number, sizeof(int), 1, database->index_file);
	fwrite(&new_entry.start_bytes, sizeof(int), 1, database->index_file);
	fwrite(&new_entry.end_bytes, sizeof(int), 1, database->index_file);

	rewind(database->index_file);
	return true;
}

int qkc_pop_index_entry(struct qkc_database *database) {
	struct stat database_metadata;

	if (database->entry_count == 0) {
		return QKC_INDEX_EMPTY_DB;
	}

	if (stat(database->index_name, &database_metadata) == -1) {
		return QKC_INDEX_FS_FAIL;
	}

	truncate(database->index_name, database_metadata.st_size - sizeof(struct index_entry));
	return QKC_INDEX_OK;
}
